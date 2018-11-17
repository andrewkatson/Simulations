/*
 * Takes in completion and arrival times for a series of jobs 
 * and calculates the service time for each job 
 * @author Andrew Katson
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define FILENAME   "ac.dat"                  /* input data file */
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
   double GetCompletion(FILE *fp)                 /* read a completion time */
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

  //hold all the service times
  double serviceTimes[500];

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

  double numJobs = 0;                          /* the number of jobs at time t  */
  double minServiceTime = 0xFFFF;                 /* the minimum service time */
  double maxSericeTime = 0.0;                  /* the maximum service time */
  int currJob = 0; 

  while (!feof(fp)) {
    index++;
    arrival      = GetArrival(fp);


    if (arrival < departure) {
      delay      = departure - arrival;        /* delay in queue    */
      numDelayedJobs++;                        /*number of jobs delayed */
    }
    else 
      delay      = 0.0;                        /* no delay          */
    departure    = GetCompletion(fp);
    wait         = departure - arrival;             /* wait*/
	service      = wait - delay;
	//record the service time 
	serviceTimes[currJob] = service;
    sum.delay   += delay;
    sum.wait    += wait;
    sum.service += service;
    totalJobs++;
    //printf("%6.2f\n", service);
    if(arrival < timeOfJobs && timeOfJobs < departure){
      numJobs++;
    }

    if(service < minServiceTime){
    	minServiceTime = service;
    }
    if(service > maxSericeTime){
    	maxSericeTime = service;
    }

    if(delay > maxDelay){
      maxDelay = delay;
    }

    currJob++;
  }
  sum.interarrival = arrival - START;


  //Calcualte the standard deviation of the service times 
  double standardDeviation; 
  double variance;
  double runningSum;
  double mean = sum.service/500;

  for(int i = 0; i < index; i++){
  	runningSum += pow((serviceTimes[i] - mean),2);
  }
  variance = runningSum/500;

  standardDeviation = sqrt(variance);


  double utilization = (1000/departure)*(sum.service/1000);
  
  printf("\nfor %ld jobs\n", index);
  printf("   average interarrival time... = %6.2f\n", sum.interarrival / index);
  printf("   average service time ....... = %6.2f\n", sum.service / index);
  printf("   average delay .............. = %6.2f\n", sum.delay / index);
  printf("   average wait ............... = %6.2f\n", sum.wait / index);
  printf("   maximum delay .............. = %6.2f\n", maxDelay);
  printf("   proportion of jobs delayed.. = %6.2f\n", numDelayedJobs/totalJobs);
  printf("   the utilization is ......... = %6.2f\n", utilization);
  printf("   traffic intensity is ....... = %6.2f\n", (sum.service/index)/(sum.interarrival/index));
  
  printf("\n\n");
  printf("Statistics for 4.3.5\n");
  printf("   sample mean for service .... = %6.2f\n", sum.service / index);
  printf("   sample standard deviation .. = %6.2f\n", standardDeviation);
  printf("   sample variance ............ = %6.2f\n", variance);
  printf("   maximum service time ....... = %6.2f\n", maxSericeTime);
  printf("   minimum service time ....... = %6.2f\n", minServiceTime);

  printf("   best value for K ........... = %6.2f\n", floor((5.0/3.0) * cbrt(500.0)));

  if(timeOfJobs > 0){
    printf("   number of jobs at time %6.2f = %6.2f\n", timeOfJobs, numJobs);
  }
  
  
  fclose(fp);
  return (0);
}
