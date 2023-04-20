#include "shell.h"

/**
 * free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */

void free_info(info_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
		{
			free(info->arg);
		}
		if (info->env)
		{
			free_list(&(info->env));
		}
		if (info->history)
		{
			free_list(&(info->history));
		}
		if (info->alias)
		{
			free_list(&(info->alias));
		}
		ffree(info->environ);
			info->environ = NULL;
		bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
		{
			close(info->readfd);
		}
		_putchar(BUF_FLUSH);
	}
}

/**
 * del_info - clears info_t struct
 * @info: struct address
 */

void del_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * init_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */

void init_info(info_t *info, char **av)
{
	int n = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (n = 0; info->argv && info->argv[n]; n++)
		{
			;
		}
		info->argc = n;

		replace_alias(info);
		replace_vars(info);
	}
}