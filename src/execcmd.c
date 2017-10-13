/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépini 2017                    *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include "execcmd.h"

void execute(struct cmdline *cmd)
{
  printf("\nCommand requested:\n%s\n", *cmd->seq[0]);
}