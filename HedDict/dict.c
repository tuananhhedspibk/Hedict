#include <string.h>
#include "dict.h"
#include "command.h"
#include "btree/btree.h"
#include "btree_func.h"
#include <readline/readline.h>

int get_dict_by_name(const char* d_name){
	int i;
	for(i=0; i<MAX_DICT; ++i){
		if(list_dict[i].d_id >= 0){
			if(!strcmp(d_name, list_dict[i].d_name)){
				cmd_errno = E_NO_ERROR;
				return list_dict[i].d_id;
			}
		}
	}
	cmd_errno = E_DICT_NOT_FOUND;
	return -1;
}

int get_dict_by_path(const char* d_path){
	int i;
	for(i=0; i<MAX_DICT; ++i){
		if(list_dict[i].d_id >= 0){
			if(!strcmp(d_path, list_dict[i].d_path)){
				cmd_errno = E_NO_ERROR;
				return list_dict[i].d_id;
			}
		}
	}
	cmd_errno = E_DICT_NOT_FOUND;
	return -1;
}

int set_defaut_dict(int flag, const char* d_name){
	if(flag == FLAG_OFF){
		default_dict = -1;
	} else if(flag == FLAG_ON){
		int id = get_dict_by_name(d_name);
		if(id >= 0){
			default_dict = id;
		} else {
			cmd_errno = E_DICT_NOT_FOUND;
			return -1;
		}
	}
	cmd_errno = E_NO_ERROR;
	return cmd_errno;
}

int keep(const char* d_name){
	int d_id = get_dict_by_name(d_name);
	if(d_id >= 0){
		list_dict[d_id].keep_flag = FLAG_ON;
	} else{
		cmd_errno = E_DICT_NOT_FOUND;
		return -1;
	}
	cmd_errno = E_NO_ERROR;
	return cmd_errno;
}

int unkeep(const char* d_name){
	int d_id = get_dict_by_name(d_name);
	if(d_id >= 0){
		list_dict[d_id].keep_flag = FLAG_OFF;
	} else {
		cmd_errno = E_DICT_NOT_FOUND;
		return -1;
	}
	cmd_errno = E_NO_ERROR;
	return cmd_errno;
}

int drop(const char* d_name){
	int d_id = get_dict_by_name(d_name);
	if(d_id >= 0){
		free_dict(d_id);
		--num_dict;
		if(default_dict = d_id){
			set_defaut_dict(FLAG_OFF, NULL);
		}
		free_dict(d_id);
		cmd_errno = E_NO_ERROR;
		return cmd_errno;
	}
	cmd_errno = E_DICT_NOT_FOUND;
	return -1;
}

void free_dict(int d_id){
	list_dict[d_id].d_id = -1;
	btcls(list_dict[d_id].d_btact);	            
	btcls(list_dict[d_id].d_btact_soundex);	            
	char* cmd = malloc(MAX_VALUE_LENGTH);
	if(list_dict[d_id].keep_flag == FLAG_OFF){
		sprintf(cmd, "rm -rf %s_dic %s_sdx", list_dict[d_id].d_name, list_dict[d_id].d_name);
		system(cmd);
		free(cmd);
	}
}

int load(const char* d_name, const char* d_path){
	if(get_dict_by_name(d_name) >= 0){
        cmd_errno = E_DICT_NAME_USED;
		return -1;
	}
	if(get_dict_by_path(d_path) >= 0){
        cmd_errno = E_PATH_USED;
		return -1;
	}
	FILE* file=fopen(d_path, "r");
    if(file == NULL){
        cmd_errno = E_INVALID_PATH;
        return -1;
    }
    fclose(file);
    char* dict_name = (char*)malloc(MAX_VALUE_LENGTH);
    char* soundex_name = (char*)malloc(MAX_VALUE_LENGTH);
    sprintf(dict_name, "%s_dic", d_name);
    sprintf(soundex_name, "%s_sdx", d_name);
    FILE* b_dict_file = fopen(dict_name, "r");
    FILE* b_soundex_file = fopen(soundex_name, "r");
    if(b_dict_file != NULL && b_soundex_file != NULL){
    	int d_id;
   		for(d_id=0; d_id<MAX_DICT; ++d_id){
			if(list_dict[d_id].d_id < 0){
				list_dict[d_id].d_btact = btopn(dict_name, 0, 0);
        		list_dict[d_id].d_btact_soundex = btopn(soundex_name, 0, 0);
    			if(list_dict[d_id].d_btact != NULL && list_dict[d_id].d_btact_soundex != NULL){
    				strcpy(list_dict[d_id].d_name, d_name);
					strcpy(list_dict[d_id].d_path, d_path);
    				num_dict++;
					list_dict[d_id].d_id = d_id;
					cmd_errno = E_NO_ERROR;
					return cmd_errno;
    			}
    			break;
    		}
   		}	
    }
	int d_id;
	for(d_id=0; d_id<MAX_DICT; ++d_id){
		if(list_dict[d_id].d_id < 0){
			if((list_dict[d_id].d_btact = btcrt(dict_name, 0, 0)) != NULL 
				&& (list_dict[d_id].d_btact_soundex = btcrt(soundex_name, 0, 0)) != NULL){
				strcpy(list_dict[d_id].d_name, d_name);
				strcpy(list_dict[d_id].d_path, d_path);
				if(add_word(d_id) == -1){
					cmd_errno = E_LOAD_ERROR;
					return -1;
				} else{
					list_dict[d_id].d_id = d_id;
					num_dict++;
					cmd_errno = E_NO_ERROR;
					return cmd_errno;
				}
			}
			break;
		}
	}
	cmd_errno = E_FULL_DICT;
	return cmd_errno;
}

int del(int d_id, const char* pattern){
	if(!btdel(list_dict[d_id].d_btact, (char*)pattern)){
	    cmd_errno = E_NO_ERROR;
	    return cmd_errno;
	}
   	cmd_errno = E_PATTERN_NOT_FOUND;
   	return -1;
}

int add(int d_id, const char* pattern){
	BTint rsize;
	char *buffer = readline("Meaning: ");
    char *meaning = get_words(buffer, 0, -1);
    char *data = (char*)malloc(MAX_LINE_LENGTH);
    if(!btsel(list_dict[d_id].d_btact, (char*)pattern, data, MAX_LINE_LENGTH, &rsize)){
        cmd_errno = E_PATTERN_EXISTED;
        return -1;
    }
	btins(list_dict[d_id].d_btact, (char*)pattern, meaning, strlen(meaning));
    add_soundex(d_id, (char*)pattern);
	cmd_errno = E_NO_ERROR;
	return cmd_errno;
}

void init_list_dict(){
	int i;
	for(i=0; i<MAX_DICT; ++i){
		list_dict[i].d_id = -1;
		list_dict[i].d_name = (char*)malloc(MAX_VALUE_LENGTH);
		list_dict[i].d_path = (char*)malloc(MAX_VALUE_LENGTH);
		list_dict[i].keep_flag = FLAG_ON;
	}
	default_dict = -1;
}

int find(int d_id, const char* pattern){
    BTint rsize;
    char* data= malloc(MAX_LINE_LENGTH);
    if(!btsel(list_dict[d_id].d_btact, (char*)pattern, data, MAX_LINE_LENGTH, &rsize)){
		printf("%s: %s\n", pattern, data);
    	cmd_errno = E_NO_ERROR;
    	return cmd_errno;
    }
    cmd_errno = E_PATTERN_NOT_FOUND;
    return -1;    
}