#ifndef __RL__
#define __RL__

#include <readline/readline.h>
#include <readline/history.h>
#include "command.h"
#include "dict.h"

void 	init_rl();
char** 	main_completion_rl(const char* text, int start, int end);
char* 	command_generator(const char* text, int state);
char* 	param_dict_generator(const char* text, int state);
char* 	param_set_generator(const char* text, int state);
char* 	param_unset_generator(const char* text, int state);
char* 	param_info_generator(const char* text, int state);
char* 	param_pattern_generator(const char* str, int state);

#endif