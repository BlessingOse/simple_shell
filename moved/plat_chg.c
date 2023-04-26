#include "ssh.h"

/**
 * undo_string - reverses a string.
 * @s: input string.
 * Return: no return.
 */

void undo_string(char *s)
{
        int count = 0, i, j;
        char *str, temp;

        while (count >= 0)
        {
                if (s[count] == '\0')
                        break;
                count++;
        }
        str = s;

        for (i = 0; i < (count - 1); i++)
        {
                for (j = i + 1; j > 0; j--)
                {
                        temp = *(str + j);
                        *(str + j) = *(str + (j - 1));
                        *(str + (j - 1)) = temp;
                }
        }
}

/**
 * CDir_ftp - changes to the parent directory
 *
 * @datash: data relevant (environ)
 *
 * Return: no return
 */

void CDir_ftp(data_shell *datash)
{
        char pwd[PATH_MAX];
        char *dir, *cp_pwd, *cp_strtok_pwd;

        getcwd(pwd, sizeof(pwd));
        cp_pwd = _strdup(pwd);
        PREP_env("OLDPWD", cp_pwd, datash);
        dir = datash->args[1];
        if (_strcmp(".", dir) == 0)
        {
                PREP_env("PWD", cp_pwd, datash);
                free(cp_pwd);
                return;
        }
        if (_strcmp("/", cp_pwd) == 0)
        {
                free(cp_pwd);
                return;
        }
        cp_strtok_pwd = cp_pwd;
        undo_string(cp_strtok_pwd);
        cp_strtok_pwd = _strtok(cp_strtok_pwd, "/");
        if (cp_strtok_pwd != NULL)
        {
                cp_strtok_pwd = _strtok(NULL, "\0");

                if (cp_strtok_pwd != NULL)
                        undo_string(cp_strtok_pwd);
        }
        if (cp_strtok_pwd != NULL)
        {
                chdir(cp_strtok_pwd);
                PREP_env("PWD", cp_strtok_pwd, datash);
        }
        else
        {
                chdir("/");
                PREP_env("PWD", "/", datash);
        }
        datash->status = 0;
        free(cp_pwd);
}

/**
 * CDir_loc - changes to a directory given
 * by the user
 *
 * @datash: data relevant (directories)
 * Return: no return
 */

void CDir_loc(data_shell *datash)
{
        char pwd[PATH_MAX];
        char *dir, *cp_pwd, *cp_dir;

        getcwd(pwd, sizeof(pwd));

        dir = datash->args[1];
        if (chdir(dir) == -1)
        {
                fetch_ERR(datash, 2);
                return;
        }

        cp_pwd = _strdup(pwd);
        PREP_env("OLDPWD", cp_pwd, datash);

        cp_dir = _strdup(dir);
        PREP_env("PWD", cp_dir, datash);

        free(cp_pwd);
        free(cp_dir);

        datash->status = 0;

        chdir(dir);
}

/**
 * CDir_last - changes to the previous directory
 *
 * @datash: data relevant (environ)
 * Return: no return
 */

void CDir_last(data_shell *datash)
{
        char pwd[PATH_MAX];
        char *p_pwd, *p_oldpwd, *cp_pwd, *cp_oldpwd;

        getcwd(pwd, sizeof(pwd));
        cp_pwd = _strdup(pwd);

        p_oldpwd = _getenv("OLDPWD", datash->_environ);

        if (p_oldpwd == NULL)
                cp_oldpwd = cp_pwd;
        else
                cp_oldpwd = _strdup(p_oldpwd);

        PREP_env("OLDPWD", cp_pwd, datash);

        if (chdir(cp_oldpwd) == -1)
                PREP_env("PWD", cp_pwd, datash);
        else
                PREP_env("PWD", cp_oldpwd, datash);

        p_pwd = _getenv("PWD", datash->_environ);

        write(STDOUT_FILENO, p_pwd, _strlen(p_pwd));
        write(STDOUT_FILENO, "\n", 1);

        free(cp_pwd);
        if (p_oldpwd)
                free(cp_oldpwd);

        datash->status = 0;

        chdir(p_pwd);
}
