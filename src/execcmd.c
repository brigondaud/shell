/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépini 2017                    *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include "execcmd.h"

void execute(struct cmdline *cmd)
{
  // TODO: create as many child proc there are prog in cmd->seq
  int pid;
  if ((pid = fork()) == 0) {
    /* Child proc */
    printf("\nIn the child proc :\n%s\n", *cmd->seq[0]);
  }
  int status = -1;
  pid_t child_pid = wait(&status);
  if (child_pid == -1) {
    printf("Error terminating the child proc.\n");
  }
}