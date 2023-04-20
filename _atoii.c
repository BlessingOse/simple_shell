#include "sh.h"

/**
 * flexible - flexible mode
 * @info: struct address
 *
 * Return: 1 if flex mode, 0 otherwise
 */

int flexible(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * is_delim - to check if character is a deli
 * @c: the char to check
 * @delim: the deli string
 *
 * Return: 1 if true, 0 if false
 */

int is_delim(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
		{
			return (1);
		}
	return (0);
}

/**
 *_isalpha - to check for alphabetic character
 *@c: character to input
 *
 *Return: 1 if c is alphabetic, 0 otherwise
 */

int _isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

/**
 *_atoi - converts a string to an integer
 *@s: the string to be converted
 *
 *Return: 0 if no numbers in string, converted number otherwise
 */

int _atoi(char *s)
{
	int n, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (n = 0;  s[n] != '\0' && flag != 2; n++)
	{
		if (s[n] == '-')
		{
			sign *= -1;
		}

		if (s[n] >= '0' && s[n] <= '9')
		{
			flag = 1;
			result = result * 10;
			result = result + (s[n] - '0');
		}
		else if (flag == 1)
		{
			flag = 2;
		}
	}

	if (sign == -1)
	{
		output = -result;
	}
	else
	{
		output = result;
	}

	return (output);
}
