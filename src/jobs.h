/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépin 2017                     *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>

/**
 * A status can either be running or finished
 */
typedef enum {
  RUNNING,
  DONE
} job_status;

/**
 * A stack of the jobs running in background
 */
struct jobs {
  pid_t pid;
  char *job_name;
  job_status status;
  struct job *next;
};

struct jobs *first_job;
struct jobs *last_job;

/**
 * Jobs command: display all the jobs running in background and
 * their status (running, finished)
 */
void jobs(void);

/**
 * Register a job as running in the job list
 */
void job_register(pid_t pid, char *job_name);

/**
 * Modify the status of a job in the jobs list
 * Returns -1 if the job was not found
 */
int change_status(struct job *job, job_status status);