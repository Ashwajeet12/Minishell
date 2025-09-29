#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
extern struct list* head;

void scan_input();
char *get_command(char *input_string,char*);
void extract_external_command(char arr[][20]);
void copy_change(char *prompt, char *input_string);
void n_pipe(int,char* argv[]); 
int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_External_commands(char *input_string,char *argv[]);
void signal_handler(int sig_num);
char* validate_PS1(char*);
void signal_handler(int signum);
void extract_external_commands(char **external_commands);
void execute_internal_commands(char *input_string);
void insert_last(struct list**, int ,char*);
void print_list(struct list*); 
int get_last_stoped_process_id(struct list*);
int delete_last(struct list**);
#endif