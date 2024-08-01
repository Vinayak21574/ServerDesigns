#include "utils.h"

int constructFDset(struct epoll_event  here[MAX_CONN + 1]){

    memset(here,0,(MAX_CONN+1)*sizeof(struct epoll_event));

    for(int i=0;i<MAX_CONN+1;i++){
        here[i].data.fd=-1;
    }

    int value=epoll_create(1);

    if(value<0){
        perror("epoll_create");
        exit(1);
    }

    return value;

}

void getFD(int epoll,struct epoll_event here[MAX_CONN + 1]){
    // checker
    if(epoll_wait(epoll,here, MAX_CONN+ 1, -1)<0){
        perror("poll_wait");
        exit(1);
    }
}

void setFD(int epoll,int fd){
    struct epoll_event ideal;
    ideal.data.fd=fd;
    ideal.events=EPOLLIN;

    if(epoll_ctl(epoll,EPOLL_CTL_ADD,fd,&ideal)<0){
        perror("epoll_ctl_add");
        exit(1);
    }

}

void removeFD(int epoll,int fd){

    struct epoll_event ideal;
    ideal.data.fd=fd;
    ideal.events=EPOLLIN;

    if(epoll_ctl(epoll,EPOLL_CTL_DEL,fd,&ideal)<0){
        perror("epoll_ctl_del");
        exit(1);
    }

}


int main(int argc,char* argv[]){
    int NUM_Clients=atoi(argv[1]);
    int server=start_server();

    struct epoll_event FDset[MAX_CONN + 1];

    int epollFD=constructFDset(FDset);
    setFD(epollFD,server);

    while(NUM_Clients){
        getFD(epollFD,FDset);

        for (int fd_index=0;fd_index<MAX_CONN+1;fd_index++){
            if(FDset[fd_index].data.fd>=0){
                if(FDset[fd_index].data.fd==server){
                    int client=accept_client(server);
                    setFD(epollFD,client);
                    break;
                }
                else {
                    removeFD(epollFD,FDset[fd_index].data.fd);
                    handle_client(FDset[fd_index].data.fd);
                    NUM_Clients--;
                    break;
                }
            }
        }
    }

    return 0;
}