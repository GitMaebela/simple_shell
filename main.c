#include "main.h"

/**
* free_data - frees data structure
*
* @datash: data structure
* Return: none
*/
void free_data(data_shell *datash)
{
	unsigned int i;

	for (i = 0; datash->_environ[i]; i++)
	{
		free(datash->_environ[i]);
	}

	free(datash->_environ);
	free(datash->pid);
}

/**
* set_data - Initialize data structure
*
* @datash: data structure
* @av: argument vector
* Return: none
*/
void set_data(data_shell *datash, char **av)
{
	unsigned int i;

	datash->av = av;
	datash->input = NULL;
	datash->args = NULL;
	datash->status = 0;
	datash->counter = 1;

	for (i = 0; environ[i]; i++)
		;

	datash->_environ = malloc(sizeof(char *) * (i + 1));

	for (i = 0; environ[i]; i++)
	{
		datash->_environ[i] = _strdup(environ[i]);
	}

	datash->_environ[i] = NULL;
	datash->pid = aux_itoa(getpid());
}

/**
* main - Entry point
*
* @argc: argument count
* @argv: argument vector
*
* Return: 0 on success.
*/


int main(int argc, char** argv) 
{
    process_args(argc, argv);
    return 0;
}
