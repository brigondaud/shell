/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépin 2017                     *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include "jobs.h"

struct job *first_job = NULL;
struct job *last_job = NULL;


void jobs(void)
{
  struct job *current = first_job;

    /* First, check the status of the jobs */
    while (current){
        check_status(current);
        current = current->next;
    }
    /* Display the status of the remaining jobs */
    current = first_job;
  while(current){

    char status[9];
    switch(current->status){
      case RUNNING:
      strcpy(status, "RUNNING");
      break;
      case FINISHED:
      strcpy(status, "FINISHED");
      break;
    }
    printf("PID: %d, job: %s, status: %s\n", current->pid, current->job_name, status);
    current = current->next;
  }
}


void job_register(pid_t pid, char *job_name)
{
  /* The new job */
  struct job *job = malloc(sizeof(struct job));
  job->pid = pid;
  job->job_name = malloc(sizeof(job_name));
  strcpy(job->job_name, job_name);
  job->status = RUNNING;

  /* Registers the new job */
  if(!first_job){
    first_job = job;
    last_job = job;
  } else {
    last_job->next = job;
    last_job = job;
  }
}


int change_status(struct job *the_job, job_status status)
{
    if (the_job == NULL)
        return -1;

    the_job->status = status;
    return 0;
}

int remove_job(struct job *job)
{
    /* If no job */
    if (!first_job){
        return 0;
    }
    struct job *current  = first_job;
    struct job *prec = current;
    while (current != last_job || current != job) {
        prec = current;
        current = current->next;
    }
    /* If the job was not found */
    if (current != job) {
        return 0;
    }

    if (last_job == first_job) {
        first_job = NULL;
        last_job = NULL;
        return 1;
    }

    if (current == first_job) { /* If first job */
        first_job = current->next;
    } else if (current == last_job) { /* If last job */
        last_job = prec;
    } else {
        prec->next = current->next;
    }
    return 1;
}


int check_status(struct job *the_job)
{
    if (the_job == NULL || the_job->status == FINISHED)
        return 0;

    int wait_res = waitpid(the_job->pid, NULL, WNOHANG);

    if (wait_res != 0) {
        if (wait_res == -1)
            perror("waitpid: ");
        return remove_job(the_job);
    }
    return 0;
}
