#include "processpoll.h"
#include "../../api/api.h"
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

namespace poll
{
Poll *Poll::m_pInstance = NULL; //must please in the .cc file
//if he is in .h file,the complier will error

Poll::Poll(int listenfd, int process_number) : m_nListenfd(listenfd),
                                               m_nProcessTotalNumber(process_number),
                                               m_nIndex(-1),
                                               m_nStop(false)
{

    m_pProcess = new Process[process_number];

    for (int i = 0; i < process_number; i++)
    {
        if (socketpair(PF_UNIX, SOCK_STREAM, 0, m_pProcess[i].m_nPipefd) == -1)
        {
            char msg[] = "socketpair error";
            handle_error(msg);
        }

        m_pProcess[i].m_nPid = fork();
        if (m_pProcess[i].m_nPid == 0)
        {
            std::cout << "create child " << getpid() << std::endl;
            close(m_pProcess[i].m_nPipefd[1]);
            m_nIndex = i;
            break;
        }
        else if (m_pProcess[i].m_nPid > 0)
        {
            close(m_pProcess[i].m_nPipefd[0]);
            continue;
        }
        else
        {
            char msg[] = "fork error";
            handle_error(msg);
        }
    }
}

void Poll::setup_sig_pipe(void)
{
    m_nEpollfd = epoll_create(8);
    if (m_nEpollfd == -1)
    {
        char msg[] = "epoll create error";
        handle_error(msg);
    }

    m_nEpollfd = epoll_create(8);
    int res = socketpair(PF_UNIX, SOCK_STREAM, 0, sigpipefd);
    if (res == -1)
    {
        char msg[] = "sockpair error";
        handle_error(msg);
    }
    setnoblocking(sigpipefd[1]);
    epolladdfd(m_nEpollfd, sigpipefd[0]);

    add_signal(SIGHUP, signal_handler);
    add_signal(SIGCHLD, signal_handler);
    add_signal(SIGTERM, signal_handler);
    add_signal(SIGINT, signal_handler);
    add_signal(SIGPIPE, signal_handler);
}

void Poll::run(void)
{
    if (m_nIndex != -1)
        m_fRunChild();
    else
        m_fRunParent();
}

void Poll::m_fRunChild(void)
{
    setup_sig_pipe();

    int getmsgPipe = m_pProcess[m_nIndex].m_nPipefd[0];
    epolladdfd(m_nEpollfd, getmsgPipe);

    epoll_event arrives[8];

    Service *service = new Service();

    while (!m_nStop)
    {
        int number = epoll_wait(m_nEpollfd, arrives, 8, -1);
        for (int i = 0; i < number; i++)
        {
            int sockfd = arrives[i].data.fd;
            if (sockfd == getmsgPipe && (arrives[i].events & EPOLLIN))
            {
                //get message from parent process,
                //and connect the listenfd
                int client = 0;
                int res = read(sockfd, (char *)&client, sizeof(client));
                if (res > 0)
                {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int connect_fd = accept(m_nListenfd, (struct sockaddr *)&client_addr, &client_len);
                    epolladdfd(m_nEpollfd, connect_fd);
                    //init the user function
                    service->init(connect_fd, client_addr);
                }
            }
            else if (sockfd == sigpipefd[0] && (arrives[i].events & EPOLLIN))
            {
                //get signal and
                int sig;
                char signals[1024];
                int res = read(sockfd, signals, sizeof(signals));
                if (res > 0)
                {
                    for (int i = 0; i < res; i++)
                    {
                        switch (signals[i])
                        {
                        case SIGCHLD:
                        {
                            pid_t pid;
                            int stat;
                            while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
                            {
                                continue;
                            }
                            break;
                        }
                        case SIGTERM:
                        case SIGINT:
                        {
                            m_nStop = true;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                        }
                    }
                }
            }
            else if (arrives[i].events & EPOLLIN)
            {
                //receive data from client
                std::cout << "receive data\n";
                service->proc();
            }
        }
    }
}

void Poll::m_fRunParent(void)
{
    setup_sig_pipe();
    epolladdfd(m_nEpollfd, m_nListenfd);

    epoll_event arrives[8];

    while (!m_nStop)
    {
        int number = epoll_wait(m_nEpollfd, arrives, 8, -1);
        for (int i = 0; i < number; i++)
        {
            int sockfd = arrives[i].data.fd;
            if (sockfd == m_nListenfd)
            {
                //have a new connetion,
                //select a child to finish it,
                //send message to child,
                //child will connect the client
                //and

                int i = (m_nProcessCounter + 1) % m_nProcessTotalNumber;

                do
                {
                    if (!m_pProcess[i].m_nPid)
                    {
                        break;
                    }
                    i = (i + 1) % m_nProcessTotalNumber;

                } while (i != m_nProcessCounter);

                if (!m_pProcess[i].m_nPid)
                {
                    m_nStop = true;
                    break;
                }

                m_nProcessCounter = i;
                int new_conn = 1;
                std::cout << "select child:" << i << std::endl;
                write(m_pProcess[i].m_nPipefd[1], (char *)&new_conn, sizeof(new_conn));
            }
            else if (sockfd == sigpipefd[0] && (arrives[i].events & EPOLLIN))
            {
                //get signal

                //get signal and
                int sig;
                char signals[1024];
                int res = read(sockfd, signals, sizeof(signals));
                if (res > 0)
                {
                    for (int i = 0; i < res; i++)
                    {
                        switch (signals[i])
                        {
                        case SIGCHLD:
                        {
                            pid_t pid;
                            int stat;
                            while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
                            {
                                for (int i = 0; i < m_nProcessTotalNumber; i++)
                                {
                                    if (m_pProcess[i].m_nPid == pid)
                                    {
                                        std::cout << "child " << i << " join" << std::endl;
                                        close(m_pProcess[i].m_nPipefd[1]);
                                        m_pProcess[i].m_nPid = -1;
                                    }
                                }
                            }

                            m_nStop = true;
                            for (int i = 0; i < m_nProcessTotalNumber; i++)
                            {
                                if (m_pProcess[i].m_nPid != -1)
                                {
                                    m_nStop = false;
                                    break;
                                }
                            }

                            break;
                        }
                        case SIGTERM:
                        case SIGINT:
                        {
                            //kill all
                            std::cout << "kill all the child\n";
                            for (int i = 0; i < m_nProcessTotalNumber; i++)
                            {
                                int pid = m_pProcess[i].m_nPid;
                                if (pid != -1)
                                    kill(pid, SIGTERM);
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                        }
                    }
                }
            }
        }
    }

    // close(m_nListenfd);
    close(m_nEpollfd);
}

void Service::init(int sockfd, const sockaddr_in &client_addr)
{
    m_nConnectFd = sockfd;
    m_nClientAddr = client_addr;
}

void Service::proc()
{

    int res = read(m_nConnectFd, m_nBuf, sizeof(m_nBuf));
    std::cout << "receive:"<<m_nBuf<<"---\n";
    for(char *tmp=m_nBuf;*tmp!='\0';tmp++)
        printf("%d ",*tmp);
    std::cout<<std::endl;
    if (res > 0)
    {
        using namespace std;
        vector<string> header;
        header.reserve(3);
        header.push_back("HTTP/1.0 200 OK\r\n");
        header.push_back("Content-Type: text/html\r\n\r\n");
        for (auto t : header)
        {
            writeString(m_nConnectFd, t);
        }
        std::string file = "home.html";
        writeHtml(m_nConnectFd, file);
    }
    close(m_nConnectFd);
}
}