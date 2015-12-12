#include "btree_func.h"
#include "error.h"
#include "btree/btree.h"
#include <string.h>

char *soundex(char* In){
    int Iin, Iout;
    char C, PrevDig, *Out = (char *)malloc(10);
    if (FullFlag){
        Iin = 0;
        Iout = 0;
        PrevDig = '*';
    } else {
        Iin = 1; Iout = 1;
        Out[0] = tolower(In[0]);
        PrevDig = Out[0];
    }
    while ((In[Iin] != '\0') && (Iout <= 4)){
        In[Iin] = tolower(In[Iin]);
        switch (In[Iin]){
            case 'b' : C = '1'; break;
            case 'p' : C = '1'; break;
            case 'f' : C = '1'; break;
            case 'v' : C = '1'; break;
            case 'c' : C = '2'; break;
            case 's' : C = '2'; break;
            case 'k' : C = '2'; break;
            case 'g' : C = '2'; break;
            case 'j' : C = '2'; break;
            case 'q' : C = '2'; break;
            case 'x' : C = '2'; break;
            case 'z' : C = '2'; break;
            case 'd' : C = '3'; break;
            case 't' : C = '3'; break;
            case 'l' : C = '4'; break;
            case 'm' : C = '5'; break;
            case 'n' : C = '5'; break;
            case 'r' : C = '6'; break;
            default : C = '*';
        }
        if ((C != PrevDig) && (C != '*')){
            Out[Iout] = C;
            PrevDig = Out[Iout];
            Iout++;
        }
        Iin++;
    }
    if (Iout < 4){
        for (Iin=Iout; Iin<4; Iin++){
             Out[Iin] = '0';
        }
    }
    Out[4] = '\0';
    return Out;
}

void Solution_Soundex(int d_id, char* pattern){
    BTint rsize;
    char* data = malloc(MAX_LINE_LENGTH);
    char* sdx = soundex(pattern);
    printf("\n-------------------------------------------------------------------------------------------------");
    printf("\n\t\t\t\tDid You Mean\n");
    printf("-------------------------------------------------------------------------------------------------\n");
    if(!btsel(list_dict[d_id].d_btact_soundex, sdx, data, MAX_LINE_LENGTH, &rsize)){
        printf("%s", data);
    }
    printf("\n--------------------------------------------------------------------------------------------------\n");
}

void Support_string(char *str){
    int i=0;
    if(str[0]!=':'){
        while(i<strlen(str)){
            str[i]=str[i+1];
            i++;
        }
    } else {
        while(i<strlen(str)-1){
            str[i]=str[i+2];
            i++;
        }
    }
}

void add_soundex(int d_id, char* word){
    BTint rsize;
    char *data = malloc(MAX_LINE_LENGTH);
    char *sdx = soundex(word);
    if(btsel(list_dict[d_id].d_btact_soundex, sdx, data, MAX_LINE_LENGTH, &rsize)){
        strcpy(data, word);
        btins(list_dict[d_id].d_btact_soundex, sdx, data, strlen(data)+1);
    } else {
        if(strlen(data)+strlen(word)<999){
            strcat(data, "\n");
            strcat(data, word);
            btupd(list_dict[d_id].d_btact_soundex, sdx, data, strlen(data)+1);//Update data cho ma soundex
        }
    }
}

int add_word(int d_id){
    int rsize, n;
    char *line, *word, *mean;
    FILE* file = fopen(list_dict[d_id].d_path, "r");
    if(file==NULL){
        cmd_errno = E_INVALID_PATH;
        return -1;
    }
    word = malloc(MAX_NAME_LENGTH);
    line = malloc(MAX_LINE_LENGTH);
    mean = malloc(MAX_VALUE_LENGTH);
    while(fgets(line, MAX_LINE_LENGTH, file)!=NULL){
        Repair_string(line);
        if(line[0]=='['){
            continue;
        }
        if(line[0]==']'){
            break;
        }
        Delete_Space(line);
        if(line[0]!='/' && line[1]!='/'){
            word = strtok(line, "}");
            mean = strtok(NULL, "}");
            Support_string(mean);
            Support_string(word);
            btins(list_dict[d_id].d_btact, word, mean, strlen(mean));
            add_soundex(d_id, word);
        }
    }
    fclose(file);
    cmd_errno = E_NO_ERROR;
    return cmd_errno;
}

void Delete_Space(char *s){
    int i=0,count=0;
    while(i<strlen(s)){
        if(s[i]==' '){
            count++;
        }
        else{
            break;
        }
        i++;
    }
    if(!count){
        return ;
    }
    else{
        i=0;
        while(i<=strlen(s)-count){
            s[i]=s[i+count];
            i++;
        }
    }
}
