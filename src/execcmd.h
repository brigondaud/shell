/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépini 2017                    *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#ifndef __EXECCMD_H
#define __EXECCMD_H

#include <stdio.h>
#include "readcmd.h"

/* Execute a command read on the standard input */
void execute(struct cmdline *cmd);

#endif
