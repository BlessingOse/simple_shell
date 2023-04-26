#include "ssh.h"

/**
 * inc_LN_node_extr - adds a command line at the end
 * of a line_list.
 * @head: head of the linked list.
 * @line: command line.
 * Return: address of the head.
 */

line_list *inc_LN_node_extr(line_list **head, char *line)
{
        line_list *new, *temp;

        new = malloc(sizeof(line_list));
        if (new == NULL)
                return (NULL);

        new->line = line;
        new->next = NULL;
        temp = *head;

        if (temp == NULL)
        {
                *head = new;
        }
        else
        {
                while (temp->next != NULL)
                        temp = temp->next;
                temp->next = new;
        }

        return (*head);
}

/**
 * inc_Rvar_node - adds a variable at the end
 * of a r_var list.
 * @head: head of the linked list.
 * @lvar: length of the variable.
 * @val: value of the variable.
 * @lval: length of the value.
 * Return: address of the head.
 */

r_var *inc_Rvar_node(r_var **head, int lvar, char *val, int lval)
{
        r_var *new, *temp;

        new = malloc(sizeof(r_var));
        if (new == NULL)
                return (NULL);

        new->len_var = lvar;
        new->val = val;
        new->len_val = lval;

        new->next = NULL;
        temp = *head;

        if (temp == NULL)
        {
                *head = new;
        }
        else
        {
                while (temp->next != NULL)
                        temp = temp->next;
                temp->next = new;
        }

        return (*head);
}
