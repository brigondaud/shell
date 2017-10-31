/*****************************************************
 * Copyright Baptiste Rigondaud 2017                 *
 *           Aurélien Pépin 2017                     *
 * This code is distributed under the GLPv3 licence. *
 * Ce code est distribué sous la licence GPLv3+.     *
 *****************************************************/

#include "jobs.h"


void jobs(void)
{
  struct job *current = first_job;
  while(current){

    /* First, check the status of the job */
    check_status(current);

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

// int change_status(pid_t pid, job_status status)
// {
//   struct job *current = first_job;
//   if (!current)
//     return -1;
//   while(current != last_job || current->pid != pid) {
//     current = current->next;
//   }
//   if (current->pid != pid)
//     return -1;
//
//   current->status = status;
//   return 0;
// }

// void update_jobs(void)
// {
//   struct job *current = first_job;
//   while (current) {
//     int status;
//     int wait = waitpid(current->pid, &status, WNOHANG);
//     if(wait != 0) {
//       if (wait == -1) {
//         perror("waitpid: ");
//       } else {
//         int res = change_status(current->pid, FINISHED);
//         if (res == -1) {
//           perror("Change status failed: ");
//         }
//       }
//     }
//   }
// }

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
