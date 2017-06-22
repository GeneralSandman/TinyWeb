#include <iostream>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char **argv)
{
    pid_t pid = pid_t(atoi(argv[1]));
    int sig;
    
    while (std::cin >> sig)
    {
        kill(pid, sig);
        std::cout << "send signal to " << atoi(argv[1]) << std::endl;
    }
    return 0;
}