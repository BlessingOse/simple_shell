#include "ssh.h"

/**
 * aut_asst_CDir - Help information for the builtin alias.
 * Return: no return
 */

void aut_asst_CDir(void)
{
        char *help = "cd: cd [-L|[-P [-e]] [-@]] [dir]\n";

        write(STDOUT_FILENO, help, _strlen(help));
        help = "\tChange the shell working directory.\n ";
        write(STDOUT_FILENO, help, _strlen(help));
}

/**
 * aut_itoa - function converts int to string.
 * @n: type int number
 * Return: String.
 */

char *aut_itoa(int n)
{
        unsigned int n1;
        int lenght = fetch_len(n);
        char *buffer;

        buffer = malloc(sizeof(char) * (lenght + 1));
        if (buffer == 0)
                return (NULL);

        *(buffer + lenght) = '\0';

        if (n < 0)
        {
                n1 = n * -1;
                buffer[0] = '-';
        }
        else
        {
                n1 = n;
        }

        lenght--;
        do {
                *(buffer + lenght) = (n1 % 10) + '0';
                n1 = n1 / 10;
                lenght--;
        }
        while (n1 > 0)
                ;
        return (buffer);
}

/**
 * inc_dif_node_extr - adds a separator found at the end
 * of a sep_list.
 * @head: head of the linked list.
 * @sep: separator found (; | &).
 * Return: address of the head.
 */

sep_list *inc_dif_node_extr(sep_list **head, char sep)
{
        sep_list *new, *temp;

        new = malloc(sizeof(sep_list));
        if (new == NULL)
                return (NULL);

        new->separator = sep;
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
