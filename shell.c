#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include "main.h"


/**
*
* Function: exit_builtin
* This function implements the built-in command "exit".
* @args: A list of arguments passed to the "exit" command.
* returns: Nothing. Exits the shell.
*/
void exit_builtin(char **args)
{
	int status = 0;

	// if an argument is passed, use it as the exit status
	if (args[1])
		status = atoi(args[1]);

	// exit the shell with the specified status
	exit(status);

}


/**
*
* Function: env_builtin
* This function implements the built-in command "env".
* @envp: An array of environment variables.
* returns: Nothing. Prints the current environment variables.
*/
void env_builtin(char **envp)
{
	int i = 0;

	// loop through the envp array and print each environment variable
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}


/**
*
* Function: setenv_builtin
* This function implements the built-in command "setenv".
* @args: An array of arguments, where args[0] is "setenv", args[1] is the name of the environment variable, and args[2] is its value.
* returns: Nothing. Sets or updates an environment variable.
*/
void setenv_builtin(char **args)
{
	//check if the required arguments are present
	if (args[1] == NULL || args[2] == NULL)
	{
		// print an error message if the required arguments are missing
		fprintf(stderr, "setenv: missing operand\n");
		return;
	}

	// call the setenv function to set or update the environment variable
	if (setenv(args[1], args[2], 1) == -1)
	{
		// print an error message if there was a problem with setenv
		perror("setenv");
	}
}


/**
*
* unsetenv_builtin - function to unset environment variable
* @args: the input command
* Description: This function unset an environment variable passed
* as argument to the unsetenv built-in command. If the argument is
* missing, the function will print an error message on stderr.
*/
void unsetenv_builtin(char **args)
{
	if (args[1] == NULL)
	{
		fprintf(stderr, "unsetenv: missing operand\n");
		return;
	}
	if (unsetenv(args[1]) == -1)
	{
		perror("unsetenv");
	}
}


/**
*
* This function implements the cd builtin command. It changes the current working directory of the process.
*
* If no argument is given, the current working directory is changed to the home directory, specified by the
*
* value of the HOME environment variable. If the argument is -, the current working directory is changed to
*
* the value of the OLDPWD environment variable. If the argument is a valid directory, the current working
*
* directory is changed to the specified directory. If the change directory operation fails, an error message is printed to the standard error.
*
* @param args - An array of strings containing the arguments passed to the cd command
*/
void cd_builtin(char **args)
{
	char *dir;

	if (args[1] == NULL)
		dir = getenv("HOME"); // if no argument is given, change to the home directory
	else if (args[1][0] == '-')
		dir = getenv("OLDPWD"); // if argument is '-', change to the previous working directory
	else
		dir = args[1]; // otherwise change to the specified directory

	if (chdir(dir) == -1)
	{
		perror("cd");
		return;
	}

	setenv("OLDPWD", getenv("PWD"), 1);
	setenv("PWD", getcwd(NULL, 0), 1);
}


/**
*
* execute_command_sequence - execute a sequence of commands separated by ';'
* @command_sequence: the string containing the sequence of commands
* This function tokenizes the command_sequence string using ';' as the delimiter and
* executes each command by calling the execute_command function.
*/
void execute_command_sequence(char *command_sequence)
{
	char *token = strtok(command_sequence, ";");

	while (token)
	{
		execute_command(token);
		token = strtok(NULL, ";");
	}

	char *cmd = strtok(line, ";");

	while (cmd != NULL)
	{
		execute_command(cmd);
		cmd = strtok(NULL, ";");
	}





	char *cmd = strtok(line, "&&");

	while (cmd != NULL)
	{
		int status = execute_command(cmd);

		if (status != 0)
		{
			break;
		}

		cmd = strtok(NULL, "&&");

	}


	char *cmd = strtok(line, "||");

	while (cmd != NULL)
	{
		int status = execute_command(cmd);

		if (status == 0)
		{
			break;
		}

		cmd = strtok(NULL, "||");
	}
}


// Structure to hold an alias name and value
typedef struct alias
{
	char *name; // The name of the alias
	char *value; // The value of the alias
	struct alias *next; // Pointer to the next alias in the linked list
} alias_t;

// Linked list of aliases, head is the first element in the list
alias_t *head = NULL;

// Function to print all the defined aliases
void print_alias()
{
	// Pointer to traverse the linked list of aliases
	alias_t *ptr = head;

	while (ptr != NULL)
	{
		// Print the name and value of the current alias
		printf("%s='%s'\n", ptr->name, ptr->value);
		ptr = ptr->next;
	}
}



/**
* set_alias -
* @name
* @value
*/
void set_alias(char *name, char *value) {
	// pointer to traverse the linked list of aliases
	alias_t *ptr = head;

	// loop through the linked list to check if an alias with the same name already exists
	while (ptr != NULL) {
		// if an alias with the same name is found, update its value
		if (strcmp(ptr->name, name) == 0) {
			// free the previous value
			free(ptr->value);
			// update the value with the new one
			ptr->value = strdup(value);
			return;
		}
		ptr = ptr->next;
	}

	// if no alias with the same name is found, create a new one
	alias_t *new = (alias_t*) malloc(sizeof(alias_t));
	new->name = strdup(name);
	new->value = strdup(value);
	new->next = head;
	head = new;
}



/**
*
* get_alias - function that returns the value of the alias with the given name.
* @name: name of the alias to look up.
* This function iterates through the linked list of aliases, comparing each name
* to the given name. If a match is found, the value of the alias is returned.
* Otherwise, the function returns NULL.
*/
char *get_alias(char *name)
{
	alias_t *ptr = head;

	while (ptr != NULL)
	{
		if (strcmp(ptr->name, name) == 0)
		{
			return ptr->value;
		}

		ptr = ptr->next;
	}

	return NULL;
}


/**
*
* expand_vars - expand environment variables in a given line
* @line: pointer to the input line
* This function takes a line as input and replaces any environment variable in the
* line with its value. If the variable is not found, it is not replaced.
* The function returns a newly allocated string containing the expanded line.
* Return: pointer to the expanded line.
*/
char *expand_vars(char *line)
{
	int len = strlen(line);
	char *new = (char*) malloc(len + 1); // allocate memory for the expanded line
	int j = 0;

	for (int i = 0; i < len; i++)
	{
		// check if the current character is a $
		if (line[i] == '$')
		{
			char *var = &line[i + 1]; // get the environment variable
			char *value = getenv(var); // get the value of the environment variable

			// if the variable is found
			if (value != NULL)
			{
				strcpy(&new[j], value); // replace the variable with its value
				j += strlen(value);
			}
		}
		else
		{
			new[j++] = line[i]; // copy the character to the expanded line
		}
	}

	new[j] = '\0'; // null-terminate the expanded line

	return new;
}


// andle the $? variable
// The $? variable holds the exit status of the last executed command. To implement this,
// we can pass the exit status of each executed command to the shell and store it in a variable.
// Here's how the implementation might look like:

// int last_status = 0;

// void execute_command(char *line) {
//   int status = 0;
//   // execute command here
//   last_status


int last_exit_status = 0;

//in the execute_command function
last_exit_status = status;

//in the shell loop
if (strcmp(args[0], "$?") == 0)
{
	printf("%d\n", last_exit_status);
	return;
}
