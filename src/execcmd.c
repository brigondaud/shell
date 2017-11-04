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

	if(!strncmp(prog, "jobs", 4)){
		printf("\n-----------JOBS-----------\n");
		jobs();
		printf("\n");
		return;
	}

	/*
	 ******************
	 * MULTIPLE PIPES *
	 ******************
	 */

	int channel[2];
	int in_desc = get_redirection_desc(line, 1);

	int number_of_commands = count_commands(line);

	// Let's create recursively one process per pipe
	execute_command(line, 0, number_of_commands, channel, in_desc);
}


void execute_command(struct cmdline *line, int i, const int noc, int *channel, int in_desc)
{
	// 1. Change input and output
	int status;
	pipe(channel);

	int out_desc = (i < noc - 1)			// Is it the last command?
		? channel[1]						// | Middle pipe
		: get_redirection_desc(line, 0);	// | End pipe

	// 2. Execute the command
	char *prog = line->seq[i][0];
	char **args = line->seq[i];

	pid_t pid = fork();
	switch (pid) {
		case -1:
			perror("fork: ");
			break;

		case 0:
			/* In child, before execution, check input/output. */
			check_in_out_desc(in_desc, out_desc);

			/* In child, execute the command. */
			if (execvp(prog, args) == -1) {
				perror("");
				exit(EXIT_FAILURE);
			}

			break;

		default:
			/* In father, launch other commands with pipes. */
			if (i < noc - 1) {
				close(channel[1]);
				execute_command(line, i + 1, noc, channel, channel[0]);
			} else {
				close(channel[0]);
			}

			/* In father, wait for the end of the child. */
			if (!line->bg) {
				if (waitpid(pid, &status, 0) == -1) {
					perror("waitpid: ");
					exit(EXIT_FAILURE);
				}
			} else {
				job_register(pid, prog);
			}
	}
}


void check_in_out_desc(int in_desc, int out_desc)
{
	/* Redirection < */
	if (in_desc != 0) {
		if (dup2(in_desc, 0) == -1)
			perror("descriptor duplication [in]: ");

		close(in_desc);
	}

	/* Redirection > */
	if (out_desc != 1) {
		if (dup2(out_desc, 1) == -1)
			perror("descriptor duplication [out]: ");

		close(out_desc);
	}
}


int get_redirection_desc(struct cmdline *line, int is_input)
{
	int desc_code;

	if (is_input) {
		if (line == NULL || !line->in)
			return 0;

		desc_code = open(line->in, O_RDONLY);
		if (desc_code == -1)
			perror("in redirection (<): ");

		return desc_code;
	} else {
		if (line == NULL || !line->out)
			return 1;

		desc_code = open(line->out, O_WRONLY | O_CREAT, 0666);
		if (desc_code == -1)
			perror("out redirection (>): ");

		return desc_code;
	}
}


int count_commands(struct cmdline *line)
{
	if (line == NULL)
		return -1;

	int number_of_commands = 0;

	while (line->seq[number_of_commands] != NULL)
		number_of_commands++;

	return number_of_commands;
}
