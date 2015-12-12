#include <string.h>
#include <ctype.h>
#include "tokenizer.h"
#include <stdlib.h>

char* dupstr(const char* str){
	char* result = (char*)malloc(strlen(str));
	strcpy(result, str);
	return result;
}
 
int del_space(char* str){
	int i=0, j=0, len = strlen(str);
	while(j<len && isspace(str[j])){
		j++;
	}
	while(j<len){
		while(isspace(str[j]) && isspace(str[j+1])){
			j++;
		}
		str[i++] = str[j++];
	}
	str[i]='\0';
	return i;
}

int is_end_space(char* str){
	int i=0;
	while(str[i++] != '\0');
	if(isspace(str[i-1]))
		return str[i-1];
	return 0;
}

int get_num_words(const char* str){
	int i=0, num_words=0, len = strlen(str);
	while(i<len){
		if(!isspace(str[i++])){
			while(i<len && !isspace(str[i++]));
			++num_words;
		}
	}
	return num_words;
}

char* get_words(const char* str, int pos, int offset){
	int len = strlen(str), i=0, j=0, num_words=0, pos_word=0;
	char* result = (char*)malloc(len);
	while(i<len && isspace(str[i])){
		i++;
	}
	while(i<len && pos_word++ < pos){
		while(i<len && !isspace(str[i])){
			++i;
		}
		while(i<len && isspace(str[i])){
			++i;
		}
	}
	while(i<len && (offset<0 || offset>num_words)){
		if(!isspace(str[i])){
			while(i<len && !isspace(str[i])){
				result[j++]=str[i++];
			}
			++num_words;
		} else {
			result[j++]=str[i++];
		}
	}
	result[j]='\0';
	return result;
}

int get_position(const char* str, int wordno){
	int i=0, j=0, len = strlen(str);
	while(i<len && isspace(str[i])){
		++i;
	}
	while(i<len && j++ < wordno){
		while(i<len && !isspace(str[i])){
			++i;
		}
		while(i<len && isspace(str[i])){
			++i;
		}
	}
	return i;
}

void Repair_string(char *str)
{
    int len = strlen(str);
    str[len-1]='\0';
}