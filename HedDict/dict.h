#ifndef __DICT__
#define __DICT__

#include "error.h"
#include "btree/btree.h"

#define MAX_DICT 10

typedef struct DICT{
	int d_id;
	char* d_name;
	char* d_path;
	BTA *d_btact;
	BTA *d_btact_soundex;
	int keep_flag;
}DICT;

DICT	list_dict[MAX_DICT];									
int 	default_dict;
int 	current_dict;
int 	num_dict;
			
void 	init_list_dict();
int 	set_defaut_dict(int flag, const char* d_name);
int 	get_dict_by_path(const char* d_path);
int 	get_dict_by_name(const char* d_name);
void 	free_dict(int d_id);
int 	drop(const char* d_name);
int 	keep(const char* d_name);
int 	set_default_dict(int flag, const char* d_name);

int 	load(const char* d_name, const char* d_path);	
int 	find(int d_id, const char* pattern);
int 	del(int d_id, const char* pattern);		
int 	add(int d_id, const char* d_pattern);				

#endif