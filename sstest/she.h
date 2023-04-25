#ifndef _SHE_H_
#define _SHE_H_

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>

#define BUFSIZE 1024
#define TOK_BUFSIZE 128
#define TOK_DELIM " \t\r\n\a"

/* Points to an array of pointers to strings called the "environment" */
extern char **environ;


/**
 * struct data - struct that contains all relevant data on runtime
 * @av: argument vector
 * @input: command line written by the user
 * @args: tokens of the command line
 * @status: last status of the shell
 * @counter: lines counter
 * @_environ: environment variable
 * @pid: process ID of the shell
 */
typedef struct data
{
	char **av;
	char *input;
	char **args;
	int status;
	int counter;
	char **_environ;
	char *pid;
} data_shell;

/**
 * struct sep_list_s - single linked list
 * @separator: ; | &
 * @next: next node
 * Description: single linked list to store separators
 */
typedef struct sep_list_s
{
	char separator;
	struct sep_list_s *next;
} sep_list;

/**
 * struct line_list_s - single linked list
 * @line: command line
 * @next: next node
 * Description: single linked list to store command lines
 */
typedef struct line_list_s
{
	char *line;
	struct line_list_s *next;
} line_list;

/**
 * struct r_var_list - single linked list
 * @len_var: length of the variable
 * @val: value of the variable
 * @len_val: length of the value
 * @next: next node
 * Description: single linked list to store variables
 */
typedef struct r_var_list
{
	int len_var;
	char *val;
	int len_val;
	struct r_var_list *next;
} r_var;

/**
 * struct builtin_s - Builtin struct for command args.
 * @name: The name of the command builtin i.e cd, exit, env
 * @f: data type pointer function.
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(data_shell *datash);
} builtin_t;

/* aux_lists.c */
sep_list *inc_dif_node_extr(sep_list **head, char sep);
void empt_dif_list(sep_list **head);
line_list *inc_LN_node_extr(line_list **head, char *line);
void empt_LN_list(line_list **head);
void freeDataFxn(data_shell *datash);
void init_data(data_shell *datash, char **av);

/* aux_lists2.c */
r_var *inc_Rvar_node(r_var **head, int lvar, char *var, int lval);
void empt_Rvar_list(r_var **head);

/* aux_str functions */
char *_strcat(char *dest, const char *src);
char *_strcpy(char *dest, char *src);
int _strcmp(char *s1, char *s2);
char *_strchr(char *s, char c);
int _strspn(char *s, char *accept);

/* aux_mem.c */
void _memcpy(void *newptr, const void *ptr, unsigned int size);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
char **_reallocdp(char **ptr, unsigned int old_size, unsigned int new_size);

/* aux_str2.c */
char *_strdup(const char *s);
int _strlen(const char *s);
int liken_chars(char str[], const char *delim);
char *_strtok(char str[], const char *delim);
int _isdigit(const char *s);

/* aux_str3.c */
void undo_string(char *s);

/* check_syntax_error.c */
int dup_Char(char *input, int i);
int err_sep_OP(char *input, int i, char last);
int basic_Char(char *input, int *i);
void dis_stx_err(data_shell *datash, char *input, int i, int bool);
int confirm_stx_err(data_shell *datash, char *input);

/* shell_loop.c */
char *RMV_com(char *in);
void SH_loop(data_shell *datash);

/* read_line.c */
char *scan_LN(int *i_eof);

/* split.c */
char *EXCH_char(char *input, int bool);
void inc_nodes(sep_list **head_s, line_list **head_l, char *input);
void nxt_cmd(sep_list **list_s, line_list **list_l, data_shell *datash);
int SEP_cMDS(data_shell *datash, char *input);
char **sep_LN(char *input);

/* rep_var.c */
void confirm_ENV(r_var **h, char *in, data_shell *data);
int confirm_VARS(r_var **h, char *in, char *st, data_shell *data);
char *rest_inp(r_var **head, char *input, char *new_input, int nlen);
char *rest_VAR(char *input, data_shell *datash);

/* get_line.c */
void show_LN(char **lineptr, size_t *n, char *buffer, size_t j);
ssize_t fetch_LN(char **lineptr, size_t *n, FILE *stream);

/* exec_line */
int XCute_line(data_shell *datash);

/* cmd_exec.c */
int test_CDir(char *path, int *i);
char *_locate(char *cmd, char **_environ);
int test_EXEcute(data_shell *datash);
int confirm_ERR(char *dir, data_shell *datash);
int CMD_XC(data_shell *datash);

/* env1.c */
char *_getenv(const char *name, char **_environ);
int _env(data_shell *datash);
int jux_ENV_ID(const char *nenv, const char *name);

/* env2.c */
char *MIM_detail(char *name, char *value);
void PREP_env(char *name, char *value, data_shell *datash);
int _setenv(data_shell *datash);
int _unsetenv(data_shell *datash);

/* cd.c */
void CDir_ftp(data_shell *datash);
void CDir_loc(data_shell *datash);
void CDir_last(data_shell *datash);
void CDir_loc_HM(data_shell *datash);

/* cd_shell.c */
int CDir_SH(data_shell *datash);

/* get_builtin */
int (*fetch_builtin(char *cmd))(data_shell *datash);

/* _exit.c */
int quit_SH(data_shell *datash);

/* aux_stdlib.c */
int fetch_len(int n);
char *aut_itoa(int n);
int _atoi(char *s);

/* aux_error1.c */
char *strCat_cdir(data_shell *, char *, char *, char *);
char *err_getCdir(data_shell *datash);
char *err_missing(data_shell *datash);
char *err_EXT_sh(data_shell *datash);

/* aux_error2.c */
char *error_get_alias(char **args);
char *err_env_msg(data_shell *datash);
char *error_syntax(char **args);
char *error_permission(char **args);
char *err_RD_126(data_shell *datash);


/* get_error.c */
int fetch_ERR(data_shell *datash, int eval);

/* get_sigint.c */
void fetch_SIGINT(int sig);

/* aux_help.c */
void aut_asst_env(void);
void aut_asst_setenv(void);
void aut_asst_unsetenv(void);
void aut_asst_Gen(void);
void aut_asst_Ext(void);

/* aux_help2.c */
void aut_asst(void);
void aut_asst_alias(void);
void aut_asst_CDir(void);

/* get_help.c */
int fetch_asst(data_shell *datash);

#endif
