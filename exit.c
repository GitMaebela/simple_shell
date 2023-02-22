/**
 * exit_shell - Exits the shell
 * @datash: Pointer to struct of shell data
 * Return: 0 on success, 1 on failure
 */
int exit_shell(data_shell *datash)
{
	unsigned int status = 0;

	if (datash->args[1])
	{
		if (!is_valid_number(datash->args[1], &status))
		{
			get_error(datash, 2);
			datash->status = 2;
			return (1);
		}
	}

	datash->status = status % 256;
	return (0);
}

/**
 * is_valid_number - Checks if the string is a valid number
 * @str: Pointer to string to check
 * @status: Pointer to store the converted number
 * Return: 1 if string is a valid number, 0 otherwise
 */
int is_valid_number(char *str, unsigned int *status)
{
	unsigned int num = 0;

	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		num = (num * 10) + (*str++ - '0');
		if (num > INT_MAX)
			return (0);
	}

	*status = num;
	return (1);
}
