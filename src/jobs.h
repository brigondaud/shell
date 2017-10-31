/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépin 2017                     *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

/**
 * A status can either be running or finished
 */
typedef enum {
  RUNNING,
  FINISHED
} job_status;

/**
 * A stack of the jobs running in background
 */
struct job {
  pid_t pid;
  char *job_name;
  job_status status;
  struct job *next;
};

struct job *first_job;
struct job *last_job;

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
 * Returns -1 if the job was not found, else return 0
 */
// int change_status(pid_t pid, job_status status);
int change_status(struct job *the_job, job_status status);


/**
 * Goes through all the jobs and see if they are finished
 */
void update_jobs(void);

/**
 * Check if a process is finished.
 * If finished, update its current status via jobs.h@change_status
 */
void check_status(struct job *the_job);
