#include "ssh.h"

/**
 * fetch_builtin - builtin that pais the command in the arg
 * @cmd: command
 * Return: function pointer of the builtin command
 */

int (*fetch_builtin(char *cmd))(data_shell *)
{
	builtin_t builtin[] = {
		{ "env", _env },
		{ "exit", quit_SH },
		{ "setenv", _setenv },
		{ "unsetenv", _unsetenv },
		{ "cd", CDir_SH },
		{ "help", fetch_asst },
		{ NULL, NULL }
	};
	int i;

	for (i = 0; builtin[i].name; i++)
	{
		if (_strcmp(builtin[i].name, cmd) == 0)
			break;
	}
	return (builtin[i].f);
}

/**
 * fetch_ERR - calls the error according the builtin, syntax or permission
 * @datash: data structure that contains arguments
 * @eval: error value
 * Return: error
 */

int fetch_ERR(data_shell *datash, int eval)
{
	char *error;

	switch (eval)
	{
		case -1:
		error = err_env_msg(datash);
		break;
	case 126:
		error = err_RD_126(datash);
		break;
	case 127:
		error = err_missing(datash);
		break;
	case 2:
		if (_strcmp("exit", datash->args[0]) == 0)
			error = err_EXT_sh(datash);
		else if (_strcmp("cd", datash->args[0]) == 0)
			error = err_getCdir(datash);
		break;
	}
	if (error)
	{
		write(STDERR_FILENO, error, _strlen(error));
		free(error);
	}
	datash->status = eval;
	return (eval);
}

/**
 * fetch_asst - function that retrieves help messages according builtin
 * @datash: data structure (args and input)
 * Return: Return 0
*/

int fetch_asst(data_shell *datash)
{
	if (datash->args[1] == 0)
		aut_asst_Gen();
	else if (_strcmp(datash->args[1], "setenv") == 0)
		aut_asst_setenv();
	else if (_strcmp(datash->args[1], "env") == 0)
		aut_asst_env();
	else if (_strcmp(datash->args[1], "unsetenv") == 0)
		aut_asst_unsetenv();
	else if (_strcmp(datash->args[1], "help") == 0)
		aut_asst();
	else if (_strcmp(datash->args[1], "exit") == 0)
		aut_asst_Ext();
	else if (_strcmp(datash->args[1], "cd") == 0)
		aut_asst_CDir();
	else if (_strcmp(datash->args[1], "alias") == 0)
		aut_asst_alias();
	else
		write(STDERR_FILENO, datash->args[0],
				_strlen(datash->args[0]));

	datash->status = 0;
	return (1);
}
