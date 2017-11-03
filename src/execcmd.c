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

	int i;
	int channel[2];
	int in_desc = get_redirection_desc(line, 1);

	/* check_in_out(line->in, line->out); */
	int number_of_commands = count_commands(line);

	// Let's create one process per pipe!
	for (i = 0; i < number_of_commands - 1; ++i) {

		// 1. Change input and output
		pipe(channel);

		// 2. Execute the command
		execute_command(line->seq[i], line->bg, in_desc, channel[1]);

		// 3. For the next process, redirect the input
		in_desc = channel[0];
		close(channel[1]);
	}

	int out_desc = get_redirection_desc(line, 0);
	execute_command(line->seq[i], line->bg, in_desc, out_desc);
	close(channel[0]);

	// int status;
	// pid_t pid = fork();
	//
	// switch(pid) {
	// 	case -1:
	// 		perror("fork: ");
	// break;
	//
	// 	case 0: /*In child: exec the command */
	// 		check_in_out(line->in, line->out);
	// 		execvp(prog, args);
	// 	break;
	//
	// 	default: /* In father: wait for child to finish */
	// 	if (line->bg != 1){
	// 		if (waitpid(pid, &status, 0) == -1) {
	// 	// if (wait(NULL)==-1){
	// 		perror("wait: ");
	// 		exit(EXIT_FAILURE);
	// 	}
	// 	} else {
	// 		/* Register the job */
	// 		job_register(pid, prog);
	// 	}
	// 	break;
	// }
}


void execute_command(char **cmd, int bg, int in_desc, int out_desc)
{
	int status;
	char *prog = cmd[0];
	char **args = cmd++;

	pid_t pid = fork();

	switch (pid) {
		case -1:
			perror("fork: ");
			break;

		case 0:
			/* In child, before execution, check input/output. */
			check_in_out_desc(in_desc, out_desc);

			/* In child, execute the command. */
			execvp(prog, args);
			break;

		default:
			/* In father, execute the command. */
			if (!bg) {
				if (waitpid(pid, &status, 0) == -1) {
					perror("waitpid: ");
					exit(EXIT_FAILURE);
				}
			} else {
				/* Register the job */
				job_register(pid, prog);
			}
	}
}


// TODO. Peut-être inutilisable avec les pipes maintenant.
// void check_in_out(char *in, char *out)
// {
// 	int in_code, out_code;
//
// 	/* File redirection : < */
// 	if (in != NULL) {
// 		in_code = open(in, O_RDONLY);
// 		if (in_code == -1)
// 			perror("in redirection (<): ");
//
// 		if (dup2(in_code, 0) == -1)
// 			perror("descriptor duplication [in]: ");
//
// 		close(in_code);
// 	}
//
// 	/* File redirection : > */
// 	if (out != NULL) {
// 		out_code = open(out, O_WRONLY | O_CREAT);
// 		if (out_code == -1)
// 			perror("out redirection (>): ");
//
// 		if (dup2(out_code, 1) == -1)
// 			perror("descriptor duplication [out]: ");
//
// 		close(out_code);
// 	}
// }


void check_in_out_desc(int in_desc, int out_desc)
{
	// printf("%d %d\n", in_desc, out_desc);

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
