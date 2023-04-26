#include "ssh.h"

/**
 * SH_loop - Loop of shell
 * @datash: data relevant (av, input, args)
 *
 * Return: no return.
 */

void SH_loop(data_shell *datash)
{
	int loop, i_eof;
	char *input;

	loop = 1;
	while (loop == 1)
	{
		write(STDIN_FILENO, "prompt_by_blessing$ ", 19);
		input = scan_LN(&i_eof);
		if (i_eof != -1)
		{
			input = RMV_com(input);
			if (input == NULL)
				continue;

			if (confirm_stx_err(datash, input) == 1)
			{
				datash->status = 2;
				free(input);
				continue;
			}
			input = rest_VAR(input, datash);
			loop = SEP_cMDS(datash, input);
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
 * EXCH_char - swaps | and & for non-printed chars
 *
 * @input: input string
 * @bool: type of swap
 * Return: swapped string
 */

char *EXCH_char(char *input, int bool)
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
