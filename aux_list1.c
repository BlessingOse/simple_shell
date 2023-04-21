#include "sh.h"

/**
 * len_list - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */

size_t len_list(const list_t *h)
{
	size_t n = 0;

	while (h)
	{
		h = h->next;
		n++;
	}
	return (n);
}

/**
 * list_to_strings - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */

char **list_to_strings(list_t *head)
{
	list_t *node = head;
	size_t n = len_list(head), x;
	char **strs;
	char *str;

	if (!head || !n)
		return (NULL);
	strs = malloc(sizeof(char *) * (n + 1));
	if (!strs)
		return (NULL);
	for (n = 0; node; node = node->next, n++)
	{
		str = malloc(_strlen(node->str) + 1);
		if (!str)
		{
			for (x = 0; x < n; x++)
			{
				free(strs[x]);
			}
			free(strs);
			return (NULL);
		}

		str = _strcpy(str, node->str);
		strs[n] = str;
	}
	strs[n] = NULL;
	return (strs);
}


/**
 * write_list - prints all elements of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */

size_t write_list(const list_t *h)
{
	size_t n = 0;

	while (h)
	{
		_puts(turn_number(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		n++;
	}
	return (n);
}

/**
 * node_begins_with - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */

list_t *node_begins_with(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = begins_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
		{
			return (node);
		}
		node = node->next;
	}
	return (NULL);
}

/**
 * get_node_index - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */

ssize_t get_node_index(list_t *head, list_t *node)
{
	size_t n = 0;

	while (head)
	{
		if (head == node)
		{
			return (n);
		}
		head = head->next;
		n++;
	}
	return (-1);
}
