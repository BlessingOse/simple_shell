#include "ssh.h"

/**
 * CDir_loc_HM - changes to home directory
 *
 * @datash: data relevant (environ)
 * Return: no return
 */

void CDir_loc_HM(data_shell *datash)
{
	char *p_pwd, *home;
	char pwd[PATH_MAX];

	getcwd(pwd, sizeof(pwd));
	p_pwd = _strdup(pwd);

	home = _getenv("HOME", datash->_environ);

	if (home == NULL)
	{
		PREP_env("OLDPWD", p_pwd, datash);
		free(p_pwd);
		return;
	}

	if (chdir(home) == -1)
	{
		fetch_ERR(datash, 2);
		free(p_pwd);
		return;
	}

	PREP_env("OLDPWD", p_pwd, datash);
	PREP_env("PWD", home, datash);
	free(p_pwd);
	datash->status = 0;
}

/**
 * CDir_SH - changes current directory
 *
 * @datash: data relevant
 * Return: 1 on success
 */

int CDir_SH(data_shell *datash)
{
	char *dir;
	int ishome, ishome2, isddash;

	dir = datash->args[1];

	if (dir != NULL)
	{
		ishome = _strcmp("$HOME", dir);
		ishome2 = _strcmp("~", dir);
		isddash = _strcmp("--", dir);
	}

	if (dir == NULL || !ishome || !ishome2 || !isddash)
	{
		CDir_loc_HM(datash);
		return (1);
	}

	if (_strcmp("-", dir) == 0)
	{
		CDir_last(datash);
		return (1);
	}

	if (_strcmp(".", dir) == 0 || _strcmp("..", dir) == 0)
	{
		CDir_ftp(datash);
		return (1);
	}

	CDir_loc(datash);

	return (1);
	}

	CDir_loc(datash);

	return (1);
}

/**
 * dup_Char - counts the repetitions of a char
 *
 * @input: input string
 * @i: index
 * Return: repetitions
 */

int dup_Char(char *input, int i)
{
	if (*(input - 1) == *input)
		return (dup_Char(input - 1, i + 1));

	return (i);
}

/**
 * basic_Char - finds index of the first char
 *
 * @input: input string
 * @i: index
 * Return: 1 if there is an error. 0 in other case.
 */

int basic_Char(char *input, int *i)
{
	for (*i = 0; input[*i]; *i += 1)
	{
		if (input[*i] == ' ' || input[*i] == '\t')
			continue;

		if (input[*i] == ';' || input[*i] == '|' || input[*i] == '&')
		return (-1);

		break;
	}

	return (0);
}
