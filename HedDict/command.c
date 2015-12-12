#include <stdio.h>
#include <string.h>
#include <time.h>
#include <readline/readline.h>
#include "tokenizer.h"
#include "command.h"
#include "dict.h"
#include "error.h"
#include "btree/btree.h"
#include "btree_func.h"
#include "tab.h"

COMMAND* get_command(const char* cmdline){
	COMMAND* cmd = (COMMAND*)malloc(sizeof(COMMAND));
	if(cmd == NULL){
		cmd_errno = E_MEMORY_ALLOCATE;
		return NULL;
	}
	if(fill_param(cmd, cmdline) != -1)
		return cmd;
	return NULL;
}

int fill_param(COMMAND *cmd, const char* cmdline){
	cmd->num_param = get_num_words(cmdline)-1;
	cmd->name = get_words(cmdline, 0, 1);
	cmd->id = get_command_id(cmd->name);
	if(cmd->id != -1){
		int i;
		cmd->param = (PARAM*)malloc(sizeof(PARAM)*NUM_PARAMS);
		for(i=0; i<NUM_PARAMS; ++i){
			cmd->param[i].flag = FLAG_OFF;
			cmd->param[i].value = (char*)malloc(MAX_VALUE_LENGTH);
			cmd->param[i].name = (char*)malloc(MAX_NAME_LENGTH);
			cmd->param[i].id = i;
		}

		if(cmd->id == C_DROP || cmd->id == C_KEEP || cmd->id == C_UNKEEP){
			if(cmd->num_param != 1){
				cmd_errno = E_INVALID_PARAM;
				return -1;
			}
			cmd->param[P_DICT].flag = FLAG_ON;
			char* value = get_words(cmdline, 1, 1);
			strcpy(cmd->param[P_DICT].value, value);
			cmd_errno = E_NO_ERROR;
			return cmd->num_param;
		}

		if(cmd->id == C_INFO){
			for(i=1; i<=cmd->num_param; ++i){
				char* param = get_words(cmdline, i, 1);
				if(!strcasecmp(param, "version")){
					cmd->param[P_VERSION].flag = FLAG_ON;
				} else if(!strcasecmp(param, "dict")){
					cmd->param[P_DICT].flag = FLAG_ON;
				} else if(!strcasecmp(param, "all")){
					cmd->param[P_VERSION].flag = FLAG_ON;
					cmd->param[P_DICT].flag = FLAG_ON;
				} else {
					cmd_errno = E_INVALID_PARAM;
					return -1;
				}
			}
			cmd_errno = E_NO_ERROR;
			return cmd->num_param;
		}

		if(cmd->id == C_SET){
			for(i=1; i<=cmd->num_param; ++i){
				char* param = get_words(cmdline, i, 1);
				if(!strcasecmp(param, "test")){
					cmd->param[P_TEST].flag = FLAG_ON;
				} else if(!strncasecmp(param, "dict=", 5) && strlen(param)>5){
					cmd->param[P_DICT].flag = FLAG_ON;
					strcpy(cmd->param[P_DICT].value, param+5);
				} else {
					cmd_errno = E_INVALID_PARAM;
					return -1;
				}
			}
			cmd_errno = E_NO_ERROR;
			return cmd->num_param;
		}
		
		if(cmd->id == C_UNSET){
			for(i=1; i<=cmd->num_param; ++i){
				char* param = get_words(cmdline, i, 1);
				if(!strcasecmp(param, "dict")){
					cmd->param[P_DICT].flag = FLAG_ON;
				} else if(!strcasecmp(param, "all")){
					cmd->param[P_TEST].flag = FLAG_ON;
					cmd->param[P_DICT].flag = FLAG_ON;
				}else if(!strcasecmp(param, "test")){
					cmd->param[P_TEST].flag = FLAG_ON;
				}else {
					cmd_errno = E_INVALID_PARAM;
					return -1;
				}
			}
			cmd_errno = E_NO_ERROR;
			return cmd->num_param;
		}
		
		if(cmd->id == C_FIND || cmd->id == C_ADD || cmd->id == C_DEL){
			if(!cmd->num_param){
				if(default_dict >= 0){
					cmd->param[P_DICT].flag = FLAG_ON;
					cmd->param[P_PATTERN].flag = FLAG_OFF;
					strcpy(cmd->param[P_DICT].value, list_dict[default_dict].d_name);
				}
			} else if(cmd->num_param == 1){
				if(cmd->num_param == 1){
					cmd->param[P_DICT].flag = FLAG_ON;
					cmd->param[P_PATTERN].flag = FLAG_OFF;
					char* param = get_words(cmdline, 1, 1);
					strcpy(cmd->param[P_DICT].value, param);
				} 
			} else {
				cmd->param[P_PATTERN].flag = FLAG_ON;
				cmd->param[P_DICT].flag = FLAG_ON;
				char* param1 = get_words(cmdline, 1, 1);
				char* param2 = get_words(cmdline, 2, -1);
				strcpy(cmd->param[P_PATTERN].value, param2);
				strcpy(cmd->param[P_DICT].value, param1);
			} 
			cmd_errno = E_NO_ERROR;
			return cmd->num_param;
		}

		if(cmd->id == C_LOAD){
			if(cmd->num_param == 2){
				cmd->param[P_PATH].flag = FLAG_ON;
				cmd->param[P_DICT].flag = FLAG_ON;
				char* param1 = get_words(cmdline, 1, 1);
				char* param2 = get_words(cmdline, 2, 1);
				strcpy(cmd->param[P_DICT].value, param2);
				strcpy(cmd->param[P_PATH].value, param1);
			} else {
				cmd_errno = E_INVALID_PARAM;
				return -1;
			}
			cmd_errno = E_NO_ERROR;
			return cmd->num_param;
		}

		if(cmd->id == C_HELP || cmd->id == C_EXIT){
			cmd_errno = E_NO_ERROR;
			return cmd->num_param;
		}
	}
	cmd_errno = E_INVALID_COMMAND_NAME;
	return -1;
}

void free_command(COMMAND* cmd){
	int i;
	for(i=0; i<NUM_PARAMS; ++i){
		free(cmd->param[i].value);
		free(cmd->param[i].name);
	}
	free(cmd->param);
	free(cmd->name);
	free(cmd);
}

int exec_command(COMMAND *cmd){
	switch(cmd->id){
		case C_SET:	
			return c_set(cmd);
		case C_UNSET:
			return c_unset(cmd);
		case C_HELP:
			return c_help();
		case C_LOAD:
			return c_load(cmd);
		case C_FIND:
			return c_find(cmd);
		case C_ADD:
			return c_add(cmd);
		case C_DEL:
			return c_del(cmd);
		case C_INFO:	
			return c_info(cmd);		
		case C_DROP:
			return c_drop(cmd);	
		case C_EXIT:
			return c_exit();		
		case C_KEEP:
			return c_keep(cmd);
		case C_UNKEEP:
			return c_unkeep(cmd);			
	}
	return 0;
}

int c_info(COMMAND *cmd){
	if(cmd->param[P_VERSION].flag == FLAG_ON){
		printf(VERSION);
		printf("\n");
	}
	if(test_mode == FLAG_ON){
		printf("This app is running with test mode.\n");
	} else {
		printf("This app is running with non-test mode.\n");
	}
	if(cmd->param[P_DICT].flag == FLAG_ON){
		if(num_dict <=0){
			printf("No dictionary available.\n");
		} else{
			printf("Available dictionaries: \nID\tName\tFiles\n");
			int i;
			for(i=0; i<MAX_DICT; ++i){
				if(list_dict[i].d_id>=0){
					printf("%d\t%s\t%s", list_dict[i].d_id, list_dict[i].d_name, list_dict[i].d_path);
					if(list_dict[i].keep_flag == FLAG_ON){
						printf("(kept)");
					}
					printf("\n");
				}
			}
			if(default_dict >= 0){
				printf("\nDefault dictionary: %s\n", list_dict[default_dict].d_name);
			} else{
				printf("\nNo default dictionary.\n");		
			}
		}
	cmd_errno = E_NO_ERROR;
	return E_NO_ERROR;
	}
}

int c_set(COMMAND* cmd){
	if(cmd->param[P_TEST].flag == FLAG_ON){
		test_mode = FLAG_ON;
	}
	if(cmd->param[P_DICT].flag == FLAG_ON){
		return set_defaut_dict(FLAG_ON, cmd->param[P_DICT].value);
	}
	cmd_errno = E_NO_ERROR;			
	return E_NO_ERROR;
}

int c_unset(COMMAND* cmd){
	if(cmd->param[P_TEST].flag == FLAG_ON){
		test_mode = FLAG_OFF;
	}
	if(cmd->param[P_DICT].flag == FLAG_ON){
		return set_defaut_dict(FLAG_OFF, NULL);
	}
	return E_NO_ERROR;
}

int c_drop(COMMAND* cmd){
	return drop(cmd->param[P_DICT].value);
}

int c_keep(COMMAND* cmd){
	return keep(cmd->param[P_DICT].value);
}

int c_unkeep(COMMAND* cmd){
	return unkeep(cmd->param[P_DICT].value);
}

int c_exit(COMMAND* cmd){
	int d_id;
	for(d_id=0; d_id<MAX_DICT; ++d_id){
		if(d_id >=0 && d_id < MAX_DICT){
			free_dict(d_id);
		}
	}
	printf("Bye.\n");
	exit(0);
}

int c_load(COMMAND* cmd){
	clock_t start = clock();
    printf("Loading...\n");
  	int result = load(cmd->param[P_DICT].value, cmd->param[P_PATH].value);
	if(result != -1){
		clock_t stop = clock();
		if(test_mode == FLAG_ON){
			printf("Loaded in %f miliseconds.\n", (double)(stop - start) *1000/ CLOCKS_PER_SEC);
		} else if(test_mode == FLAG_OFF){
			printf("Loaded.\n");
		}
	}
	return result;
}

int c_find(COMMAND* cmd){
	if(cmd->param[P_DICT].flag == FLAG_OFF && default_dict < 0){
		cmd_errno = E_INVALID_PARAM;
		return -1;
	}
	int d_id;
	if(cmd->param[P_DICT].flag == FLAG_ON){
		d_id = get_dict_by_name(cmd->param[P_DICT].value);
		if(d_id < 0){
			cmd_errno = E_DICT_NOT_FOUND;
			return -1;
		}
	}
	else{
		d_id = default_dict;
	}
	if(cmd->param[P_PATTERN].flag == FLAG_OFF){
		tab_complt(list_dict[d_id].d_btact, cmd->param[P_PATTERN].value);
		printf("\n");
	}
	clock_t start = clock();
	int	result = find(d_id, cmd->param[P_PATTERN].value);
	if(result == -1){
		if(cmd_errno == E_PATTERN_NOT_FOUND){
			c_error(cmd, NULL);
			Solution_Soundex(d_id, cmd->param[P_PATTERN].value);
		}
	}
	clock_t stop = clock();
	if(test_mode == FLAG_ON){
	 	printf("Found in %f miliseconds.\n", ((double)stop - (double)start) *1000.0/ CLOCKS_PER_SEC);
	}
	cmd_errno = E_NO_ERROR;
	return E_NO_ERROR;
}

int c_add(COMMAND* cmd){
	if(cmd->param[P_PATTERN].flag == FLAG_OFF){
		cmd->param[P_PATTERN].value = readline("Type your new word: ");
	}
	clock_t start = clock();
	int d_id = get_dict_by_name(cmd->param[P_DICT].value);
	int add_code = add(d_id, cmd->param[P_PATTERN].value);
	if(add_code != -1 && test_mode == FLAG_ON){
		clock_t stop = clock();
		printf("Added in %f miliseconds.\n", ((double)stop - (double)start) *1000.0/ CLOCKS_PER_SEC);
	}
	return add_code;
}

int c_del(COMMAND* cmd){
	int d_id = get_dict_by_name(cmd->param[P_DICT].value);
	if(cmd->param[P_PATTERN].flag == FLAG_OFF){
		tab_complt(list_dict[d_id].d_btact, cmd->param[P_PATTERN].value);
		printf("\n");
	}
	clock_t start = clock();
	int del_code = del(d_id, cmd->param[P_PATTERN].value);
	if(del_code != -1 && test_mode == FLAG_ON){
		clock_t stop = clock();
		printf("Deleted in %f miliseconds.\n", ((double)stop - (double)start) *1000.0/ CLOCKS_PER_SEC);
	}
	return del_code;
}

int c_help(){
	printf("Dictionary 1.0\n");
	printf("Working with dictionaries by using command line interface\n");
	printf("List of commands:\n");
	printf("\tSET    [TEST]    [DICT=dictname]\n");
	printf("\tUNSET  [TEST]    [DICT=dictname]  [ALL]\n");
	printf("\tINFO   [VERSION]  [DICT] [ALL]\n");
	printf("\tLOAD   path_to_dict dictname\n");
	printf("\tDROP   dictname\n");
	printf("\tKEEP   [dictname]\n");
	printf("\tUNKEEP [dictname]\n");
	printf("\tFIND   [dictname] pattern\n");
	printf("\tADD    [dictname] pattern\n");
	printf("\tDEL    [dictname] pattern\n");
	printf("\tEXIT\n");
	printf("Note that we don't have to type [dictname] in FIND, ADD, DEL command if we 've typed SET DICT=dictname before\n");
	cmd_errno = E_NO_ERROR;
	return cmd_errno;
}

void c_error(COMMAND* cmd, const char* cmdline){
	switch(cmd_errno){
		case E_NO_ERROR:
			return;
		case E_MEMORY_ALLOCATE:
			printf("Memory allocation failed!!!\n");
			return;
		case E_INVALID_COMMAND_NAME:
			printf("No such command: %s. Type 'help' to see help.\n", get_words(cmdline, 0, 1));
			return;
		case E_INVALID_PARAM:
			printf("Syntax error: %s\n", get_words(cmdline, 0, 1));
			return;
		case E_DICT_NOT_FOUND:
			printf("Dictionary not found: %s\n", cmd->param[P_DICT].value);
			return;
		case E_INVALID_PATH:
			printf("Invalid dictionary file: %s\n", cmd->param[P_PATH].value);
			return;
		case E_PATTERN_NOT_FOUND:
			printf("Pattern not found: %s\n", cmd->param[P_PATTERN].value);
			return;
		case E_PERMISSION:
			printf("Permission denied!!!\n");
			return;
		case E_DICT_NAME_USED:
			printf("Dictionary name duplicated: %s\n", cmd->param[P_DICT].value);
			return;
		case E_PATH_USED:
			printf("Dictionary used!!!\n");
			return;
		case E_PATTERN_EXISTED:
			printf("Word existed: %s\n", cmd->param[P_PATTERN].value);
			return;
	}
	return;
}

void init_commands_info(){
	commands_info = (COMMAND*)malloc(sizeof(COMMAND)*NUM_COMMANDS);
	int i;
	for(i=0; i<NUM_COMMANDS; ++i){
		commands_info[i].id = i;
		commands_info[i].name = (char*)malloc(16);
		commands_info[i].num_param = NUM_PARAMS;
		commands_info[i].param = (PARAM*)malloc(sizeof(PARAM)*NUM_PARAMS);
		int j;
		for(j=0; j<NUM_PARAMS; ++j){
			commands_info[i].param[j].id = j;
			commands_info[i].param[j].name = (char*)malloc(MAX_VALUE_LENGTH);
		}
	}
	strcpy(commands_info[C_SET].name, "set");
	strcpy(commands_info[C_UNSET].name, "unset");
	strcpy(commands_info[C_HELP].name, "help");
	strcpy(commands_info[C_FIND].name, "find");
	strcpy(commands_info[C_ADD].name, "add");
	strcpy(commands_info[C_DEL].name, "del");
	strcpy(commands_info[C_INFO].name, "info");
	strcpy(commands_info[C_DROP].name, "drop");
	strcpy(commands_info[C_LOAD].name, "load");
	strcpy(commands_info[C_EXIT].name, "exit");
	strcpy(commands_info[C_UNKEEP].name, "unkeep");
	strcpy(commands_info[C_KEEP].name, "keep");
	
	strcpy(commands_info[C_SET].param[P_TEST].name, "test");
	strcpy(commands_info[C_SET].param[P_DICT].name, "dict=");
	
	strcpy(commands_info[C_UNSET].param[P_TEST].name, "test");
	strcpy(commands_info[C_UNSET].param[P_DICT].name, "dict");
	strcpy(commands_info[C_UNSET].param[P_ALL].name, "all");

	strcpy(commands_info[C_INFO].param[P_VERSION].name, "version");
	strcpy(commands_info[C_INFO].param[P_DICT].name, "dict");
	strcpy(commands_info[C_INFO].param[P_ALL].name, "all");
}

int get_command_id(const char* name){
	if(!strcasecmp(name, "set")){
		return C_SET;
	} else if(!strcasecmp(name, "unset")){
		return C_UNSET;
	} else if(!strcasecmp(name, "load")){
		return C_LOAD;
	} else if(!strcasecmp(name, "exit")){
		return C_EXIT;
	} else if(!strcasecmp(name, "drop")){
		return C_DROP;
	} else if(!strcasecmp(name, "find")){
		return C_FIND;
	} else if(!strcasecmp(name, "info")){
		return C_INFO;
	} else if(!strcasecmp(name, "help")){
		return C_HELP;
	} else if(!strcasecmp(name, "add")){
		return C_ADD;
	} else if(!strcasecmp(name, "del")){
		return C_DEL;
	} else if(!strcasecmp(name, "keep")){
		return C_KEEP;
	} else if(!strcasecmp(name, "unkeep")){
		return C_UNKEEP;
	}
	return -1;
}

void welcome(){
	printf("Welcome to HedDict.\nDictionary version: ");
	printf(VERSION);
	printf("\nCopyright(c) 2015. All rights reserved.\n");
	printf("Written by DatTV, AnhTT and DucMH - HUST\n");
	printf("Type 'help' for help. Type 'Ctrl-C' to exit. Have fun!!\n");
}
