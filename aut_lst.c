#include "ssh.h"

/**
 * empt_dif_list - frees a sep_list
 * @head: head of the linked list.
 * Return: no return.
 */

void empt_dif_list(sep_list **head)
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
 * empt_LN_list - frees a line_list
 * @head: head of the linked list.
 * Return: no return.
 */

void empt_LN_list(line_list **head)
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
 * empt_Rvar_list - frees a r_var list
 * @head: head of the linked list.
 * Return: no return.
 */

void empt_Rvar_list(r_var **head)
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
