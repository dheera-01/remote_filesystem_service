#include <stdbool.h>        /* For bool flag used. */
#include "encrypt_decrypt.h" /* Header file containing algorithms. */

/* Constants. */
#define BUFFER_SIZE 1024
#define MAXPENDING 5

/* Redirect to chosen encryption algorithm. */
void directToEn(char *str){
    if(mode_int == 0){
        return;
    }else if(mode_int == 1){
        encrypt1(str);
    }else if(mode_int == 2){
        encrypt2(str);
    }
}

/* Redirect to chosen decryption algorithm. */
void directToDe(char *str){
    if(mode_int == 0){
        return;
    }else if(mode_int == 1){
        decrypt1(str);
    }else if(mode_int == 2){
        decrypt2(str);
    }
}

/* Print passed string formatted. */
void printString(char *str){
    printf("%s\n",str);
}

/* Print LOGS. */
void printLog(int errStatus, char *errContext){
    if(errStatus == -1){
        printf("[ERROR] ");
    }else{
        printf("[SUCCESS] ");
    }
    printString(errContext);
}

/* Function for sending passed string to socket sock. */
int sendString(int sock,char *str){
    int sz = strlen(str);
    
    directToEn(str);
    
    int send_try = send(sock,str,sz,0);
    if(send_try < 0){
        return -1;
    }
    return 1;
}

/* Function for receiving string. */
void recvString(int sock,char *recvContext){
    char buffer[BUFFER_SIZE];
    
    for(int i = 0; i<BUFFER_SIZE; i++){
        buffer[i] = '\0';
    }
    
    int bytes_received = recv(sock,buffer,BUFFER_SIZE-1,0);
    if(bytes_received <= 0){
        printLog(-1,recvContext);
    }else{
        directToDe(buffer);
        printLog(1,buffer);
    }
}

/* Return -1 on fail and 1 on success. */
int sendFile(int sock,char *filename){
    char buffer[BUFFER_SIZE];
    
    FILE* f = fopen(filename,"r");
    if(f == NULL){
        return -1;
    }
    
    bool flag = false; /* File read finished or not. */
    while(1){
        /* Clear the buffer. */
        for(int i = 0; i<BUFFER_SIZE; i++){
            buffer[i] = '\0';
        }
        
        /* Fill the buffer. */
        for(int i = 0; i<BUFFER_SIZE; i++){
            buffer[i] = fgetc(f);
            if(buffer[i] == EOF){
                buffer[i] = EOF;
                flag = true;
                break;
            }
        }
        
        directToEn(buffer);
        
        /* Send the buffer. */
        int send_try = send(sock,buffer,BUFFER_SIZE,0);
        if(send_try == -1){
            return -1;
        }
        
        if(flag){
            break;
        }
    }
    fclose(f);
    return true;
}

/* Return -1 on fail and 1 on success. */
int receiveFile(int sock,char *filename){
    char buffer[BUFFER_SIZE];
    
    FILE* f = fopen(filename,"w");
    if(f == NULL){
        return -1;
    }
    
    bool flag = false; /* File read finished or not. */
    while(1){
        /* Clear the buffer. */
        for(int i = 0; i<BUFFER_SIZE; i++){
            buffer[i] = '\0';
        }
        
        /* Receive file. */
        int recv_try = recv(sock,buffer,BUFFER_SIZE,0);
        if(recv_try == -1){
            return -1;
        }

        directToDe(buffer);
        
        /* Fill the buffer. */
        for(int i = 0; i<BUFFER_SIZE; i++){
            if(buffer[i] == EOF){
                flag = true;
                break;
            }
            fputc(buffer[i],f);
        }
        
        if(flag){
            break;
        }
        
    }
    fclose(f);
    return 1;
}

/* Function equivalent to cat. */
int printFile(char *filename){
    printf("\n");
    FILE *f = fopen(filename,"r");
    if(f == NULL){
        return -1;
    }
    while(1){
        if(feof(f)){break;}
        printf("%c",fgetc(f));
    }
    return 1;
}
