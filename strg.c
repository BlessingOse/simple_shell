#include "sh.h"

/**
 * _lenstr - length of a string
 * @s: string
 *
 * Return: integer length of string
 */

int _lenstr(char *s)
{
	int n = 0;

	if (!s)
	{
		return (0);
	}

	while (*s++)
	{
		n++;
	}
	return (n);
}

/**
 * _cmpstr - compares two strings
 * @s1: string one
 * @s2: string two
 *
 * Return: negative if s1 < s2, positive if s1 > s2, zero if s1 == s2
 */

int _cmpstr(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
		{
			return (*s1 - *s2);
		}
		s1++;
		s2++;
	}
	if (*s1 == *s2)
	{
		return (0);
	}
	else
	{
		return (*s1 < *s2 ? -1 : 1);
	}
}

/**
 * begins_with - checks if needle starts with haystack
 * @haystack: string to search
 * @needle: the substring to find
 *
 * Return: address of next char of haystack or NULL
 */

char *begins_with(const char *haystack, const char *needle)
{
	while (*needle)
	{
		if (*needle++ != *haystack++)
		{
			return (NULL);
		}
	}
	return ((char *)haystack);
}

/**
 * _catstr - concatenates two strings
 * @dest: the destination buffer
 * @src: the source buffer
 *
 * Return: pointer to destination buffer
 */

char *_catstr(char *dest, char *src)
{
	char *ret = dest;

	while (*dest)
	{
		dest++;
	}
	while (*src)
	{
		*dest++ = *src++;
	}
	*dest = *src;
	return (ret);
}
