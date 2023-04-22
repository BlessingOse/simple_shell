#include "she.h"

/**
 * freeDataFxn - frees data structure
 *
 * @datash: data structure
 * Return: no return
 */

void freeDataFxn(data_shell *datash)
{
	unsigned int n;

	for (n = 0; datash->_environ[n]; n++)
	{
		free(datash->_environ[n]);
	}

	free(datash->_environ);
	free(datash->pid);
}

/**
 * set_data - Initialize data structure
 *
 * @datash: data structure
 * @av: argument vector
 * Return: no return
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
 * @ac: argument count
 * @av: argument vector
 *
 * Return: 0 on success.
 */
int main(int ac, char **av)
{
	data_shell datash;
	(void) ac;

	signal(SIGINT, get_sigint);
	set_data(&datash, av);
	shell_loop(&datash);
	free_data(&datash);
	if (datash.status < 0)
		return (255);
	return (datash.status);
}










/**
 * strcat_cd - function that concatenates the message for cd error
 *
 * @datash: data relevant (directory)
 * @msg: message to print
 * @error: output message
 * @ver_str: counter lines
 * Return: error message
 */
char *strcat_cd(data_shell *datash, char *msg, char *error, char *ver_str)
{
	char *illegal_flag;

	_strcpy(error, datash->av[0]);
	_strcat(error, ": ");
	_strcat(error, ver_str);
	_strcat(error, ": ");
	_strcat(error, datash->args[0]);
	_strcat(error, msg);
	if (datash->args[1][0] == '-')
	{
		illegal_flag = malloc(3);
		illegal_flag[0] = '-';
		illegal_flag[1] = datash->args[1][1];
		illegal_flag[2] = '\0';
		_strcat(error, illegal_flag);
		free(illegal_flag);
	}
	else
	{
		_strcat(error, datash->args[1]);
	}

	_strcat(error, "\n");
	_strcat(error, "\0");
	return (error);
}







/**
 * error_get_cd - error message for cd command in get_cd
 * @datash: data relevant (directory)
 * Return: Error message
 */
char *error_get_cd(data_shell *datash)
{
	int length, len_id;
	char *error, *ver_str, *msg;

	ver_str = aux_itoa(datash->counter);
	if (datash->args[1][0] == '-')
	{
		msg = ": Illegal option ";
		len_id = 2;
	}
	else
	{
		msg = ": can't cd to ";
		len_id = _strlen(datash->args[1]);
	}

	length = _strlen(datash->av[0]) + _strlen(datash->args[0]);
	length += _strlen(ver_str) + _strlen(msg) + len_id + 5;
	error = malloc(sizeof(char) * (length + 1));

	if (error == 0)
	{
		free(ver_str);
		return (NULL);
	}

	error = strcat_cd(datash, msg, error, ver_str);

	free(ver_str);

	return (error);
}

/**
 * error_not_found - generic error message for command not found
 * @datash: data relevant (counter, arguments)
 * Return: Error message
 */
char *error_not_found(data_shell *datash)
{
	int length;
	char *error;
	char *ver_str;

	ver_str = aux_itoa(datash->counter);
	length = _strlen(datash->av[0]) + _strlen(ver_str);
	length += _strlen(datash->args[0]) + 16;
	error = malloc(sizeof(char) * (length + 1));
	if (error == 0)
	{
		free(error);
		free(ver_str);
		return (NULL);
	}
	_strcpy(error, datash->av[0]);
	_strcat(error, ": ");
	_strcat(error, ver_str);
	_strcat(error, ": ");
	_strcat(error, datash->args[0]);
	_strcat(error, ": not found\n");
	_strcat(error, "\0");
	free(ver_str);
	return (error);
}

/**
 * error_exit_shell - generic error message for exit in get_exit
 * @datash: data relevant (counter, arguments)
 *
 * Return: Error message
 */
char *error_exit_shell(data_shell *datash)
{
	int length;
	char *error;
	char *ver_str;

	ver_str = aux_itoa(datash->counter);
	length = _strlen(datash->av[0]) + _strlen(ver_str);
	length += _strlen(datash->args[0]) + _strlen(datash->args[1]) + 23;
	error = malloc(sizeof(char) * (length + 1));
	if (error == 0)
	{
		free(ver_str);
		return (NULL);
	}
	_strcpy(error, datash->av[0]);
	_strcat(error, ": ");
	_strcat(error, ver_str);
	_strcat(error, ": ");
	_strcat(error, datash->args[0]);
	_strcat(error, ": Illegal number: ");
	_strcat(error, datash->args[1]);
	_strcat(error, "\n\0");
	free(ver_str);

	return (error);
}

/**
 * error_env - error message for env in get_env.
 * @datash: data relevant (counter, arguments)
 * Return: error message.
 */
char *error_env(data_shell *datash)
{
	int length;
	char *error;
	char *ver_str;
	char *msg;

	ver_str = aux_itoa(datash->counter);
	msg = ": Unable to add/remove from environment\n";
	length = _strlen(datash->av[0]) + _strlen(ver_str);
	length += _strlen(datash->args[0]) + _strlen(msg) + 4;
	error = malloc(sizeof(char) * (length + 1));
	if (error == 0)
	{
		free(error);
		free(ver_str);
		return (NULL);
	}

	_strcpy(error, datash->av[0]);
	_strcat(error, ": ");
	_strcat(error, ver_str);
	_strcat(error, ": ");
	_strcat(error, datash->args[0]);
	_strcat(error, msg);
	_strcat(error, "\0");
	free(ver_str);

	return (error);
}
/**
 * error_path_126 - error message for path and failure denied permission.
 * @datash: data relevant (counter, arguments).
 *
 * Return: The error string.
 */
char *error_path_126(data_shell *datash)
{
	int length;
	char *ver_str;
	char *error;

	ver_str = aux_itoa(datash->counter);
	length = _strlen(datash->av[0]) + _strlen(ver_str);
	length += _strlen(datash->args[0]) + 24;
	error = malloc(sizeof(char) * (length + 1));
	if (error == 0)
	{
		free(error);
		free(ver_str);
		return (NULL);
	}
	_strcpy(error, datash->av[0]);
	_strcat(error, ": ");
	_strcat(error, ver_str);
	_strcat(error, ": ");
	_strcat(error, datash->args[0]);
	_strcat(error, ": Permission denied\n");
	_strcat(error, "\0");
	free(ver_str);
	return (error);
}










/**
 * aux_help_env - Help information for the builtin env
 * Return: no return
 */
void aux_help_env(void)
{
	char *help = "env: env [option] [name=value] [command [args]]\n\t";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "Print the enviroment of the shell.\n";
	write(STDOUT_FILENO, help, _strlen(help));

}
/**
 * aux_help_setenv - Help information for the builtin setenv
 * Return: no return
 */
void aux_help_setenv(void)
{

	char *help = "setenv: setenv (const char *name, const char *value,";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "int replace)\n\t";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "Add a new definition to the environment\n";
	write(STDOUT_FILENO, help, _strlen(help));
}
/**
 * aux_help_unsetenv - Help information for the builtin unsetenv
 * Return: no return
 */
void aux_help_unsetenv(void)
{
	char *help = "unsetenv: unsetenv (const char *name)\n\t";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "Remove an entry completely from the environment\n";
	write(STDOUT_FILENO, help, _strlen(help));
}


/**
 * aux_help_general - Entry point for help information for the help builtin
 * Return: no return
 */
void aux_help_general(void)
{
	char *help = "^-^ bash, version 1.0(1)-release\n";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "These commands are defined internally.Type 'help' to see the list";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "Type 'help name' to find out more about the function 'name'.\n\n ";
	write(STDOUT_FILENO, help, _strlen(help));
	help = " alias: alias [name=['string']]\n cd: cd [-L|[-P [-e]] [-@]] ";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "[dir]\nexit: exit [n]\n  env: env [option] [name=value] [command ";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "[args]]\n  setenv: setenv [variable] [value]\n  unsetenv: ";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "unsetenv [variable]\n";
	write(STDOUT_FILENO, help, _strlen(help));
}
/**
 * aux_help_exit - Help information fot the builint exit
 * Return: no return
 */
void aux_help_exit(void)
{
	char *help = "exit: exit [n]\n Exit shell.\n";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "Exits the shell with a status of N. If N is ommited, the exit";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "statusis that of the last command executed\n";
	write(STDOUT_FILENO, help, _strlen(help));
}

/**
 * aux_help - Help information for the builtin help.
 * Return: no return
 */
void aux_help(void)
{
	char *help = "help: help [-dms] [pattern ...]\n";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "\tDisplay information about builtin commands.\n ";
	write(STDOUT_FILENO, help, _strlen(help));
	help = "Displays brief summaries of builtin commands.\n";
	write(STDOUT_FILENO, help, _strlen(help));
}
/**
 * aux_help_alias - Help information for the builtin alias.
 * Return: no return
 */
void aux_help_alias(void)
{
	char *help = "alias: alias [-p] [name[=value]...]\n";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "\tDefine or display aliases.\n ";
	write(STDOUT_FILENO, help, _strlen(help));
}
/**
 * aux_help_cd - Help information for the builtin alias.
 * Return: no return
 */
void aux_help_cd(void)
{
	char *help = "cd: cd [-L|[-P [-e]] [-@]] [dir]\n";

	write(STDOUT_FILENO, help, _strlen(help));
	help = "\tChange the shell working directory.\n ";
	write(STDOUT_FILENO, help, _strlen(help));
}

/**
 * aux_itoa - function converts int to string.
 * @n: type int number
 * Return: String.
 */
char *aux_itoa(int n)
{
        unsigned int n1;
        int lenght = get_len(n);
        char *buffer;

        buffer = malloc(sizeof(char) * (lenght + 1));
        if (buffer == 0)
                return (NULL);

        *(buffer + lenght) = '\0';

        if (n < 0)
        {
                n1 = n * -1;
                buffer[0] = '-';
        }
        else
        {
                n1 = n;
        }

        lenght--;
        do {
                *(buffer + lenght) = (n1 % 10) + '0';
                n1 = n1 / 10;
                lenght--;
        }
        while (n1 > 0)
                ;
        return (buffer);
}














/**
 * add_sep_node_end - adds a separator found at the end
 * of a sep_list.
 * @head: head of the linked list.
 * @sep: separator found (; | &).
 * Return: address of the head.
 */
sep_list *add_sep_node_end(sep_list **head, char sep)
{
	sep_list *new, *temp;

	new = malloc(sizeof(sep_list));
	if (new == NULL)
		return (NULL);

	new->separator = sep;
	new->next = NULL;
	temp = *head;

	if (temp == NULL)
	{
		*head = new;
	}
	else
	{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}

	return (*head);
}

/**
 * free_sep_list - frees a sep_list
 * @head: head of the linked list.
 * Return: no return.
 */
void free_sep_list(sep_list **head)
{
	sep_list *temp;
	sep_list *curr;

	if (head != NULL)
	{
		curr = *head;
		while ((temp = curr) != NULL)
		{
			curr = curr->next;
			free(temp);
		}
		*head = NULL;
	}
}

/**
 * free_line_list - frees a line_list
 * @head: head of the linked list.
 * Return: no return.
 */
void free_line_list(line_list **head)
{
        line_list *temp;
        line_list *curr;

        if (head != NULL)
        {
                curr = *head;
                while ((temp = curr) != NULL)
                {
                        curr = curr->next;
                        free(temp);
                }
                *head = NULL;
        }
}

/**
 * free_rvar_list - frees a r_var list
 * @head: head of the linked list.
 * Return: no return.
 */
void free_rvar_list(r_var **head)
{
        r_var *temp;
        r_var *curr;

        if (head != NULL)
        {
                curr = *head;
                while ((temp = curr) != NULL)
                {
                        curr = curr->next;
                        free(temp);
                }
                *head = NULL;
        }
}



























/**
 * add_line_node_end - adds a command line at the end
 * of a line_list.
 * @head: head of the linked list.
 * @line: command line.
 * Return: address of the head.
 */
line_list *add_line_node_end(line_list **head, char *line)
{
	line_list *new, *temp;

	new = malloc(sizeof(line_list));
	if (new == NULL)
		return (NULL);

	new->line = line;
	new->next = NULL;
	temp = *head;

	if (temp == NULL)
	{
		*head = new;
	}
	else
	{
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}

	return (*head);
}

/**
 * add_rvar_node - adds a variable at the end
 * of a r_var list.
 * @head: head of the linked list.
 * @lvar: length of the variable.
 * @val: value of the variable.
 * @lval: length of the value.
 * Return: address of the head.
 */
r_var *add_rvar_node(r_var **head, int lvar, char *val, int lval)
{
        r_var *new, *temp;

        new = malloc(sizeof(r_var));
        if (new == NULL)
                return (NULL);

        new->len_var = lvar;
        new->val = val;
        new->len_val = lval;

        new->next = NULL;
        temp = *head;

        if (temp == NULL)
        {
                *head = new;
        }
        else
        {
                while (temp->next != NULL)
                        temp = temp->next;
                temp->next = new;
        }

        return (*head);
}











































/**
 * _realloc - reallocates a memory block.
 * @ptr: pointer to the memory previously allocated.
 * @old_size: size, in bytes, of the allocated space of ptr.
 * @new_size: new size, in bytes, of the new memory block.
 *
 * Return: ptr.
 * if new_size == old_size, returns ptr without changes.
 * if malloc fails, returns NULL.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *newptr;

	if (ptr == NULL)
		return (malloc(new_size));

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}

	if (new_size == old_size)
		return (ptr);

	newptr = malloc(new_size);
	if (newptr == NULL)
		return (NULL);

	if (new_size < old_size)
		_memcpy(newptr, ptr, new_size);
	else
		_memcpy(newptr, ptr, old_size);

	free(ptr);
	return (newptr);
}

/**
 * _reallocdp - reallocates a memory block of a double pointer.
 * @ptr: double pointer to the memory previously allocated.
 * @old_size: size, in bytes, of the allocated space of ptr.
 * @new_size: new size, in bytes, of the new memory block.
 *
 * Return: ptr.
 * if new_size == old_size, returns ptr without changes.
 * if malloc fails, returns NULL.
 */
char **_reallocdp(char **ptr, unsigned int old_size, unsigned int new_size)
{
	char **newptr;
	unsigned int i;

	if (ptr == NULL)
		return (malloc(sizeof(char *) * new_size));

	if (new_size == old_size)
		return (ptr);

	newptr = malloc(sizeof(char *) * new_size);
	if (newptr == NULL)
		return (NULL);

	for (i = 0; i < old_size; i++)
		newptr[i] = ptr[i];

	free(ptr);

	return (newptr);

}














/**
 * get_len - Get the lenght of a number.
 * @n: type int number.
 * Return: Lenght of a number.
 */
int get_len(int n)
{
	unsigned int n1;
	int lenght = 1;

	if (n < 0)
	{
		lenght++;
		n1 = n * -1;
	}
	else
	{
		n1 = n;
	}
	while (n1 > 9)
	{
		lenght++;
		n1 = n1 / 10;
	}

	return (lenght);
}













/**
 * _atoi - converts a string to an integer.
 * @s: input string.
 * Return: integer.
 */
int _atoi(char *s)
{
	unsigned int count = 0, size = 0, oi = 0, pn = 1, m = 1, i;

	while (*(s + count) != '\0')
	{
		if (size > 0 && (*(s + count) < '0' || *(s + count) > '9'))
			break;

		if (*(s + count) == '-')
			pn *= -1;

		if ((*(s + count) >= '0') && (*(s + count) <= '9'))
		{
			if (size > 0)
				m *= 10;
			size++;
		}
		count++;
	}

	for (i = count - size; i < count; i++)
	{
		oi = oi + ((*(s + i) - 48) * m);
		m /= 10;
	}
	return (oi * pn);
}

























/**
 * _strcat - concatenate two strings
 * @dest: char pointer the dest of the copied str
 * @src: const char pointer the source of str
 * Return: the dest
 */
char *_strcat(char *dest, const char *src)
{
	int i;
	int j;

	for (i = 0; dest[i] != '\0'; i++)
		;

	for (j = 0; src[j] != '\0'; j++)
	{
		dest[i] = src[j];
		i++;
	}

	dest[i] = '\0';
	return (dest);
}
/**
 * *_strcpy - Copies the string pointed to by src.
 * @dest: Type char pointer the dest of the copied str
 * @src: Type char pointer the source of str
 * Return: the dest.
 */
char *_strcpy(char *dest, char *src)
{

	size_t a;

	for (a = 0; src[a] != '\0'; a++)
	{
		dest[a] = src[a];
	}
	dest[a] = '\0';

	return (dest);
}
/**
 * _strcmp - Function that compares two strings.
 * @s1: type str compared
 * @s2: type str compared
 * Return: Always 0.
 */
int _strcmp(char *s1, char *s2)
{
	int i;

	for (i = 0; s1[i] == s2[i] && s1[i]; i++)
		;

	if (s1[i] > s2[i])
		return (1);
	if (s1[i] < s2[i])
		return (-1);
	return (0);
}
/**
 * _strchr - locates a character in a string,
 * @s: string.
 * @c: character.
 * Return: the pointer to the first occurrence of the character c.
 */
char *_strchr(char *s, char c)
{
	unsigned int i = 0;

	for (; *(s + i) != '\0'; i++)
		if (*(s + i) == c)
			return (s + i);
	if (*(s + i) == c)
		return (s + i);
	return ('\0');
}
/**
 * _strspn - gets the length of a prefix substring.
 * @s: initial segment.
 * @accept: accepted bytes.
 * Return: the number of accepted bytes.
 */
int _strspn(char *s, char *accept)
{
	int i, j, bool;

	for (i = 0; *(s + i) != '\0'; i++)
	{
		bool = 1;
		for (j = 0; *(accept + j) != '\0'; j++)
		{
			if (*(s + i) == *(accept + j))
			{
				bool = 0;
				break;
			}
		}
		if (bool == 1)
			break;
	}
	return (i);
}

/**
 * _strdup - duplicates a str in the heap memory.
 * @s: Type char pointer str
 * Return: duplicated str
 */
char *_strdup(const char *s)
{
	char *new;
	size_t len;

	len = _strlen(s);
	new = malloc(sizeof(char) * (len + 1));
	if (new == NULL)
		return (NULL);
	_memcpy(new, s, len + 1);
	return (new);
}

/**
 * _strlen - Returns the lenght of a string.
 * @s: Type char pointer
 * Return: Always 0.
 */
int _strlen(const char *s)
{
	int len;

	for (len = 0; s[len] != 0; len++)
	{
	}
	return (len);
}


/**
 * _memcpy - copies information between void pointers.
 * @newptr: destination pointer.
 * @ptr: source pointer.
 * @size: size of the new pointer.
 *
 * Return: no return.
 */
void _memcpy(void *newptr, const void *ptr, unsigned int size)
{
        char *char_ptr = (char *)ptr;
        char *char_newptr = (char *)newptr;
        unsigned int i;

        for (i = 0; i < size; i++)
                char_newptr[i] = char_ptr[i];
}


/**
 * cmp_chars - compare chars of strings
 * @str: input string.
 * @delim: delimiter.
 *
 * Return: 1 if are equals, 0 if not.
 */
int cmp_chars(char str[], const char *delim)
{
	unsigned int i, j, k;

	for (i = 0, k = 0; str[i]; i++)
	{
		for (j = 0; delim[j]; j++)
		{
			if (str[i] == delim[j])
			{
				k++;
				break;
			}
		}
	}
	if (i == k)
		return (1);
	return (0);
}

/**
 * _strtok - splits a string by some delimiter.
 * @str: input string.
 * @delim: delimiter.
 *
 * Return: string splited.
 */
char *_strtok(char str[], const char *delim)
{
	static char *splitted, *str_end;
	char *str_start;
	unsigned int i, bool;

	if (str != NULL)
	{
		if (cmp_chars(str, delim))
			return (NULL);
		splitted = str; /*Store first address*/
		i = _strlen(str);
		str_end = &str[i]; /*Store last address*/
	}
	str_start = splitted;
	if (str_start == str_end) /*Reaching the end*/
		return (NULL);

	for (bool = 0; *splitted; splitted++)
	{
		/*Breaking loop finding the next token*/
		if (splitted != str_start)
			if (*splitted && *(splitted - 1) == '\0')
				break;
		/*Replacing delimiter for null char*/
		for (i = 0; delim[i]; i++)
		{
			if (*splitted == delim[i])
			{
				*splitted = '\0';
				if (splitted == str_start)
					str_start++;
				break;
			}
		}
		if (bool == 0 && *splitted) /*Str != Delim*/
			bool = 1;
	}
	if (bool == 0) /*Str == Delim*/
		return (NULL);
	return (str_start);
}





















/**
 * _isdigit - defines if string passed is a number
 *
 * @s: input string
 * Return: 1 if string is a number. 0 in other case.
 */
int _isdigit(const char *s)
{
	unsigned int i;

	for (i = 0; s[i]; i++)
	{
		if (s[i] < 48 || s[i] > 57)
			return (0);
	}
	return (1);
}


















/**
 * rev_string - reverses a string.
 * @s: input string.
 * Return: no return.
 */
void rev_string(char *s)
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
 * cd_dot - changes to the parent directory
 *
 * @datash: data relevant (environ)
 *
 * Return: no return
 */
void cd_dot(data_shell *datash)
{
	char pwd[PATH_MAX];
	char *dir, *cp_pwd, *cp_strtok_pwd;

	getcwd(pwd, sizeof(pwd));
	cp_pwd = _strdup(pwd);
	set_env("OLDPWD", cp_pwd, datash);
	dir = datash->args[1];
	if (_strcmp(".", dir) == 0)
	{
		set_env("PWD", cp_pwd, datash);
		free(cp_pwd);
		return;
	}
	if (_strcmp("/", cp_pwd) == 0)
	{
		free(cp_pwd);
		return;
	}
	cp_strtok_pwd = cp_pwd;
	rev_string(cp_strtok_pwd);
	cp_strtok_pwd = _strtok(cp_strtok_pwd, "/");
	if (cp_strtok_pwd != NULL)
	{
		cp_strtok_pwd = _strtok(NULL, "\0");

		if (cp_strtok_pwd != NULL)
			rev_string(cp_strtok_pwd);
	}
	if (cp_strtok_pwd != NULL)
	{
		chdir(cp_strtok_pwd);
		set_env("PWD", cp_strtok_pwd, datash);
	}
	else
	{
		chdir("/");
		set_env("PWD", "/", datash);
	}
	datash->status = 0;
	free(cp_pwd);
}

/**
 * cd_to - changes to a directory given
 * by the user
 *
 * @datash: data relevant (directories)
 * Return: no return
 */
void cd_to(data_shell *datash)
{
	char pwd[PATH_MAX];
	char *dir, *cp_pwd, *cp_dir;

	getcwd(pwd, sizeof(pwd));

	dir = datash->args[1];
	if (chdir(dir) == -1)
	{
		get_error(datash, 2);
		return;
	}

	cp_pwd = _strdup(pwd);
	set_env("OLDPWD", cp_pwd, datash);

	cp_dir = _strdup(dir);
	set_env("PWD", cp_dir, datash);

	free(cp_pwd);
	free(cp_dir);

	datash->status = 0;

	chdir(dir);
}

/**
 * cd_previous - changes to the previous directory
 *
 * @datash: data relevant (environ)
 * Return: no return
 */
void cd_previous(data_shell *datash)
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

	set_env("OLDPWD", cp_pwd, datash);

	if (chdir(cp_oldpwd) == -1)
		set_env("PWD", cp_pwd, datash);
	else
		set_env("PWD", cp_oldpwd, datash);

	p_pwd = _getenv("PWD", datash->_environ);

	write(STDOUT_FILENO, p_pwd, _strlen(p_pwd));
	write(STDOUT_FILENO, "\n", 1);

	free(cp_pwd);
	if (p_oldpwd)
		free(cp_oldpwd);

	datash->status = 0;

	chdir(p_pwd);
}

/**
 * cd_to_home - changes to home directory
 *
 * @datash: data relevant (environ)
 * Return: no return
 */
void cd_to_home(data_shell *datash)
{
	char *p_pwd, *home;
	char pwd[PATH_MAX];

	getcwd(pwd, sizeof(pwd));
	p_pwd = _strdup(pwd);

	home = _getenv("HOME", datash->_environ);

	if (home == NULL)
	{
		set_env("OLDPWD", p_pwd, datash);
		free(p_pwd);
		return;
	}

	if (chdir(home) == -1)
	{
		get_error(datash, 2);
		free(p_pwd);
		return;
	}

	set_env("OLDPWD", p_pwd, datash);
	set_env("PWD", home, datash);
	free(p_pwd);
	datash->status = 0;
}

/**
 * cd_shell - changes current directory
 *
 * @datash: data relevant
 * Return: 1 on success
 */
int cd_shell(data_shell *datash)
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
		cd_to_home(datash);
		return (1);
	}

	if (_strcmp("-", dir) == 0)
	{
		cd_previous(datash);
		return (1);
	}

	if (_strcmp(".", dir) == 0 || _strcmp("..", dir) == 0)
	{
		cd_dot(datash);
		return (1);
	}

	cd_to(datash);

	return (1);
}
























/**
 * repeated_char - counts the repetitions of a char
 *
 * @input: input string
 * @i: index
 * Return: repetitions
 */
int repeated_char(char *input, int i)
{
	if (*(input - 1) == *input)
		return (repeated_char(input - 1, i + 1));

	return (i);
}


/**
 * first_char - finds index of the first char
 *
 * @input: input string
 * @i: index
 * Return: 1 if there is an error. 0 in other case.
 */
int first_char(char *input, int *i)
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
















/**
 * error_sep_op - finds syntax errors
 *
 * @input: input string
 * @i: index
 * @last: last char read
 * Return: index of error. 0 when there are no
 * errors
 */
int error_sep_op(char *input, int i, char last)
{
        int count;

        count = 0;
        if (*input == '\0')
                return (0);

        if (*input == ' ' || *input == '\t')
                return (error_sep_op(input + 1, i + 1, last));

        if (*input == ';')
                if (last == '|' || last == '&' || last == ';')
                        return (i);

        if (*input == '|')
        {
                if (last == ';' || last == '&')
                        return (i);

                if (last == '|')
                {
                        count = repeated_char(input, 0);
                        if (count == 0 || count > 1)
                                return (i);
                }
        }

        if (*input == '&')
        {
		if (last == ';' || last == '|')
                        return (i);

                if (last == '&')
                {
                        count = repeated_char(input, 0);
                        if (count == 0 || count > 1)
                                return (i);
                }
        }

        return (error_sep_op(input + 1, i + 1, *input));
}


/**
 * print_syntax_error - prints when a syntax error is found
 *
 * @datash: data structure
 * @input: input string
 * @i: index of the error
 * @bool: to control msg error
 * Return: no return
 */
void print_syntax_error(data_shell *datash, char *input, int i, int bool)
{
	char *msg, *msg2, *msg3, *error, *counter;
	int length;

	if (input[i] == ';')
	{
		if (bool == 0)
			msg = (input[i + 1] == ';' ? ";;" : ";");
		else
			msg = (input[i - 1] == ';' ? ";;" : ";");
	}

	if (input[i] == '|')
		msg = (input[i + 1] == '|' ? "||" : "|");

	if (input[i] == '&')
		msg = (input[i + 1] == '&' ? "&&" : "&");

	msg2 = ": Syntax error: \"";
	msg3 = "\" unexpected\n";
	counter = aux_itoa(datash->counter);
	length = _strlen(datash->av[0]) + _strlen(counter);
	length += _strlen(msg) + _strlen(msg2) + _strlen(msg3) + 2;

	error = malloc(sizeof(char) * (length + 1));
	if (error == 0)
	{
		free(counter);
		return;
	}
	_strcpy(error, datash->av[0]);
	_strcat(error, ": ");
	_strcat(error, counter);
	_strcat(error, msg2);
	_strcat(error, msg);
	_strcat(error, msg3);
	_strcat(error, "\0");

	write(STDERR_FILENO, error, length);
	free(error);
	free(counter);
}

/**
 * check_syntax_error - intermediate function to
 * find and print a syntax error
 *
 * @datash: data structure
 * @input: input string
 * Return: 1 if there is an error. 0 in other case
 */
int check_syntax_error(data_shell *datash, char *input)
{
	int begin = 0;
	int f_char = 0;
	int i = 0;

	f_char = first_char(input, &begin);
	if (f_char == -1)
	{
		print_syntax_error(datash, input, begin, 0);
		return (1);
	}

	i = error_sep_op(input + begin, 0, *(input + begin));
	if (i != 0)
	{
		print_syntax_error(datash, input, begin + i, 1);
		return (1);
	}

	return (0);
}



























/**
 * is_cdir - checks ":" if is in the current directory.
 * @path: type char pointer char.
 * @i: type int pointer of index.
 * Return: 1 if the path is searchable in the cd, 0 otherwise.
 */
int is_cdir(char *path, int *i)
{
	if (path[*i] == ':')
		return (1);

	while (path[*i] != ':' && path[*i])
	{
		*i += 1;
	}

	if (path[*i])
		*i += 1;

	return (0);
}

/**
 * _which - locates a command
 *
 * @cmd: command name
 * @_environ: environment variable
 * Return: location of the command.
 */
char *_which(char *cmd, char **_environ)
{
	char *path, *ptr_path, *token_path, *dir;
	int len_dir, len_cmd, i;
	struct stat st;

	path = _getenv("PATH", _environ);
	if (path)
	{
		ptr_path = _strdup(path);
		len_cmd = _strlen(cmd);
		token_path = _strtok(ptr_path, ":");
		i = 0;
		while (token_path != NULL)
		{
			if (is_cdir(path, &i))
				if (stat(cmd, &st) == 0)
					return (cmd);
			len_dir = _strlen(token_path);
			dir = malloc(len_dir + len_cmd + 2);
			_strcpy(dir, token_path);
			_strcat(dir, "/");
			_strcat(dir, cmd);
			_strcat(dir, "\0");
			if (stat(dir, &st) == 0)
			{
				free(ptr_path);
				return (dir);
			}
			free(dir);
			token_path = _strtok(NULL, ":");
		}
		free(ptr_path);
		if (stat(cmd, &st) == 0)
			return (cmd);
		return (NULL);
	}
	if (cmd[0] == '/')
		if (stat(cmd, &st) == 0)
			return (cmd);
	return (NULL);
}

/**
 * is_executable - determines if is an executable
 *
 * @datash: data structure
 * Return: 0 if is not an executable, other number if it does
 */
int is_executable(data_shell *datash)
{
	struct stat st;
	int i;
	char *input;

	input = datash->args[0];
	for (i = 0; input[i]; i++)
	{
		if (input[i] == '.')
		{
			if (input[i + 1] == '.')
				return (0);
			if (input[i + 1] == '/')
				continue;
			else
				break;
		}
		else if (input[i] == '/' && i != 0)
		{
			if (input[i + 1] == '.')
				continue;
			i++;
			break;
		}
		else
			break;
	}
	if (i == 0)
		return (0);

	if (stat(input + i, &st) == 0)
	{
		return (i);
	}
	get_error(datash, 127);
	return (-1);
}




















/**
 * check_error_cmd - verifies if user has permissions to access
 *
 * @dir: destination directory
 * @datash: data structure
 * Return: 1 if there is an error, 0 if not
 */
int check_error_cmd(char *dir, data_shell *datash)
{
	if (dir == NULL)
	{
		get_error(datash, 127);
		return (1);
	}

	if (_strcmp(datash->args[0], dir) != 0)
	{
		if (access(dir, X_OK) == -1)
		{
			get_error(datash, 126);
			free(dir);
			return (1);
		}
		free(dir);
	}
	else
	{
		if (access(datash->args[0], X_OK) == -1)
		{
			get_error(datash, 126);
			return (1);
		}
	}

	return (0);
}

/**
 * cmd_exec - executes command lines
 *
 * @datash: data relevant (args and input)
 * Return: 1 on success.
 */
int cmd_exec(data_shell *datash)
{
	pid_t pd;
	pid_t wpd;
	int state;
	int exec;
	char *dir;
	(void) wpd;

	exec = is_executable(datash);
	if (exec == -1)
		return (1);
	if (exec == 0)
	{
		dir = _which(datash->args[0], datash->_environ);
		if (check_error_cmd(dir, datash) == 1)
			return (1);
	}

	pd = fork();
	if (pd == 0)
	{
		if (exec == 0)
			dir = _which(datash->args[0], datash->_environ);
		else
			dir = datash->args[0];
		execve(dir + exec, datash->args, datash->_environ);
	}
	else if (pd < 0)
	{
		perror(datash->av[0]);
		return (1);
	}
	else
	{
		do {
			wpd = waitpid(pd, &state, WUNTRACED);
		} while (!WIFEXITED(state) && !WIFSIGNALED(state));
	}

	datash->status = state / 256;
	return (1);
}























/**
 * cmp_env_name - compares env variables names
 * with the name passed.
 * @nenv: name of the environment variable
 * @name: name passed
 *
 * Return: 0 if are not equal. Another value if they are.
 */
int cmp_env_name(const char *nenv, const char *name)
{
	int i;

	for (i = 0; nenv[i] != '='; i++)
	{
		if (nenv[i] != name[i])
		{
			return (0);
		}
	}

	return (i + 1);
}

/**
 * _getenv - get an environment variable
 * @name: name of the environment variable
 * @_environ: environment variable
 *
 * Return: value of the environment variable if is found.
 * In other case, returns NULL.
 */
char *_getenv(const char *name, char **_environ)
{
	char *ptr_env;
	int i, mov;

	/* Initialize ptr_env value */
	ptr_env = NULL;
	mov = 0;
	/* Compare all environment variables */
	/* environ is declared in the header file */
	for (i = 0; _environ[i]; i++)
	{
		/* If name and env are equal */
		mov = cmp_env_name(_environ[i], name);
		if (mov)
		{
			ptr_env = _environ[i];
			break;
		}
	}

	return (ptr_env + mov);
}

/**
 * _env - prints the evironment variables
 *
 * @datash: data relevant.
 * Return: 1 on success.
 */
int _env(data_shell *datash)
{
	int i, j;

	for (i = 0; datash->_environ[i]; i++)
	{

		for (j = 0; datash->_environ[i][j]; j++)
			;

		write(STDOUT_FILENO, datash->_environ[i], j);
		write(STDOUT_FILENO, "\n", 1);
	}
	datash->status = 0;

	return (1);
}

/**
 * set_env - sets an environment variable
 *
 * @name: name of the environment variable
 * @value: value of the environment variable
 * @datash: data structure (environ)
 * Return: no return
 */
void set_env(char *name, char *value, data_shell *datash)
{
	int i;
	char *var_env, *name_env;

	for (i = 0; datash->_environ[i]; i++)
	{
		var_env = _strdup(datash->_environ[i]);
		name_env = _strtok(var_env, "=");
		if (_strcmp(name_env, name) == 0)
		{
			free(datash->_environ[i]);
			datash->_environ[i] = copy_info(name_env, value);
			free(var_env);
			return;
		}
		free(var_env);
	}

	datash->_environ = _reallocdp(datash->_environ, i, sizeof(char *) * (i + 2));
	datash->_environ[i] = copy_info(name, value);
	datash->_environ[i + 1] = NULL;
}

/**
 * _setenv - compares env variables names
 * with the name passed.
 * @datash: data relevant (env name and env value)
 *
 * Return: 1 on success.
 */
int _setenv(data_shell *datash)
{

	if (datash->args[1] == NULL || datash->args[2] == NULL)
	{
		get_error(datash, -1);
		return (1);
	}

	set_env(datash->args[1], datash->args[2], datash);

	return (1);
}

/**
 * _unsetenv - deletes a environment variable
 *
 * @datash: data relevant (env name)
 *
 * Return: 1 on success.
 */
int _unsetenv(data_shell *datash)
{
	char **realloc_environ;
	char *var_env, *name_env;
	int i, j, k;

	if (datash->args[1] == NULL)
	{
		get_error(datash, -1);
		return (1);
	}
	k = -1;
	for (i = 0; datash->_environ[i]; i++)
	{
		var_env = _strdup(datash->_environ[i]);
		name_env = _strtok(var_env, "=");
		if (_strcmp(name_env, datash->args[1]) == 0)
		{
			k = i;
		}
		free(var_env);
	}
	if (k == -1)
	{
		get_error(datash, -1);
		return (1);
	}
	realloc_environ = malloc(sizeof(char *) * (i));
	for (i = j = 0; datash->_environ[i]; i++)
	{
		if (i != k)
		{
			realloc_environ[j] = datash->_environ[i];
			j++;
		}
	}
	realloc_environ[j] = NULL;
	free(datash->_environ[k]);
	free(datash->_environ);
	datash->_environ = realloc_environ;
	return (1);
}



















/**
 * exec_line - finds builtins and commands
 *
 * @datash: data relevant (args)
 * Return: 1 on success.
 */
int exec_line(data_shell *datash)
{
	int (*builtin)(data_shell *datash);

	if (datash->args[0] == NULL)
		return (1);

	builtin = get_builtin(datash->args[0]);

	if (builtin != NULL)
		return (builtin(datash));

	return (cmd_exec(datash));
}

















/**
 * copy_info - copies info to create
 * a new env or alias
 * @name: name (env or alias)
 * @value: value (env or alias)
 *
 * Return: new env or alias.
 */
char *copy_info(char *name, char *value)
{
        char *new;
        int len_name, len_value, len;

        len_name = _strlen(name);
        len_value = _strlen(value);
        len = len_name + len_value + 2;
        new = malloc(sizeof(char) * (len));
        _strcpy(new, name);
        _strcat(new, "=");
        _strcat(new, value);
        _strcat(new, "\0");

        return (new);
}















/**
 * exit_shell - exits the shell
 *
 * @datash: data relevant (status and args)
 * Return: 0 on success.
 */
int exit_shell(data_shell *datash)
{
	unsigned int ustatus;
	int is_digit;
	int str_len;
	int big_number;

	if (datash->args[1] != NULL)
	{
		ustatus = _atoi(datash->args[1]);
		is_digit = _isdigit(datash->args[1]);
		str_len = _strlen(datash->args[1]);
		big_number = ustatus > (unsigned int)INT_MAX;
		if (!is_digit || str_len > 10 || big_number)
		{
			get_error(datash, 2);
			datash->status = 2;
			return (1);
		}
		datash->status = (ustatus % 256);
	}
	return (0);
}












/**
 * get_builtin - builtin that pais the command in the arg
 * @cmd: command
 * Return: function pointer of the builtin command
 */
int (*get_builtin(char *cmd))(data_shell *)
{
	builtin_t builtin[] = {
		{ "env", _env },
		{ "exit", exit_shell },
		{ "setenv", _setenv },
		{ "unsetenv", _unsetenv },
		{ "cd", cd_shell },
		{ "help", get_help },
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
 * get_error - calls the error according the builtin, syntax or permission
 * @datash: data structure that contains arguments
 * @eval: error value
 * Return: error
 */
int get_error(data_shell *datash, int eval)
{
	char *error;

	switch (eval)
	{
	case -1:
		error = error_env(datash);
		break;
	case 126:
		error = error_path_126(datash);
		break;
	case 127:
		error = error_not_found(datash);
		break;
	case 2:
		if (_strcmp("exit", datash->args[0]) == 0)
			error = error_exit_shell(datash);
		else if (_strcmp("cd", datash->args[0]) == 0)
			error = error_get_cd(datash);
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
 * get_help - function that retrieves help messages according builtin
 * @datash: data structure (args and input)
 * Return: Return 0
*/
int get_help(data_shell *datash)
{

	if (datash->args[1] == 0)
		aux_help_general();
	else if (_strcmp(datash->args[1], "setenv") == 0)
		aux_help_setenv();
	else if (_strcmp(datash->args[1], "env") == 0)
		aux_help_env();
	else if (_strcmp(datash->args[1], "unsetenv") == 0)
		aux_help_unsetenv();
	else if (_strcmp(datash->args[1], "help") == 0)
		aux_help();
	else if (_strcmp(datash->args[1], "exit") == 0)
		aux_help_exit();
	else if (_strcmp(datash->args[1], "cd") == 0)
		aux_help_cd();
	else if (_strcmp(datash->args[1], "alias") == 0)
		aux_help_alias();
	else
		write(STDERR_FILENO, datash->args[0],
		      _strlen(datash->args[0]));

	datash->status = 0;
	return (1);
}






















/**
 * bring_line - assigns the line var for get_line
 * @lineptr: Buffer that store the input str
 * @buffer: str that is been called to line
 * @n: size of line
 * @j: size of buffer
 */
void bring_line(char **lineptr, size_t *n, char *buffer, size_t j)
{

	if (*lineptr == NULL)
	{
		if  (j > BUFSIZE)
			*n = j;

		else
			*n = BUFSIZE;
		*lineptr = buffer;
	}
	else if (*n < j)
	{
		if (j > BUFSIZE)
			*n = j;
		else
			*n = BUFSIZE;
		*lineptr = buffer;
	}
	else
	{
		_strcpy(*lineptr, buffer);
		free(buffer);
	}
}

/**
 * read_line - reads the input string.
 *
 * @i_eof: return value of getline function
 * Return: input string
 */
char *read_line(int *i_eof)
{
        char *input = NULL;
        size_t bufsize = 0;

        *i_eof = getline(&input, &bufsize, stdin);

        return (input);
}

/**
 * get_line - Read inpt from stream
 * @lineptr: buffer that stores the input
 * @n: size of lineptr
 * @stream: stream to read from
 * Return: The number of bytes
 */
ssize_t get_line(char **lineptr, size_t *n, FILE *stream)
{
	int i;
	static ssize_t input;
	ssize_t retval;
	char *buffer;
	char t = 'z';

	if (input == 0)
		fflush(stream);
	else
		return (-1);
	input = 0;

	buffer = malloc(sizeof(char) * BUFSIZE);
	if (buffer == 0)
		return (-1);
	while (t != '\n')
	{
		i = read(STDIN_FILENO, &t, 1);
		if (i == -1 || (i == 0 && input == 0))
		{
			free(buffer);
			return (-1);
		}
		if (i == 0 && input != 0)
		{
			input++;
			break;
		}
		if (input >= BUFSIZE)
			buffer = _realloc(buffer, input, input + 1);
		buffer[input] = t;
		input++;
	}
	buffer[input] = '\0';
	bring_line(lineptr, n, buffer, input);
	retval = input;
	if (i != 0)
		input = 0;
	return (retval);
}










/**
 * get_sigint - Handle the crtl + c call in prompt
 * @sig: Signal handler
 */
void get_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\nblessing ", 20);
}

















/**
 * check_env - checks if the typed variable is an env variable
 *
 * @h: head of linked list
 * @in: input string
 * @data: data structure
 * Return: no return
 */
void check_env(r_var **h, char *in, data_shell *data)
{
	int row, chr, j, lval;
	char **_envr;

	_envr = data->_environ;
	for (row = 0; _envr[row]; row++)
	{
		for (j = 1, chr = 0; _envr[row][chr]; chr++)
		{
			if (_envr[row][chr] == '=')
			{
				lval = _strlen(_envr[row] + chr + 1);
				add_rvar_node(h, j, _envr[row] + chr + 1, lval);
				return;
			}

			if (in[j] == _envr[row][chr])
				j++;
			else
				break;
		}
	}

	for (j = 0; in[j]; j++)
	{
		if (in[j] == ' ' || in[j] == '\t' || in[j] == ';' || in[j] == '\n')
			break;
	}

	add_rvar_node(h, j, NULL, 0);
}

/**
 * check_vars - check if the typed variable is $$ or $?
 *
 * @h: head of the linked list
 * @in: input string
 * @st: last status of the Shell
 * @data: data structure
 * Return: no return
 */
int check_vars(r_var **h, char *in, char *st, data_shell *data)
{
	int i, lst, lpd;

	lst = _strlen(st);
	lpd = _strlen(data->pid);

	for (i = 0; in[i]; i++)
	{
		if (in[i] == '$')
		{
			if (in[i + 1] == '?')
				add_rvar_node(h, 2, st, lst), i++;
			else if (in[i + 1] == '$')
				add_rvar_node(h, 2, data->pid, lpd), i++;
			else if (in[i + 1] == '\n')
				add_rvar_node(h, 0, NULL, 0);
			else if (in[i + 1] == '\0')
				add_rvar_node(h, 0, NULL, 0);
			else if (in[i + 1] == ' ')
				add_rvar_node(h, 0, NULL, 0);
			else if (in[i + 1] == '\t')
				add_rvar_node(h, 0, NULL, 0);
			else if (in[i + 1] == ';')
				add_rvar_node(h, 0, NULL, 0);
			else
				check_env(h, in + i, data);
		}
	}

	return (i);
}
























/**
 * replaced_input - replaces string into variables
 *
 * @head: head of the linked list
 * @input: input string
 * @new_input: new input string (replaced)
 * @nlen: new length
 * Return: replaced string
 */
char *replaced_input(r_var **head, char *input, char *new_input, int nlen)
{
	r_var *indx;
	int i, j, k;

	indx = *head;
	for (j = i = 0; i < nlen; i++)
	{
		if (input[j] == '$')
		{
			if (!(indx->len_var) && !(indx->len_val))
			{
				new_input[i] = input[j];
				j++;
			}
			else if (indx->len_var && !(indx->len_val))
			{
				for (k = 0; k < indx->len_var; k++)
					j++;
				i--;
			}
			else
			{
				for (k = 0; k < indx->len_val; k++)
				{
					new_input[i] = indx->val[k];
					i++;
				}
				j += (indx->len_var);
				i--;
			}
			indx = indx->next;
		}
		else
		{
			new_input[i] = input[j];
			j++;
		}
	}

	return (new_input);
}




























/**
 * rep_var - calls functions to replace string into vars
 *
 * @input: input string
 * @datash: data structure
 * Return: replaced string
 */
char *rep_var(char *input, data_shell *datash)
{
	r_var *head, *indx;
	char *status, *new_input;
	int olen, nlen;

	status = aux_itoa(datash->status);
	head = NULL;

	olen = check_vars(&head, input, status, datash);

	if (head == NULL)
	{
		free(status);
		return (input);
	}

	indx = head;
	nlen = 0;

	while (indx != NULL)
	{
		nlen += (indx->len_val - indx->len_var);
		indx = indx->next;
	}

	nlen += olen;

	new_input = malloc(sizeof(char) * (nlen + 1));
	new_input[nlen] = '\0';

	new_input = replaced_input(&head, input, new_input, nlen);

	free(input);
	free(status);
	free_rvar_list(&head);

	return (new_input);
}

/**
 * without_comment - deletes comments from the input
 *
 * @in: input string
 * Return: input without comments
 */
char *without_comment(char *in)
{
	int i, up_to;

	up_to = 0;
	for (i = 0; in[i]; i++)
	{
		if (in[i] == '#')
		{
			if (i == 0)
			{
				free(in);
				return (NULL);
			}

			if (in[i - 1] == ' ' || in[i - 1] == '\t' || in[i - 1] == ';')
				up_to = i;
		}
	}

	if (up_to != 0)
	{
		in = _realloc(in, i, up_to + 1);
		in[up_to] = '\0';
	}

	return (in);
}
























/**
 * shell_loop - Loop of shell
 * @datash: data relevant (av, input, args)
 *
 * Return: no return.
 */
void shell_loop(data_shell *datash)
{
	int loop, i_eof;
	char *input;

	loop = 1;
	while (loop == 1)
	{
		write(STDIN_FILENO, "^-^ ", 4);
		input = read_line(&i_eof);
		if (i_eof != -1)
		{
			input = without_comment(input);
			if (input == NULL)
				continue;

			if (check_syntax_error(datash, input) == 1)
			{
				datash->status = 2;
				free(input);
				continue;
			}
			input = rep_var(input, datash);
			loop = split_commands(datash, input);
			datash->counter += 1;
			free(input);
		}
		else
		{
			loop = 0;
			free(input);
		}
	}
}























/**
 * swap_char - swaps | and & for non-printed chars
 *
 * @input: input string
 * @bool: type of swap
 * Return: swapped string
 */
char *swap_char(char *input, int bool)
{
	int i;

	if (bool == 0)
	{
		for (i = 0; input[i]; i++)
		{
			if (input[i] == '|')
			{
				if (input[i + 1] != '|')
					input[i] = 16;
				else
					i++;
			}

			if (input[i] == '&')
			{
				if (input[i + 1] != '&')
					input[i] = 12;
				else
					i++;
			}
		}
	}
	else
	{
		for (i = 0; input[i]; i++)
		{
			input[i] = (input[i] == 16 ? '|' : input[i]);
			input[i] = (input[i] == 12 ? '&' : input[i]);
		}
	}
	return (input);
}

/**
 * add_nodes - add separators and command lines in the lists
 *
 * @head_s: head of separator list
 * @head_l: head of command lines list
 * @input: input string
 * Return: no return
 */
void add_nodes(sep_list **head_s, line_list **head_l, char *input)
{
	int i;
	char *line;

	input = swap_char(input, 0);

	for (i = 0; input[i]; i++)
	{
		if (input[i] == ';')
			add_sep_node_end(head_s, input[i]);

		if (input[i] == '|' || input[i] == '&')
		{
			add_sep_node_end(head_s, input[i]);
			i++;
		}
	}

	line = _strtok(input, ";|&");
	do {
		line = swap_char(line, 1);
		add_line_node_end(head_l, line);
		line = _strtok(NULL, ";|&");
	} while (line != NULL);

}

/**
 * go_next - go to the next command line stored
 *
 * @list_s: separator list
 * @list_l: command line list
 * @datash: data structure
 * Return: no return
 */
void go_next(sep_list **list_s, line_list **list_l, data_shell *datash)
{
	int loop_sep;
	sep_list *ls_s;
	line_list *ls_l;

	loop_sep = 1;
	ls_s = *list_s;
	ls_l = *list_l;

	while (ls_s != NULL && loop_sep)
	{
		if (datash->status == 0)
		{
			if (ls_s->separator == '&' || ls_s->separator == ';')
				loop_sep = 0;
			if (ls_s->separator == '|')
				ls_l = ls_l->next, ls_s = ls_s->next;
		}
		else
		{
			if (ls_s->separator == '|' || ls_s->separator == ';')
				loop_sep = 0;
			if (ls_s->separator == '&')
				ls_l = ls_l->next, ls_s = ls_s->next;
		}
		if (ls_s != NULL && !loop_sep)
			ls_s = ls_s->next;
	}

	*list_s = ls_s;
	*list_l = ls_l;
}
























/**
 * split_commands - splits command lines according to
 * the separators ;, | and &, and executes them
 *
 * @datash: data structure
 * @input: input string
 * Return: 0 to exit, 1 to continue
 */
int split_commands(data_shell *datash, char *input)
{

	sep_list *head_s, *list_s;
	line_list *head_l, *list_l;
	int loop;

	head_s = NULL;
	head_l = NULL;

	add_nodes(&head_s, &head_l, input);

	list_s = head_s;
	list_l = head_l;

	while (list_l != NULL)
	{
		datash->input = list_l->line;
		datash->args = split_line(datash->input);
		loop = exec_line(datash);
		free(datash->args);

		if (loop == 0)
			break;

		go_next(&list_s, &list_l, datash);

		if (list_l != NULL)
			list_l = list_l->next;
	}

	free_sep_list(&head_s);
	free_line_list(&head_l);

	if (loop == 0)
		return (0);
	return (1);
}

/**
 * split_line - tokenizes the input string
 *
 * @input: input string.
 * Return: string splitted.
 */
char **split_line(char *input)
{
	size_t bsize;
	size_t i;
	char **tokens;
	char *token;

	bsize = TOK_BUFSIZE;
	tokens = malloc(sizeof(char *) * (bsize));
	if (tokens == NULL)
	{
		write(STDERR_FILENO, ": allocation error\n", 18);
		exit(EXIT_FAILURE);
	}

	token = _strtok(input, TOK_DELIM);
	tokens[0] = token;

	for (i = 1; token != NULL; i++)
	{
		if (i == bsize)
		{
			bsize += TOK_BUFSIZE;
			tokens = _reallocdp(tokens, i, sizeof(char *) * bsize);
			if (tokens == NULL)
			{
				write(STDERR_FILENO, ": allocation error\n", 18);
				exit(EXIT_FAILURE);
			}
		}
		token = _strtok(NULL, TOK_DELIM);
		tokens[i] = token;
	}

	return (tokens);
}
