#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include "rl.h"
#include "tokenizer.h"
#include "dict.h"
#include "command.h"
#include "btree_func.h"

void init_rl(){
	init_commands_info();
	init_list_dict();
	rl_attempted_completion_function = main_completion_rl;
}

char** main_completion_rl(const char* str, int start, int end){
	char* cmdline = dupstr(rl_line_buffer);
	del_space(cmdline);
	int space_flag;
	if(is_end_space(rl_line_buffer)){
		space_flag = FLAG_ON;
	} else {
		space_flag = FLAG_OFF;
	}
	int num_words = get_num_words(cmdline);
	if(num_words<1 || start < get_position(rl_line_buffer, 1)){
		return rl_completion_matches(str, command_generator);
	} else {
		int cmdid = get_command_id(get_words(cmdline, 0, 1)); 
		switch(cmdid){
			case C_SET:
				return rl_completion_matches(get_words(cmdline, 1, -1), param_set_generator);
			case C_UNSET:
				return rl_completion_matches(get_words(cmdline, 1, -1), param_unset_generator);
			case C_INFO:
				return rl_completion_matches(get_words(cmdline, 1, -1), param_info_generator);
			case C_ADD:
				if(start < get_position(rl_line_buffer, 2)+1){
					return rl_completion_matches(get_words(cmdline, 1, 1), param_dict_generator);
				}
				return NULL;
			case C_LOAD:
				return NULL;
			case C_HELP:
				rl_bind_key('\t', rl_abort);
				return NULL;
			case C_DROP:
				if(start < get_position(rl_line_buffer, 2)){
					return rl_completion_matches(get_words(cmdline, 1, 1), param_dict_generator);
				}
				return NULL;
			case C_KEEP:
				if(start < get_position(rl_line_buffer, 2)){
					return rl_completion_matches(get_words(cmdline, 1, 1), param_dict_generator);
				}
				return NULL;
			case C_UNKEEP:
				if(start < get_position(rl_line_buffer, 2)){
					return rl_completion_matches(get_words(cmdline, 1, 1), param_dict_generator);
				}
				return NULL;
			case C_FIND:
				if(start < get_position(rl_line_buffer, 2)){
					return rl_completion_matches(get_words(cmdline, 1, 1), param_dict_generator);
				}
				return NULL;
			case C_DEL:
				if(start < get_position(rl_line_buffer, 2)){
					return rl_completion_matches(get_words(cmdline, 1, 1), param_dict_generator);
				}
				return NULL;
		}
	} 
	return NULL;
}

char* command_generator(const char* str, int state){
	static int len, index;
	if(!state){
		index = 0;
		len = strlen(str);
	}
	while(index < NUM_COMMANDS){
		char* entry = commands_info[index++].name; 
		if(!strncasecmp(str, entry, len)){
			return dupstr(entry);
		}
	}
	return NULL;
}

char* param_dict_generator(const char* text, int state){
	static int len, dict_index, num_words, dictid;
	if(!state){
		num_words = get_num_words(text);
		dict_index = 0;
		len = strlen(text);
	}
	while(dict_index < MAX_DICT){
		if(list_dict[dict_index].d_id >= 0){
			if(!strncmp(text, list_dict[dict_index].d_name, len)){
				return(dupstr(list_dict[dict_index++].d_name));
			}
		}
		dict_index++;
	}
	return NULL;
}

char* param_set_generator(const char* str, int state){
	static int len, index, dict_index;
	static char* cmdname;
	if(!state){
		index = 0;
		dict_index = 0;
		len = strlen(str);
	}
	while(index < NUM_PARAMS){
		char* entry = commands_info[C_SET].param[index].name; 
		int entry_len = strlen(entry);
		if(entry_len && (!strncasecmp(str, entry, len))){
			if(index == P_DICT){
				while(dict_index < MAX_DICT){
					if(list_dict[dict_index].d_id >= 0){
						char* dict_name = list_dict[dict_index++].d_name;
						char* dup_entry = dupstr(entry);
						strcat(dup_entry, dict_name);
						return dup_entry;
					}
					dict_index++;
				}
				index++;
				if(!num_dict){
					return dupstr(entry);
				}
			} else {
				index++;
				return dupstr(entry);
			}
		} else {
			index++;		
		}
	}
	return NULL;
}

char* param_unset_generator(const char* str, int state){
	static int len, index;
	static char* cmdname;
	if(!state){
		index = 0;
		len = strlen(str);
	}
	while(index < NUM_PARAMS){
		char* entry = commands_info[C_UNSET].param[index++].name; 
		if(strlen(entry) && (!strncasecmp(str, entry, len) || !len)){
			return dupstr(entry);
		}
	}
	return NULL;
}

char* param_info_generator(const char* str, int state){
	static int len, index;
	static char* cmdname;
	if(!state){
		index = 0;
		len = strlen(str);
	}
	while(index < NUM_PARAMS){
		char* entry = commands_info[C_INFO].param[index++].name; 
		if(strlen(entry) && (!strncasecmp(str, entry, len) || !len)){
			return dupstr(entry);
		}
	}
	return NULL;
}