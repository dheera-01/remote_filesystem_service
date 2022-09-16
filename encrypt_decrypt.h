#define OFFSET 7
int mode_int = 0;

/* Function for substitute encryption. */
void encrypt1(char *str){
    int len = 0;
    int i = 0;
    while(str[i] != '\0'){
        i++;
        len++;
    }
    
    for(int i = 0; i<len; i++){
        if('a' <= str[i] && str[i] <= 'z'){
            str[i] = (str[i]-'a'+OFFSET)%26+'a';
        }else if('A' <= str[i] && str[i] <= 'Z'){
            str[i] = (str[i]-'A'+OFFSET)%26+'A';
        }else if('0' <= str[i] && str[i] <= '9'){
            str[i] = (str[i]-'0'+OFFSET)%26+'0';
        }
    }
    str[len] = '\0';
}

/* Function for substitute decryption. */
void decrypt1(char *str){
    int len = 0;
    int i = 0;
    while(str[i] != '\0'){
        i++;
        len++;
    }
    
    for(int i = 0; i<len; i++){
        if('a' <= str[i] && str[i] <= 'z'){
            str[i] = (str[i]-'a'-OFFSET+26)%26+'a';
        }else if('A' <= str[i] && str[i] <= 'Z'){
            str[i] = (str[i]-'A'-OFFSET+26)%26+'A';
        }else if('0' <= str[i] && str[i] <= '9'){
            str[i] = (str[i]-'0'-OFFSET+26)%26+'0';
        }
    }
    str[len] = '\0';
}

/* Function for transpose encryption. */
void encrypt2(char *str){
    int len = 0;
    int i = 0;
    while(str[i] != '\0'){
        i++;
        len++;
    }

    i = 0;
    int j = 0;
    while(i < len){
        if(str[i] != ' '){
            int j_temp;
            j = i;
            while(j < len && str[j] != ' '){
                j++;
            }
            
            j--;
            j_temp = j;
            
            while(i <= j){
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
                i++;
                j--;
            }
            i = j_temp;
        }
        i++;
    }
}

/* Function for decryption is applying encryption. */
void decrypt2(char *str){
    encrypt2(str);
}
