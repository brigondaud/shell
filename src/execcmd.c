/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépin 2017                     *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include "execcmd.h"

void execute(struct cmdline *line)
{
	if (line->seq[0] == NULL)
		return;

	/* The command and its args */
	char **cmd = line->seq[0];
	char *prog = cmd[0];
	char **args = cmd++;

	if(!strncmp(prog, "jobs", 4)){
		printf("\n-----------JOBS-----------\n");
		jobs();
		printf("\n");
		return;
	}

	int status;
	pid_t pid = fork();

	switch(pid) {
		case -1:
			perror("fork: ");
	break;

		case 0: /*In child: exec the command */
			check_in_out(line->in, line->out);
			execvp(prog, args);
		break;

		default: /* In father: wait for child to finish */
		if (line->bg != 1){
			if (waitpid(pid, &status, 0) == -1) {
		// if (wait(NULL)==-1){
			perror("wait: ");
			exit(EXIT_FAILURE);
		}
		} else {
			/* Register the job */
			job_register(pid, prog);
		}
		break;
	}
}

void check_in_out(char *in, char *out)
{
		int in_code, out_code;

		/* File redirection : < */
		if (in != NULL) {
				in_code = open(in, O_RDONLY);
				if (in_code == -1)
						perror("in redirection (<): ");

				if (dup2(in_code, 0) == -1)
						perror("descriptor duplication [in]: ");

				close(in_code);
		}

		/* File redirection : > */
		if (out != NULL) {
				out_code = open(out, O_WRONLY | O_CREAT);
				if (out_code == -1)
						perror("out redirection (>): ");

				if (dup2(out_code, 1) == -1)
						perror("descriptor duplication [out]: ");

				close(out_code);
	}
}
