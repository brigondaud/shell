/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépin 2017                     *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include "joker.h"

void expand_commands(struct cmdline *l)
{
	int i, j, k;
	wordexp_t p;

	struct cmd_arg *head;
	struct cmd_arg *current;

	for (i = 0; l->seq[i] != 0; i++) {

		// 1. Initialize the command linked list
		head = init_arg(l->seq[i][0]);
		current = head;

		// 2. Check the command arguments expansions
		for (j = 1; l->seq[i][j] != 0; j++) {
			int expansion = wordexp(l->seq[i][j], &p, WRDE_UNDEF);

			switch (expansion) {
			case WRDE_BADVAL:
				/* Non-existent variable becomes a non-existent arg. */
				break;
			case 0:
				/* Everything's good. */
				for (k = 0; k < p.we_wordc; k++)
					add_arg(&current, p.we_wordv[k]);

				break;
			default:
				/* Unknown error. Stop. */
				printf("wordexp error. TODO. \n");
				break;
			}

			wordfree(&p);
		}

		// 3. Group the result
		struct cmd_arg *save_head = head;
		int number_of_args = count_args(head);
		j = 0;

		char **new_seq = malloc((number_of_args + 1) * sizeof(char *));

		while (head != NULL) {
			new_seq[j] = malloc(strlen(head->s) + 1);
			strncpy(new_seq[j], head->s, strlen(head->s) + 1);

			head = head->next;
			++j;
		}

		new_seq[j] = NULL;

		free(l->seq[i]);
		l->seq[i] = new_seq;

		free_args(&save_head);
	}
}

struct cmd_arg *init_arg(char s[])
{
	struct cmd_arg *head = malloc(sizeof(struct cmd_arg));

	head->s = malloc(strlen(s) + 1);
	head->next = NULL;

	strncpy(head->s, s, strlen(s) + 1);
	return head;
}

void add_arg(struct cmd_arg **list, char s[])
{
	struct cmd_arg *new_arg = malloc(sizeof(struct cmd_arg));

	new_arg->s = malloc(strlen(s) + 1);
	new_arg->next = NULL;

	strncpy(new_arg->s, s, strlen(s) + 1);
	(*list)->next = new_arg;
	*list = new_arg;
}

void free_args(struct cmd_arg **list)
{
	if (list == NULL)
		return;

	struct cmd_arg *current = *list;
	struct cmd_arg *next_arg = current;

	while (current != NULL) {
		next_arg = current->next;

		free(current->s);
		free(current);
		current = next_arg;
	}

	*list = NULL;
}

int count_args(struct cmd_arg *list)
{
	int number_of_args = 0;

	while (list != NULL) {
		number_of_args++;
		list = list->next;
	}

	return number_of_args;
}
