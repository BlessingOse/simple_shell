#include "sh.h"

/**
 **_strncat - concatenates two strings
 *@dest: the first string
 *@src: the second string
 *@n: the amount of bytes to be maximally used
 *
 *Return: the concatenated string
 */

char *_strncat(char *dest, char *src, int n)
{
	int p, x;
	char *s = dest;

	p = 0;
	x = 0;
	while (dest[n] != '\0')
	{
		p++;
	}
	while (src[x] != '\0' && x < n)
	{
		dest[p] = src[x];
		p++;
		x++;
	}
	if (x < n)
		dest[p] = '\0';
	return (s);
}

/**
 **_strncpy - copies a string
 *@dest: the destination string to be copied to
 *@src: the source string
 *@n: the amount of characters to be copied
 *
 *Return: the concatenated string
 */

char *_strncpy(char *dest, char *src, int n)
{
	int p, x;
	char *s = dest;

	p = 0;
	x = 0;
	while (src[p] != '\0' && x < n - 1)
	{
		dest[p] = src[p];
		p++;
	}
	if (p < n)
	{
		x = p;
		while (x < n)
		{
			dest[x] = '\0';
			x++;
		}
	}
	return (s);
}

/**
 **_strchr - locates a character in a string
 *@s: the string to be parsed
 *@c: the character to look for
 *
 *Return: (s) a pointer to the memory area s
 */

char *_strchr(char *s, char c)
{
	do {
		if (*s == c)
			return (s);
	} while (*s++ != '\0');

	return (NULL);
}
