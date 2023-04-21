#include "sh.h"

/**
 * read_source - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */

int read_source(info_t *info)
{
	int n, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_source_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (n = 0; n < fsize; n++)
		if (buf[n] == '\n')
		{
			buf[n] = 0;
			build_source_list(info, buf + last, linecount++);
			last = n + 1;
		}
	if (last != n)
		build_source_list(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		del_node_index(&(info->source), 0);
	renumber_source(info);
	return (info->histcount);
}

/**
 * build_source_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */

int build_source_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->source)
	{
		node = info->source;
	}
	inc_node_end(&node, buf, linecount);

	if (!info->source)
	{
		info->source = node;
	}
	return (0);
}

/**
 * write_source - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */

int write_source(info_t *info)
{
	ssize_t fd;
	char *filename = get_source_file(info);
	list_t *node = NULL;

	if (!filename)
	{
		return (-1);
	}

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
	{
		return (-1);
	}
	for (node = info->source; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * get_source_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_source_file(info_t *info)
{
	char *buf, *dir;

	dir = _getenv(info, "HOME=");
	if (!dir)
	{
		return (NULL);
	}
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
	{
		return (NULL);
	}
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * renumber_source - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */

int renumber_source(info_t *info)
{
	list_t *node = info->source;
	int n = 0;

	while (node)
	{
		node->num = n++;
		node = node->next;
	}
	return (info->histcount = n);
}
