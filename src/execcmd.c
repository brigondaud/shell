/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépini 2017                    *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include "execcmd.h"

void execute(struct cmdline *line)
{
  if (line->seq[0] == NULL){
    return;
  }
  /* The command and its args */
  char **cmd = line->seq[0];
  char *prog = cmd[0];
  char **args = cmd++;

  pid_t pid = fork();
  switch(pid) {
    case -1:
      perror("fork: ");
    break;

    case 0: /*In child: exec the command */
      execvp(prog, args);
    break;
    
    default: /* In father: wait for child to finish */
      if(line->bg != 1){
        printf("Waiting for child to end\n");
        if (wait(NULL)==-1){
          perror("wait: ");
          exit(EXIT_FAILURE);
        }
      }
    break;
  }
}