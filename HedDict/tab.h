#include <termios.h>
#include <stdio.h>
#include "btree/btree.h"

void tty_atexit(void);                                                  // Hàm bên phục vụ dấu tab
int tty_reset(void);
void tty_raw(void);
int screenio(BTA*,char*);
void fatal(char *mess);
int tab_complt(BTA *,char*);                      //Chuc Nang Tab
