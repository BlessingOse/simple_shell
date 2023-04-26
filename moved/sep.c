#include "ssh.h"

/**
 * inc_nodes - add separators and command lines in the lists
 *
 * @head_s: head of separator list
 * @head_l: head of command lines list
 * @input: input string
 * Return: no return
 */

void inc_nodes(sep_list **head_s, line_list **head_l, char *input)
{
        int i;
        char *line;

        input = EXCH_char(input, 0);

        for (i = 0; input[i]; i++)
        {
                if (input[i] == ';')
                        inc_dif_node_extr(head_s, input[i]);

                if (input[i] == '|' || input[i] == '&')
                {
                        inc_dif_node_extr(head_s, input[i]);
                        i++;
                }
        }

        line = _strtok(input, ";|&");
        do {
                line = EXCH_char(line, 1);
                inc_LN_node_extr(head_l, line);
                line = _strtok(NULL, ";|&");
        } while (line != NULL);

}

/**
 * nxt_cmd - go to the next command line stored
 *
 * @list_s: separator list
 * @list_l: command line list
 * @datash: data structure
 * Return: no return
 */

void nxt_cmd(sep_list **list_s, line_list **list_l, data_shell *datash)
{
        int loop_sep;
        sep_list *ls_s;
        line_list *ls_l;

        loop_sep = 1;
        ls_s = *list_s;
        ls_l = *list_l;

        while (ls_s != NULL && loop_sep)
        {
                if (datash->status == 0)
                {
                        if (ls_s->separator == '&' || ls_s->separator == ';')
                                loop_sep = 0;
                        if (ls_s->separator == '|')
                                ls_l = ls_l->next, ls_s = ls_s->next;
                }
                else
                {
                        if (ls_s->separator == '|' || ls_s->separator == ';')
                                loop_sep = 0;
                        if (ls_s->separator == '&')
                                ls_l = ls_l->next, ls_s = ls_s->next;
                }
                if (ls_s != NULL && !loop_sep)
                        ls_s = ls_s->next;
        }

        *list_s = ls_s;
        *list_l = ls_l;
}

/**
 * SEP_cMDS - splits command lines according to
 * the separators ;, | and &, and executes them
 *
 * @datash: data structure
 * @input: input string
 * Return: 0 to exit, 1 to continue
 */

int SEP_cMDS(data_shell *datash, char *input)
{

        sep_list *head_s, *list_s;
        line_list *head_l, *list_l;
        int loop;

        head_s = NULL;
        head_l = NULL;

        inc_nodes(&head_s, &head_l, input);

        list_s = head_s;
        list_l = head_l;

        while (list_l != NULL)
        {
                datash->input = list_l->line;
                datash->args = sep_LN(datash->input);
                loop = XCute_line(datash);
                free(datash->args);

                if (loop == 0)
                        break;

                nxt_cmd(&list_s, &list_l, datash);

                if (list_l != NULL)
                        list_l = list_l->next;
        }

        empt_dif_list(&head_s);
        empt_LN_list(&head_l);

        if (loop == 0)
                return (0);
        return (1);
}
