#ifndef _MAIN_
#define _MAIN_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <limits.h>



#define MAX_ALIASES 100
#define MAX_ALIAS_NAME 100
#define MAX_ALIAS_VALUE 100

#define MAX_CMD_LENGTH 100
#define BUFFER_SIZE 256

/**
 * struct data - struct that contains all relevant data on runtime
 * @av: argument vector
 * @input: command line written by the user
 * @args: tokens of the command line
 * @status: last status of the shell
 * @counter: lines counter
 * @_environ: environment variable
 * @pid: process ID of the shell
 */
typedef struct data
{
	char **av;
	char *input;
	char **args;
	int status;
	int counter;
	char **_environ;
	char *pid;
} data_shell;

void free_data(data_shell *datash)
void set_data(data_shell *datash, char **av)
int is_cdir(char *path, int *i)
char *_which(char *cmd, char **_environ)
int is_executable(data_shell *datash)
int check_executable(char *command, data_shell *datash)
int execute_command(data_shell *datash)
int exit_shell(data_shell *datash)
int is_valid_number(char *str, unsigned int *status)
int setenv(const char *name, const char *value, int overwrite) 
int unsetenv(const char *name) 
void promptc()
void printenv(int argc, char *argv[], char *envp[])
char *my_getline(void) 
int cd(char *path) 
void print_alias(struct alias a) 
void print_aliases() 
int find_alias_index(char* name) 
void print_alias_names(int argc, char** argv) 
void set_alias(char* name, char* value) 
void process_args(int argc, char** argv) 

#endif
