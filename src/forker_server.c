#include "utils.h"

int main(int argc,char* argv[]){

    int NUM_Clients=atoi(argv[1]);
    int server=start_server();

    while(NUM_Clients){
        int client=accept_client(server);

        int fork_code=fork();

        //Checker
        if(fork_code<0){
            perror("Fork");
            exit(1);
        }

        if(fork_code==0){
            close(server);
            server=-1;
            handle_client(client);
            return 0;
        }
        else{
            NUM_Clients--;
        }

    }

    while(wait(NULL) > 0){};

    close(server);

    return 0;
}