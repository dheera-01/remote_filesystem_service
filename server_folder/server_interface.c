#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../send_receive.h" /* Function definitions. */

/* Function for responding CWD to client. */
void command_CWD(int sock){
    char buffer[BUFFER_SIZE];
    getcwd(buffer,BUFFER_SIZE);
    
    printLog(sendString(sock,buffer),"CWD response.");
}

/* Function for responding LS to client. */
void command_LS(int sock){
    char buffer[BUFFER_SIZE];
    
    /* Save the output of ls command in output_server_ls.txt. */
    FILE *f = popen("ls > ls_temp.txt","r");
    pclose(f);
    
    printLog(sendFile(sock,"ls_temp.txt"),"LS response.");
}

/* Function for changing directory. */
void command_CD(int sock,char *path){
    printLog(chdir(path),"CD request.");
}

/* Function for sending filename file to client. */
void command_DWD(int sock,char *filename){
    printLog(sendFile(sock,filename),"DWD response.");
}

/* Function for receiving the file to be uploaded. */
void command_UPD(int sock,char *filename){
    printLog(receiveFile(sock,filename),"UPD request.");
}

void handleClient(int sock)
{
    
    char mode[2];
    mode[0] = '\0'; mode[1] = '\0';
    printf("Waiting for encryption mode: ");

    if (recv(sock, mode, 1, 0) <= 0){
        printLog(-1,"Mode response.");
    }

    mode_int = atoi(mode); /* mode_int is global variable declared in "encrypt_decrypt.h"*/
    
    printLog(1,"Mode response.");
    
    while(1){
        char buffer[BUFFER_SIZE];
        
        /* Clear the buffer. */
        for(int i = 0; i<BUFFER_SIZE; i++){
            buffer[i] = '\0';
        }
        
        /* Receive message from client. */
        if (recv(sock, buffer, BUFFER_SIZE-1, 0) <= 0){
            break;
        }
        
        directToDe(buffer);
    
        /* Separate command and args. and execute command accordingly. */
        printf("Command: %s\n",buffer);
        char command[5];
    
        for(int i = 0; i<5; i++){
            command[i] = '\0';
        }
    
        int itr = 0;
        int len = strlen(buffer);
        while(itr < len){
            if(buffer[itr] == ' '){
                break;
            }
            command[itr] = buffer[itr];
            itr++;
        }
        command[itr] = '\0';
    
        itr++;
        char args[BUFFER_SIZE-5];
        for(int i = 0; i<BUFFER_SIZE-5; i++){
            args[i] = '\0';
        }
    
        int index_args = 0;
        while(itr < len){
            args[index_args] = buffer[itr];
            if(buffer[itr] == '\0'){
                break;
            }
            itr++;
            index_args++;
        }
    
        /* Deal with the command accordingly. */
        if(strcmp(command,"CWD") == 0)
        {
            command_CWD(sock);
        }
        else if(strcmp(command,"LS") == 0)
        {
            command_LS(sock);
        }
        else if(strcmp(command,"CD") == 0)
        {
            command_CD(sock,args);
        }
        else if(strcmp(command,"DWD") == 0)
        {
            command_DWD(sock,args);
        }
        else if(strcmp(command,"UPD") == 0)
        {
            command_UPD(sock,args);
        }
        else
        {
            printf("Invalid command!\n");
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("FORMAT: %s <server-port>\n",argv[0]);
        exit(0);
    }

    int serverSocket;
    unsigned short serverPort;
    serverPort = atoi(argv[1]);

    /* Create socket on server-side. */
    int clientSocket;
    if ((serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        printf("Socket creation failed!\n");
        exit(0);
    }
    
    printf("Socket created successfully\n");
    /* Fill serverAddr struct values. */
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(serverPort);

    /* Bind to serverAddr. */
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){
        printf("Binding failed\n");
        exit(0);
    }
    printf("Binding successful\n");

    printf("Enabling listening.\n");
    if (listen(serverSocket, MAXPENDING) < 0){
        printf("Listening failed\n");
        exit(0);
    }

    struct sockaddr_in clientAddr;
    unsigned int len_client_struct;
    while(1)
    {
        len_client_struct = sizeof(clientAddr);

        printf("Waiting for a client to connect\n");
        if ((clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr,&len_client_struct)) < 0){
            printf("Accept failed\n");
            exit(0);
        }

        printf("Connected to %s\n",inet_ntoa(clientAddr.sin_addr));
        
        printf("----------\n");
        handleClient(clientSocket);
        printf("----------\n");
        
        printf("Disconnected from %s\n",inet_ntoa(clientAddr.sin_addr));
    }
    
    return 0;
}
