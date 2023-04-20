#include "sh.h"

/**
 * _strcpy - string copy
 * @dest: destination
 * @src: the source
 *
 * Return: pointer to destination
 */

char *_strcpy(char *dest, char *src)
{
	int n = 0;

	if (dest == src || src == 0)
	{
		return (dest);
	}
	while (src[n])
	{
		dest[n] = src[n];
		n++;
	}
	dest[n] = 0;
	return (dest);
}

/**
 * _strdup - string duplicate
 * @str: string to be duplicated
 *
 * Return: pointer to the duplicated string
 */

char *_strdup(const char *str)
{
	int length = 0;
	char *ret;

	if (str == NULL)
	{
		return (NULL);
	}
	while (*str++)
	{
		length++;
	}
	ret = malloc(sizeof(char) * (length + 1));
	if (!ret)
	{
		return (NULL);
	}
	for (length++; length--;)
	{
		ret[length] = *--str;
	}
	return (ret);
}

/**
 *_puts - prints an input string
 *@str: the string to be printed
 *
 * Return: Nothing
 */

void _puts(char *str)
{
	int n = 0;

	if (!str)
	{
		return;
	}
	while (str[n] != '\0')
	{
		_putchar(str[n]);
		n++;
	}
}

/**
 * _putchar - prints the char c to standard output
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */

int _putchar(char c)
{
	static int n;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || n >= WRITE_BUF_SIZE)
	{
		write(1, buf, n);
		n = 0;
	}
	if (c != BUF_FLUSH)
	{
		buf[n++] = c;
	}
	return (1);
}
