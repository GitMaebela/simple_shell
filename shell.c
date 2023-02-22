#include "main.h"

/**
 * is_cdir - checks if ":" is in the current directory.
 * @path: pointer to a string that contains directories separated by colons.
 * @i: pointer to an index.
 * Return: 1 if the path is searchable in the current directory, 0 otherwise.
 */
int is_cdir(char *path, int *i)
{
	if (path[*i] == ':')
		return (1);

	while (path[*i] != ':' && path[*i] != '\0')
	{
		*i += 1;
	}

	if (path[*i] != '\0')
		*i += 1;

	return (0);
}

/**
 * _which - locates a command.
 * @cmd: command name.
 * @_environ: environment variable.
 * Return: location of the command.
 */
char *_which(char *cmd, char **_environ)
{
	char *path, *dir, *end_path;
	int len_dir, len_cmd, i;
	struct stat st;

	path = _getenv("PATH", _environ);
	if (!path)
	{
		if (cmd[0] == '/')
			if (stat(cmd, &st) == 0)
				return (cmd);
		return (NULL);
	}

	len_cmd = _strlen(cmd);
	i = 0;
	while (*path != '\0')
	{
		end_path = path;
		while (*end_path != '\0' && *end_path != ':')
			end_path++;

		len_dir = end_path - path;
		dir = malloc(len_dir + len_cmd + 2);
		_strncpy(dir, path, len_dir);
		dir[len_dir] = '/';
		_strcpy(dir + len_dir + 1, cmd);

		if (stat(dir, &st) == 0)
		{
			free(path);
			return (dir);
		}

		free(dir);
		if (*end_path == ':')
			path = end_path + 1;
		else
			path = end_path;
	}

	free(path);
	return (NULL);
}

/**
 * is_executable - determines if a file is an executable.
 * @datash: data structure.
 * Return: 0 if it is not an executable, other number if it is.
 */
int is_executable(data_shell *datash)
{
	struct stat st;
	int i = 0;
	char *input = datash->args[0];

	while (input[i])
	{
		if (input[i] == '/')
		{
			if (input[i + 1] == '.')
				return (0);
			i++;
			break;
		}
		i++;
	}
	if (i == 0)
		return (0);
	if (stat(input + i, &st) == 0 && S_ISREG(st.st_mode) && st.st_mode & S_IXUSR)
		return (i);
	get_error(datash, 127);
	return (-1);
}


/**
 * check_executable - checks if the command is executable
 *
 * @command: command to check
 * @datash: data relevant (args and input)
 * Return: 1 if executable, 0 if found and not executable, -1 if not found
 */
int check_executable(char *command, data_shell *datash)
{
	struct stat st;

	if (_strchr(command, '/') != NULL)
	{
		if (stat(command, &st) == 0)
			return (st.st_mode & S_IXUSR);
	}
	else
	{
		char *path = _which(command, datash->_environ);
		if (path != NULL)
		{
			int is_executable = stat(path, &st) == 0 && st.st_mode & S_IXUSR;
			free(path);
			return is_executable;
		}
	}

	get_error(datash, 127);
	return -1;
}

/**
 * execute_command - executes command lines
 *
 * @datash: data relevant (args and input)
 * Return: 1 on success, 0 on failure
 */
int execute_command(data_shell *datash)
{
	char *command = datash->args[0];
	int is_executable = check_executable(command, datash);

	if (is_executable == -1)
		return 0;
	if (is_executable == 0)
	{
		get_error(datash, 126);
		return 0;
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		perror(datash->av[0]);
		return 0;
	}
	else if (pid == 0)
	{
		execve(command, datash->args, datash->_environ);
		perror(datash->av[0]);
		exit(1);
	}
	else
	{
		int status;
		waitpid(pid, &status, WUNTRACED);
		datash->status = WIFEXITED(status) ? WEXITSTATUS(status) : 128 + WTERMSIG(status);
		return 1;
	}
}
