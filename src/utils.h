#ifndef UTILS_H
#define UTILS_H

# include <stdio.h>
#include <sys/types.h>        
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/select.h>
#include <poll.h>
#include <signal.h>
#include <sys/epoll.h>
#include <time.h>
#include <sys/wait.h>


#define MAX_CONN 5000
#define MESSAGE "Welcome\n\0"

#define PORT 4040
// #define ADDR "127.0.0.1"
#define ADDR "192.168.0.4"
#define SIZE 1000000



int64_t factorial(char num[]){
    int NUM=atoi(num);

    NUM=NUM>20?20:NUM;
    
    int64_t res=1;

    while(NUM>0){
        res*=NUM--;
    }

    return res;
}


int start_server(){
    printf("Started server at %s:%d with pid %d\n",ADDR,PORT,getpid());

    int server= socket(AF_INET, SOCK_STREAM, 0);
    int status;

    //Checker
    if(server<0){
        perror("SocketCreation");
        exit(1);
    }
    else{
        // printf("SocketCreated\n");
    }

    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=inet_addr(ADDR);

    status=bind(server,(struct sockaddr*)&addr,sizeof(addr));
    //Checker
    if(status!=0){
        perror("Bind");
        exit(1);
    }
    else{
        // printf("SocketBinded at %s:%d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
    }

    //Checker
    status=listen(server,MAX_CONN);
    if(status!=0){
        perror("Listen");
        exit(1);
    }
    else{
        // printf("%s listening at %d\n",inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
    }

    return server;
}


void handle_client(int client){
    char buffer[SIZE];
    char res[SIZE];
    memset(buffer,'\0',SIZE);
    memset(res,'\0',SIZE);

    int status=-1;

    while(status!=0){
        if(status>0){
            // printf("Status(%d)->%d\n",client,status);
            // printf("Argument(%d)->%s\n",client,buffer);


            sprintf(res,"%ld", factorial(buffer));

            // printf("Sending(%d)->%s\n",client,res);

            send(client,res,strlen(res),0);

            memset(buffer,'\0',SIZE);
            
            status=-1;
        }
        else{
            status=recv(client,buffer,SIZE,0);
            // sleep(1);
        }
    }
    close(client);
    client=-1;
    return;
}


int accept_client(int server){
    // struct sockaddr_in addr_;
    // socklen_t here;

    // int client=accept(server,(struct sockaddr*)&addr_,&here);
    int client=accept(server,(struct sockaddr *) 0, (int *) 0);

    //Checker
    if(client<0){
        perror("AcceptClient");
        exit(1);
    }

    // printf("Connected to %s:%d \n",inet_ntoa(addr_.sin_addr),ntohs(addr_.sin_port));

    return client;
}


void* handle_client_thread(void* client){
    handle_client(*((int*)client));
    pthread_exit(NULL);
    free(client);
    return NULL;
}
// ulimit -n 8192
// ulimit -n

#endif