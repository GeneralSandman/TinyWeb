#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <error.h> 
#include <errno.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
 
#define BUF_SIZE 30 
 
int main(){ 
        int s[2]; 
        int w,r; 
        char * string = "This is a test string"; 
        char * buf = (char*)calloc(1 , BUF_SIZE); 
        pid_t pid; 
 
        if( socketpair(AF_UNIX,SOCK_STREAM,0,s) == -1 ){ 
                printf("create unnamed socket pair failed:%s\n",strerror(errno) ); 
                exit(-1); 
        } 
 
        /***********Test : fork but don't close any fd in neither parent nor child process***********/ 
        if( ( pid = fork() ) > 0 ){ 
                printf("Parent process's pid is %d\n",getpid()); 
                close(s[1]); 
                if( ( w = write(s[0] , string , strlen(string) ) ) == -1 ){ 
                        printf("Write socket error:%s\n",strerror(errno)); 
                        exit(-1); 
                } 
        }else if(pid == 0){ 
                printf("Fork child process successed\n"); 
                printf("Child process's pid is :%d\n",getpid()); 
                close(s[0]); 
        }else{ 
                printf("Fork failed:%s\n",strerror(errno)); 
                exit(-1); 
        } 
 
        /*****read***In parent and child****/ 
        if( (r = read(s[1], buf , BUF_SIZE )) == -1){ 
                printf("Pid %d read from socket error:%s\n",getpid() , strerror(errno) ); 
                exit(-1); 
        } 
        printf("Pid %d read string in same process : %s \n",getpid(),buf); 
        printf("Test successed , %d\n",getpid()); 
        exit(0); 
} 