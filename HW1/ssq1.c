
/* -------------------------------------------------------------------------
 * This program simulates a single-server FIFO service node using arrival
 * times and service times read from a text file.  The server is assumed
 * to be idle when the first job arrives.  All jobs are processed completely
 * so that the server is again idle at the end of the simulation.   The
 * output statistics are the average interarrival time, average service
 * time, the average delay in the queue, and the average wait in the service 
 * node. 
 *
 * Name              : ssq1.c  (Single Server Queue, version 1)
 * Authors           : Steve Park & Dave Geyer
 * Language          : ANSI C
 * Latest Revision   : 9-01-98
 * Compile with      : gcc ssq1.c 
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>                              

#define FILENAME   "ssq1.dat"                  /* input data file */
#define START      0.0

/* =========================== */
   double GetArrival(FILE *fp)                 /* read an arrival time */
/* =========================== */
{ 
  double a;

  fscanf(fp, "%lf", &a);
  return (a);
}

/* =========================== */
   double GetService(FILE *fp)                 /* read a service time */
/* =========================== */
{ 
  double s;

  fscanf(fp, "%lf\n", &s);
  return (s);
}
/* =========================== */
   double getNumJobs(double serviceNode[], int jobNodeLen)                 /* get the number of jobs in the service array*/
/* =========================== */
{ 
  double numJobs = 0;

    for(int q = 0; q < jobNodeLen; q++){
      if((serviceNode[q] >= 1.00 || serviceNode[q] <= -1.00 )){
        numJobs++;
      }
    }

  return numJobs;
}

/* ============== */
   int main(void)
/* ============== */
{
  FILE   *fp;                                  /* input data file      */
  long   index     = 0;                        /* job index            */
  double arrival   = START;                    /* arrival time         */
  double delay;                                /* delay in queue       */
  double service;                              /* service time         */
  double wait;                                 /* delay + service      */
  double departure = START;                    /* departure time       */
  double maxDelay = -10000;                    /* maximum maxDelay     */
  double totalJobs = 0;                        /* total number of jobs */
  double numDelayedJobs = 0;                   /* number of delayed jobs */
  double timeOfJobs = 400;                     /* number of jobs at this time */
  struct {                                     /* sum of ...           */
    double delay;                              /*   delay times        */
    double wait;                               /*   wait times         */
    double service;                            /*   service times      */
    double interarrival;                       /*   interarrival times */
  } sum = {0.0, 0.0, 0.0};

  fp = fopen(FILENAME, "r");
  if (fp == NULL) {
    fprintf(stderr, "Cannot open input file %s\n", FILENAME);
    return (1);
  }

  double numJobs = 0;                          /* the number of jobs at time t                                 */

  while (!feof(fp)) {
    index++;
    arrival      = GetArrival(fp);


    if (arrival < departure) {
      delay      = departure - arrival;        /* delay in queue    */
      numDelayedJobs++;                        /*number of jobs delayed */
    }
    else 
      delay      = 0.0;                        /* no delay          */
    service      = GetService(fp);
    wait         = delay + service;
    departure    = arrival + wait;             /* time of departure */
    sum.delay   += delay;
    sum.wait    += wait;
    sum.service += service;
    totalJobs++;

    if(arrival < timeOfJobs && timeOfJobs < departure){
      numJobs++;
    }

    if(delay > maxDelay){
      maxDelay = delay;
    }
  }
  sum.interarrival = arrival - START;

  double utilization = (1000/departure)*(sum.service/1000);

  printf("\nfor %ld jobs\n", index);
  printf("   average interarrival time... = %6.2f\n", sum.interarrival / index);
  printf("   average service time ....... = %6.2f\n", sum.service / index);
  printf("   average delay .............. = %6.2f\n", sum.delay / index);
  printf("   average wait ............... = %6.2f\n", sum.wait / index);
  printf("   maximum delay .............. = %6.2f\n", maxDelay);
  printf("   proportion of jobs delayed.. = %6.2f\n", numDelayedJobs/totalJobs);
  printf("   the utilization is ......... = %6.2f\n", utilization);
  
  if(timeOfJobs > 0){
    printf("   number of jobs at time %6.2f = %6.2f\n", timeOfJobs, numJobs);
  }

  fclose(fp);
  return (0);
}
