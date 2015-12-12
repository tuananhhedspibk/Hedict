#ifndef __BTREE_FUNC__
#define __BTREE_FUNC__

#include <signal.h>
#include "command.h"
#include "tokenizer.h"
#include "error.h"
#include "btree/btree.h"
#include "dict.h"

#define MAX_LENGTH 20
#define MAX 100
#define LENGTH 10
#define SOUNDEX_LENGTH 4
#define MAXNAMELEN 50
#define MAXLINELEN 500

int FullFlag;

char *soundex(char *str);
int add_Word(int d_id);           
void add_soundex(int d_id, char* word);
void Support_string(char *str);
void Solution_Soundex(int d_id, char* pattern);
void Delete_Space(char *str);

#endif