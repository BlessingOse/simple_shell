#include "ssh.h"

/**
 * confirm_ENV - checks if the typed variable is an env variable
 *
 * @h: head of linked list
 * @in: input string
 * @data: data structure
 * Return: no return
 */

void confirm_ENV(r_var **h, char *in, data_shell *data)
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
				inc_Rvar_node(h, j, _envr[row] + chr + 1, lval);
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
	inc_Rvar_node(h, j, NULL, 0);
}

/**
 * confirm_VARS - check if the typed variable is $$ or $?
 *
 * @h: head of the linked list
 * @in: input string
 * @st: last status of the Shell
 * @data: data structure
 * Return: no return
 */

int confirm_VARS(r_var **h, char *in, char *st, data_shell *data)
{
	int i, lst, lpd;

	lst = _strlen(st);
	lpd = _strlen(data->pid);

	for (i = 0; in[i]; i++)
	{
		if (in[i] == '$')
		{
			if (in[i + 1] == '?')
				inc_Rvar_node(h, 2, st, lst), i++;
			else if (in[i + 1] == '$')
				inc_Rvar_node(h, 2, data->pid, lpd), i++;
			else if (in[i + 1] == '\n')
				inc_Rvar_node(h, 0, NULL, 0);
			else if (in[i + 1] == '\0')
				inc_Rvar_node(h, 0, NULL, 0);
			else if (in[i + 1] == ' ')
				inc_Rvar_node(h, 0, NULL, 0);
			else if (in[i + 1] == '\t')
				inc_Rvar_node(h, 0, NULL, 0);
			else if (in[i + 1] == ';')
				inc_Rvar_node(h, 0, NULL, 0);
			else
				confirm_ENV(h, in + i, data);
		}
	}
	return (i);
}

/**
 * rest_inp - replaces string into variables
 *
 * @head: head of the linked list
 * @input: input string
 * @new_input: new input string (replaced)
 * @nlen: new length
 * Return: replaced string
 */

char *rest_inp(r_var **head, char *input, char *new_input, int nlen)
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
