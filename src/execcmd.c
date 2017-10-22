/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépini 2017                    *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include "execcmd.h"

void execute(char **cmd)
{
  /* The command and its args */
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
      if (wait(NULL)==-1){
        perror("wait: ");
        exit(EXIT_FAILURE);
      }
  }
}