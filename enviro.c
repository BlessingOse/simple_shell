#include "sh.h"

/**
 * _myenv - current environment
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *
 * Return: Always 0
 */

int _myenv(info_t *info)
{
	print_list_str(info->env);
	return (0);
}

/**
 * _getenv - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */

char *_getenv(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
		{
			return (p);
		}
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenv - Initialize a new environment variable,
 *  or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *
 * Return: Always 0
 */

int _mysetenv(info_t *info)
{
	if (info->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 * constant function prototype.
 *
 * Return: Always 0
 */

int _myunsetenv(info_t *info)
{
	int n;

	if (info->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (n = 1; n <= info->argc; n++)
	{
		_unsetenv(info, info->argv[n]);
	}

	return (0);
}

/**
 * expand_env_list - populates env linked list
 * @info: Structure containing potential arguments.
 *
 * Return: Always 0
 */

int expand_env_list(info_t *info)
{
	list_t *node = NULL;
	size_t n;

	for (n = 0; environ[n]; n++)
		inc_node_end(&node, environ[n], 0);
	info->env = node;
	return (0);
}
