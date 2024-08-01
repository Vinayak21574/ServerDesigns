#include "utils.h"

void constructFDset(struct pollfd here[MAX_CONN + 1]){

    memset(here,0,(MAX_CONN+1)*sizeof(struct pollfd));

    for(int i=0;i<MAX_CONN+1;i++){
        here[i].fd=-1;
    }

}

void getFD(struct pollfd here[MAX_CONN + 1]){
    // checker
    if(poll(here, MAX_CONN+ 1, -1)<0){
        perror("poll");
        exit(1);
    }
}

void setFD(struct pollfd here[],int fd,int*index){
    here[*index].fd=fd;
    here[*index].events=POLLIN;
    *index=*index+1;
}

int main(int argc,char* argv[]){
    int NUM_Clients=atoi(argv[1]);
    int server=start_server();

    struct pollfd FDset[MAX_CONN + 1];

    int index_at=0;

    constructFDset(FDset);
    setFD(FDset,server,&index_at);


    while(NUM_Clients){
        getFD(FDset);

        for (int fd_index=0;fd_index<MAX_CONN+1;fd_index++){
            if(FDset[fd_index].fd>=0 && FDset[fd_index].revents && POLLIN){
                if(fd_index==0){
                    int client=accept_client(server);
                    setFD(FDset,client,&index_at);
                    break;
                }
                else {
                    handle_client(FDset[fd_index].fd);
                    FDset[fd_index].fd=-1;
                    NUM_Clients--;
                }
            }
        }

    }

    return 0;
}