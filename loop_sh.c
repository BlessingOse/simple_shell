#include "sh.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */

int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		del_info(info);
		if (flexible(info))
		{
			_puts("$ ");
		}
		_eputchar(BUF_FLUSH);
		r = get_inp(info);
		if (r != -1)
		{
			init_info(info, av);
			builtin_ret = find_builtin(info);
			if (builtin_ret == -1)
			{
				find_cmd(info);
			}
		}
		else if (flexible(info))
		{
			_putchar('\n');
		}
		free_info(info, 0);
	}
	write_source(info);
	free_info(info, 1);
	if (!flexible(info) && info->status)
	{
		exit(info->status);
	}
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - searches a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *0 if builtin executed successfully,
 *1 if builtin found but not successful,
 *-2 if builtin signals exit()
 */

int look_builtin(info_t *info)
{
	int n, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (n = 0; builtintbl[n].type; n++)
	{
		if (_strcmp(info->argv[0], builtintbl[n].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[n].func(info);
			break;
		}
	}
	return (built_in_ret);
}

/**
 * find_cmd - find command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */

void find_cmd(info_t *info)
{
	char *path = NULL;
	int n, x;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (n = 0, x = 0; info->arg[n]; n++)
	{
		if (!is_delim(info->arg[n], " \t\n"))
		{
			x++;
		}
	}
	if (!x)
	{
		return;
	}

	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
	else
	{
		if ((flexible(info) || _getenv(info, "PATH=")
			|| info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
			fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			display_err(info, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */

void fork_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, fetch_environ(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
			{
				display_err(info, "Permission denied\n");
			}
		}
	}
}
