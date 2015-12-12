#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/signal.h>
#include "dict.h"
#include "btree_func.h"
#include "command.h"
#include "rl.h"
#include "tokenizer.h"
#include <signal.h>
#include <stdlib.h>

void sig_handler(int sigid){
    if(sigid == SIGINT || sigid == SIGTERM || sigid == SIGSTOP){
        c_exit();
    }
}

int main(int argc, char** argv){
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);
	if(btinit()){
		printf("Initialization failed!!!\n");
		exit(0);
	}
	init_rl();
	welcome();
	while(1){
		rl_bind_key('\t', rl_complete);
		char* cmdline = readline(">> ");
		char* cmdname = get_words(cmdline, 0, 1);
		if(strlen(cmdname)){
			add_history(cmdline);
			COMMAND *cmd = get_command(cmdline);
			if(cmd != NULL){
				exec_command(cmd);
				c_error(cmd, cmdline);
				free_command(cmd);
			} else {
				c_error(cmd, cmdline);
			}
		}
		free(cmdname);
	}
}