/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépin 2017                     *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

 #ifndef __JOKER_H
 #define __JOKER_H

 #include <stdio.h>
 #include <stdlib.h>
 #include <wordexp.h>

 #include "readcmd.h"


/**
 * Allows using jokers in command line.
 * Jokers can be replaced before execution.
 */
void expand_commands(struct cmdline *);


/**
 * Linked-list structure to chain args dynamically.
 * After the expansion of a whole command, this struct
 * is turned into an array for line->seq[i].
 *
 * The head of the list should be the program name.
 */
struct cmd_arg {
    char            *s;
    struct cmd_arg  *next;
};


/**
 * Init the linked list of args.
 */
struct cmd_arg *init_arg(char s[]);


/**
 * Add an argument to the linked list of args.
 */
void add_arg(struct cmd_arg **list, char s[]);


/**
 * Free the linked list of args.
 */
void free_args(struct cmd_arg **list);


/**
 * Get size of linked list of args.
 */
int count_args(struct cmd_arg *list);


#endif
