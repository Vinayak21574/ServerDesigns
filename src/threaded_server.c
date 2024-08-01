#include "utils.h"

int main(int argc,char* argv[]){
    int NUM_Clients=atoi(argv[1]);
    int server=start_server();

    pthread_t threads[NUM_Clients];
    int at_index=0;

    while(NUM_Clients){
        int* client=(int*)malloc(sizeof(int));
        *client=accept_client(server);

        pthread_t thread_client;

        int thread_code=pthread_create(&thread_client,NULL,handle_client_thread,(void*)client);
        threads[at_index++]=thread_client;
        NUM_Clients--;

        // checker
        if(thread_code!=0){
            perror("ThreadCreate");
            exit(1);
        }
    }

    for(int i=0;i<sizeof(threads)/sizeof(pthread_t);i++){
        pthread_join(threads[i],NULL);
    }

    close(server);

    return 0;
}