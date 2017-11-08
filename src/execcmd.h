/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépin 2017                     *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#ifndef __EXECCMD_H
#define __EXECCMD_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "readcmd.h"
#include "jobs.h"

/* Execute a command read on the standard input */
void execute(struct cmdline *line);

/**
 * Open and close file descriptors for pipes.
 */
void check_in_out_desc(int in_desc, int out_desc);

/**
 * Check redirections and manage descriptors if needed.
 * If is_input, the redirection checks the input, else the output.
 */
int get_redirection_desc(struct cmdline *line, int is_input);

/**
 * Count commands.
 * Return -1 if the command line cannot be read.
 */
int count_commands(struct cmdline *line);

/**
 * Execute a single command and manage input-output.
 */
void execute_command(struct cmdline *, int, const int, int *, int);

#endif
