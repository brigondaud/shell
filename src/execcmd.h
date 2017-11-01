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

/* Executes a pack of command */

/* Execute a command read on the standard input */
void execute(struct cmdline *line);

/**
 * Check the redirections.
 * Open and close file descriptors if needed.
 */
void check_in_out(char *in, char *out);
void check_in_out_desc(int in_desc, int out_desc);

/**
 * Count commands.
 * Return -1 if the command line cannot be read.
 */
int count_commands(struct cmdline *line);

/**
 * Execute a single command and manage input-output.
 */
void execute_command(char **cmd, int bg, int in_desc, int out_desc);

#endif
