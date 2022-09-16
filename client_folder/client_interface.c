#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../send_receive.h" /* Function definitions. */

/* Function for command CWD. */
void command_CWD(int sock,char *command){
    printLog(sendString(sock,command),"CWD request.");
    recvString(sock,"CWD response.");
}

/* Function for command LS. */
void command_LS(int sock,char *command){
    printLog(sendString(sock,command),"LS request.");
    printLog(receiveFile(sock,"ls_temp.txt"),"LS response.");
    printLog(printFile("ls_temp.txt"),"File printing.");
}

/* Function for command CD. */
void command_CD(int sock,char *command){
    /* CD to path on server. */
    printLog(sendString(sock,command),"CD request.");
}

/* Function for downloading command_file from server. */
void command_DWD(int sock,char *command,char *filename){
    printLog(sendString(sock,command),"DWD request.");
    printLog(receiveFile(sock,filename),"DWD response.");
}

/* Function for uploading command_file to server. */
void command_UPD(int sock,char *command,char *filename){
    printLog(sendString(sock,command),"UPD request.");
    printLog(sendFile(sock,filename),"UPD response.");
}


/* Actively look for client commands */
void handle_io(int sock){
    char mode[3];
    printf("Enter the encryption mode: ");
    
    fgets(mode,3,stdin);
    mode[strcspn(mode,"\n")] = 0;

    mode_int = atoi(mode); /* mode_int is global variable declared in "encrypt_decrypt.h"*/
    
    int send_try = send(sock,mode,2,0);
    if(send_try < 0){
        return;
    }else{
        printLog(1,"Sending encryption mode.");
    }
    
    char input_command[BUFFER_SIZE];
    while(1){
        printf("\n>> ");

        /* Clear the input command. */
        for(int i = 0; i<BUFFER_SIZE; i++){
            input_command[i] = '\0';
        }
        
        fgets(input_command,BUFFER_SIZE,stdin);
        input_command[strcspn(input_command,"\n")] = 0;
        
        /* Extract the command provided only. */
        char command[5];
        
        for(int i = 0; i<5; i++){
            command[i] = '\0';
        }
        
        int itr = 0;
        int len = strlen(input_command);
        while(itr < len){
            if(input_command[itr] == ' '){
                break;
            }
            command[itr] = input_command[itr];
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
            args[index_args] = input_command[itr];
            if(input_command[itr] == '\0'){
                break;
            }
            itr++;
            index_args++;
        }
        
        if(strcmp(input_command,"CWD") == 0){
            /* Call for executing CWD on server. */
            command_CWD(sock,command);
            
        }else if(strcmp(input_command,"LS") == 0){
            /* Call for executing LS on server. */
            command_LS(sock,command);
            
        }else if(strcmp(command,"CD") == 0){
            /* Call for executing CD .. on server. */
            command_CD(sock,input_command);
            
        }else if(strcmp(command,"DWD") == 0){
            /* Call for downloading file from server. */
            command_DWD(sock,input_command,args);
            
        }else if(strcmp(command,"UPD") == 0){
            /* Call for uploading file on server. */
            command_UPD(sock,input_command,args);
            
        }else if(strcmp(command,"EXIT") == 0){
            break;
        }
        else if(strcmp(command,"HELP") == 0){
            printf("Commands:\n");
            printf("1. HELP -- get help.\n");
            printf("2. EXIT -- exit client application.\n");
            printf("3. CWD -- get current working directory.\n");
            printf("4. LS -- get list of files and directories.\n");
            printf("5. CD <path> -- change cwd to path.\n");
            printf("6. DWD <filename> -- download filename file from server.\n");
            printf("7. UPD <filename> -- upload filename file to server.\n");
        }
        else{
            printf("Invalid Command!\n");
            system(input_command);
            printf("\n");
        }
    }
    return;
}

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serverAddr;
    unsigned short serverPort;
    char *serverIP;

    if (argc != 3)
    {
        printf("FORMAT: %s <server-ip> <server-port>\n",argv[0]);
        exit(0);
    }

    serverIP = argv[1];
    serverPort = atoi(argv[2]);
    
    /* Create socket on client side. */
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        printf("Socket creation failed!\n");
        exit(0);
    }

    /* Fill server address structure. */
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);
    serverAddr.sin_port = htons(serverPort);

    if(connect(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){
        printf("Connection failed!\n");
        exit(0);
    }

    printf("Connected to server[%s]\n",serverIP);
    handle_io(sock);
    close(sock);
    printf("Disconnected from server[%s]\n",serverIP);
    
    printf("\n");
    exit(0);
}
