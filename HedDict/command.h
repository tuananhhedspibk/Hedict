#ifndef __COMMAND__
#define __COMMAND__

#include <stdio.h>
#include <stdlib.h>
#include "dict.h"
#include "error.h"
#include "btree/btree.h"

#define FLAG_ON			1
#define FLAG_OFF		0

#define C_DROP			0
#define C_SET 			1
#define C_UNSET 		2
#define C_HELP  		3
#define C_FIND 			4
#define C_ADD			5
#define C_DEL			6
#define C_INFO  		7
#define C_LOAD			8
#define C_KEEP			9
#define C_UNKEEP		10
#define C_EXIT			11

#define P_PATTERN		0
#define P_DICT			1
#define P_REGEX			2
#define P_VERSION		3
#define P_ALL			4
#define P_PATH		 	5
#define P_TEST			6

#define NUM_PARAMS  	7
#define NUM_COMMANDS 	12
#define VERSION			"HedDict 1.0"
#define MAX_VALUE_LENGTH 300
#define MAX_NAME_LENGTH 50
#define MAX_LINE_LENGTH 1500
#define VALID_SEARCH_LENGTH 2
#define COMMAND_FLAG 	1
#define PARAM_FLAG 		2
typedef struct PARAM{
	char* name;
	int id;
	int flag;
	char* value;
}PARAM;

typedef struct COMMAND{
	char* name;
	int id;
	int num_param;
	PARAM* param;
}COMMAND;

COMMAND *commands_info;

int test_mode;

COMMAND* get_command(const char* cmdline);		
void init_commands_info();
int get_command_id(const char* name);
int exec_command(COMMAND *cmd);						
int fill_command(COMMAND *cmd, const char* cmdline);		
void free_command(COMMAND *cmd);					
void c_error(COMMAND *cmd, const char* cmdline);		//in loi dua theo bien toan cuc cmd_error vaf cmd, cmdline
int c_set(COMMAND *cmd);		// thuc hien set command
int c_drop(COMMAND *cmd);		// thuc hien drop command 
int c_unset(COMMAND *cmd);		//..
int c_help();		
int c_find(COMMAND *cmd);		
int c_add(COMMAND *cmd);		
int c_del(COMMAND *cmd);		
int c_info(COMMAND *cmd);					
int c_keep(COMMAND *cmd);
void welcome();

#endif
