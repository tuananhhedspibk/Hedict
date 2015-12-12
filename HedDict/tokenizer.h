#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

int del_spaces(char* str);
int get_num_words(const char* str);
char* get_words(const char* str, int pos, int offset);
char* dupstr(const char* str);
int get_position(const char* str, int wordno);
void Repair_string(char *str);

#endif