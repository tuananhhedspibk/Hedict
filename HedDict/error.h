#ifndef __ERROR_H__
#define __ERROR_H__

#define E_NO_ERROR					0
#define E_MEMORY_ALLOCATE 			1
#define E_INVALID_COMMAND_NAME 		2
#define E_INVALID_PARAM				3
#define E_DICT_NOT_FOUND			4
#define E_INVALID_PATH				5
#define E_PATTERN_NOT_FOUND			6
#define E_PERMISSION				7
#define E_DICT_NAME_USED			8
#define E_PATH_USED					9
#define E_PATTERN_EXISTED			10
#define E_LOAD_ERROR				11
#define E_FULL_DICT					12

int cmd_errno;

#endif
