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

    /* First, check the status of the jobs. */
    while (current){
        check_status(current);
        current = current->next;
    }

    /* Secondly, remove jobs that are finished. */
    remove_finished_jobs();

    /* Display the status of the remaining jobs */
    current = first_job;

    while (current) {
        printf("PID: %d, job: %s, status: %s\n", current->pid, current->job_name, "RUNNING");
        current = current->next;
    }
}


void job_register(pid_t pid, char *job_name)
{
  /* The new job */
  struct job *job = calloc(1, sizeof(struct job));
  job->pid = pid;
  job->job_name = malloc(strlen(job_name) + 1);
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


void check_status(struct job *the_job)
{
    if (the_job == NULL || the_job->status == FINISHED)
        return;

    int wait_res = waitpid(the_job->pid, NULL, WNOHANG);

    if (wait_res != 0) {
        if (wait_res == -1)
            perror("waitpid: ");
        else {
            if (change_status(the_job, FINISHED) == -1)
                perror("Change status failed: ");
        }
    }
}


void remove_finished_jobs(void)
{
    // CASE 1. No jobs running or finished.
    if (first_job == NULL) return;

    // CASE 2. Only one job in the list.
    if (first_job == last_job && first_job->status == FINISHED) {
        free(first_job);

        first_job = NULL;
        last_job = NULL;
        return;
    }

    // CASE 3. More than one job in the list.
    struct job *begin_sentinel = malloc(sizeof (struct job));
    struct job *end_sentinel = malloc(sizeof (struct job));

    begin_sentinel->status = SENTINEL; begin_sentinel->next = first_job;
    end_sentinel->status = SENTINEL; end_sentinel->next = NULL;
    last_job->next = end_sentinel;

    // Finished jobs are going to be deleted :)
    struct job *previous = begin_sentinel;
    struct job *the_job = first_job;

    while (the_job && the_job->status != SENTINEL) {

        /* This job should be deleted now. */
        if (the_job->status == FINISHED) {
            previous->next = the_job->next;

            if (the_job == last_job)
                last_job = previous;

            free(the_job);
            the_job = previous->next;

        /* Just iterate on the list... */
        } else {
            previous = the_job;
            the_job = the_job->next;
        }
    }

    first_job = begin_sentinel->next;
    last_job->next = NULL;

    // If serveral jobs were deleted in one call...
    if (last_job == begin_sentinel) {
        first_job = NULL;
        last_job = NULL;
    }

    free(begin_sentinel);
    free(end_sentinel);
}
