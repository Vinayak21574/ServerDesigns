#include "utils.h"

typedef struct myFDset{
    fd_set read;
    fd_set write;

    fd_set read_main;
    fd_set write_main;

    int fd_max;

}myFDset;

myFDset constructFDset(){
    myFDset FDset;
    
    FD_ZERO(&FDset.read_main);
    FD_ZERO(&FDset.write_main); 
    FDset.fd_max=-1;

    return FDset;
}

void getFD(myFDset* here){
    here->read=here->read_main;
    here->write=here->write_main;

    // checker
    if(select(FD_SETSIZE,&here->read,&here->write,NULL,NULL)<0){
        perror("select");
        // exit(1);
    }
}

void setFD(myFDset* here,int fd){
    here->fd_max=(fd>here->fd_max)?fd:here->fd_max;

    FD_SET(fd,&here->read_main);
    FD_SET(fd,&here->write_main);

}

void removeFD(myFDset* here,int fd){
    FD_CLR(fd,&here->read_main);
    FD_CLR(fd,&here->write_main);
}

int main(int argc,char* argv[]){
    int NUM_Clients=atoi(argv[1]);
    int server=start_server();

    myFDset FDset=constructFDset();
    
    setFD(&FDset,server);

    while(NUM_Clients){
        getFD(&FDset);

        for (int fd_index=0;fd_index<=FD_SETSIZE;fd_index++){
            int index=fd_index;
            if(FD_ISSET(fd_index,&FDset.read)){
                if(fd_index==server){
                    int client=accept_client(server);
                    setFD(&FDset,client);
                    break;
                }
                else if(FD_ISSET(index,&FDset.write)){
                    handle_client(index);
                    removeFD(&FDset,index);
                    NUM_Clients--;
                }
            }
        }

    }
    return 0;
}