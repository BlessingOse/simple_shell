#include "ssh.h"

/**
 * sep_LN - tokenizes the input string
 *
 * @input: input string.
 * Return: string splitted.
 */
char **sep_LN(char *input)
{
	size_t bsize;
	size_t x;
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

	for (x = 1; token != NULL; x++)
	{
		if (x == bsize)
		{
			bsize += TOK_BUFSIZE;
			tokens = _reallocdp(tokens, x, sizeof(char *) * bsize);
			if (tokens == NULL)
			{
				write(STDERR_FILENO, ": allocation error\n", 18);
				exit(EXIT_FAILURE);
			}
		}
		token = _strtok(NULL, TOK_DELIM);
		tokens[x] = token;
	}

	return (tokens);
}
