#include "main.h"



/**
* promptc
*/
void promptc()
{
    char cmd[MAX_CMD_LENGTH];

    while (1) 
    {
        printf("simple_shell$ ");
        fflush(stdout); // flush the output stream to ensure prompt is displayed

        if (fgets(cmd, MAX_CMD_LENGTH, stdin) == NULL) 
        {
            printf("\n");
            break; // exit loop on EOF (Ctrl+D)
        }

        // remove trailing newline from command string
        size_t cmd_len = strlen(cmd);
        if (cmd[cmd_len - 1] == '\n')
        {
            cmd[cmd_len - 1] = '\0';
        }

        char *token = strtok(cmd, ";");
        int status = 0;

        while (token != NULL) 
        {
            pid_t pid = fork();
            if (pid == -1) 
            {
                perror("fork");
                exit(EXIT_FAILURE);
            } 
            else if (pid == 0) 
            {
                // child process
                if (execlp(token, token, NULL) == -1) 
                {
                    perror("execlp");
                    exit(EXIT_FAILURE);
                }
            } 
            else 
            {
                // parent process
                int child_status;
                if (waitpid(pid, &child_status, 0) == -1) 
                {
                    perror("waitpid");
                    exit(EXIT_FAILURE);
                }

                if (WIFEXITED(child_status)) 
                {
                    int child_exit_status = WEXITSTATUS(child_status);
                    if (token[strlen(token) - 1] == '&') 
                    {
                        // handle &&
                        if (child_exit_status != 0) 
                        {
                            status = child_exit_status;
                            break;
                        }
                    } 
                    else 
                    {
                        // handle ||
                        if (child_exit_status == 0) 
                        {
                            status = child_exit_status;
                            break;
                        }
                    }
                }
            }
            token = strtok(NULL, ";");
        }

        printf("Command exited with status %d\n", status);
    }
}



/**
* printenv
* @argc:
* @argv:
* @envp:
*/
void printenv(int argc, char *argv[], char *envp[])
{
    int i;

    for (i = 0; envp[i] != NULL; i++) 
    {
        // Print the environment variable
        printf("%s\n", envp[i]);
    }
}



/**
* print_alias - Prints the alias
* @a: alias
*/
char *my_getline(void) 
{
    static char buffer[BUFFER_SIZE];
    static int pos = 0;
    static int size = 0;
    char *line = NULL;
    int i = 0;
    
    while (1) 
    {
        if (pos >= size) 
        {
            size = read(0, buffer, BUFFER_SIZE);
            if (size <= 0) 
            {
                return NULL;
            }
            pos = 0;
        }
        
        char c = buffer[pos++];
        if (c == '\n') 
        {
            break;
        }
        
        if (i == 0) 
        {
            line = (char*) malloc(BUFFER_SIZE);
        } 
        else if (i % BUFFER_SIZE == 0) 
        {
            line = (char*) realloc(line, i + BUFFER_SIZE);
        }
        
        line[i++] = c;
    }
    
    if (i > 0) 
    {
        line[i] = '\0';
    }
    
    return line;
}


// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>

#define MAX_PATH_LENGTH 1024



/**
* print_alias - Prints the alias
* @a: alias
*/
int cd(char *path) 
{
    char current_dir[MAX_PATH_LENGTH];

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) 
    {
        perror("getcwd() error");
        return -1;
    }

    if (path == NULL) 
    {
        path = getenv("HOME");
    } 
    else if (strcmp(path, "-") == 0) 
    {
        path = getenv("OLDPWD");
        printf("%s\n", path);
    }

    if (chdir(path) != 0) 
    {
        perror("chdir() error");
        return -1;
    }

    if (setenv("OLDPWD", current_dir, 1) != 0) 
    {
        perror("setenv() error");
        return -1;
    }

    if (getcwd(current_dir, sizeof(current_dir)) == NULL) 
    {
        perror("getcwd() error");
        return -1;
    }
    if (setenv("PWD", current_dir, 1) != 0) 
    {
        perror("setenv() error");
        return -1;
    }

    return 0;
}



/**
* print_alias - Prints the alias
* @a: alias
*/
int main(int argc, char *argv[]) 
{
    char *path = NULL;
    if (argc > 2) 
    {
        fprintf(stderr, "Usage: %s [DIRECTORY]\n", argv[0]);
        return 1;
    } 
    else if (argc == 2) 
    {
        path = argv[1];
    }

    return cd(path);
}
