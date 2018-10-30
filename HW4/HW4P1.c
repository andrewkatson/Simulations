//single server queue simulation with two job types

#include <stdio.h>
#include <math.h>                                             
#include "../../rngs.c"

#define LAST         1000000L                   /* number of jobs processed */ 
#define START        0.0                      /* initial time             */ 


   double Exponential(double m)                 
/* ---------------------------------------------------
 * generate an Exponential random variate, use m > 0.0 
 * ---------------------------------------------------
 */
{                                       
  return (-m * log(1.0 - Random()));     
}


   double Uniform(double a, double b)           
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{                                         
  return (a + (b - a) * Random());    
}


   double GetArrival(int *j)
/* ------------------------------
 * generate the next arrival time
 * ------------------------------
 * Index j corresponds to job type 
 */ 
{       
  const double mean[2] = {4.0, 6.0};
  static double arrival[2] = {START, START};
  static int init = 1;
  double temp;
  if (init){
    SelectStream(0);
    arrival[0] += Exponential(mean[0]);
    SelectStream(1);
    arrival[1] += Exponential(mean[1]);
    init = 0;
  }

  if(arrival[0] <= arrival[1]){
    *j = 0;
  }
  else{
    *j = 1;
  }
  temp = arrival[*j];
  SelectStream(*j);
  arrival[*j] += Exponential(mean[*j]);
  return (temp);
}


   double GetService(int j)
/* ------------------------------
 * generate the next service time
 * ------------------------------
 */ 
{
  const double min[2] = {1.0,0.0};
  const double max[2] = {3.0,4.0};
  SelectStream(j+2);
  return (Uniform(min[j], max[j]));
}


  int main(void)
{
  FILE *f;
  f = fopen("HW4P1.txt", "a");

  for (int i = 0; i < 5; i ++){
    long   index     = 0;                         /* job index            */
    double arrival   = START;                     /* time of arrival      */
    double delay;                                 /* delay in queue       */
    double service;                               /* service time         */
    double wait;                                  /* delay + service      */
    double departure = START;                     /* time of departure    */
    struct Sums{                                      /* sum of ...           */
      double delay;                               /*   delay times        */
      double wait;                                /*   wait times         */
      double service;                             /*   service times      */
      double interarrival;                        /*   interarrival times */
      double type1Job;                            /*   jobs of type 1     */
      double type2Job;                            /*   jobs of type 2     */
    } sum = {0.0, 0.0, 0.0};  
    double type0Job;                              /*   jobs of type 1     */
    double type1Job;                              /*   jobs of type 2     */

    struct Sums type0Sums = {0.0,0.0,0.0};               
    struct Sums type1Sums = {0.0,0.0,0.0};   
    double type0LastArrival;                  
    double type1LastArrival;  


    PutSeed(0);
    int j = 0;
    while (index < LAST) {
      index++;
      arrival      = GetArrival(&j);
      if (arrival < departure)
        delay      = departure - arrival;         /* delay in queue    */
      else
        delay      = 0.0;                         /* no delay          */
      service      = GetService(j);
      wait         = delay + service;
      departure    = arrival + wait;              /* time of departure */
      sum.delay   += delay;
      sum.wait    += wait;
      sum.service += service;

      if (j == 0){
        type0Job++;
        type0Sums.delay += delay;
        type0Sums.wait += wait;
        type0Sums.service += service;
        type0Sums.interarrival += arrival - type0LastArrival;
        type0LastArrival = arrival;
      }
      else{
        type1Job++;
        type1Sums.delay += delay;
        type1Sums.wait += wait;
        type1Sums.service += service;
        type1Sums.interarrival += arrival - type1LastArrival;
        type1LastArrival = arrival;
      }

      //fprintf(f, "%6.4f\n", service);
    } 
    sum.interarrival = arrival - START;


    printf("\nfor %ld jobs\n", index);
    printf("   average interarrival time = %6.4f\n", sum.interarrival / index);
    printf("   average wait ............ = %6.4f\n", sum.wait / index);
    printf("   average delay ........... = %6.4f\n", sum.delay / index);
    printf("   average service time .... = %6.4f\n", sum.service / index);
    printf("   average # in the node ... = %6.4f\n", sum.wait / departure);
    printf("   average # in the queue .. = %6.4f\n", sum.delay / departure);
    printf("   utilization ............. = %6.4f\n", sum.service / departure);
    printf("\n\n");
    printf("Type 0 job statistics\n");
    printf("   Type 0 job proportion ... = %6.4f\n", type0Job / index);
    printf("   average interarrival time = %6.4f\n", type0Sums.interarrival / type0Job);
    printf("   average wait ............ = %6.4f\n", type0Sums.wait / type0Job);
    printf("   average delay ........... = %6.4f\n", type0Sums.delay / type0Job);
    printf("   average service time .... = %6.4f\n", type0Sums.service / type0Job);
    printf("   average # in the node ... = %6.4f\n", type0Sums.wait / departure);
    printf("   average # in the queue .. = %6.4f\n", type0Sums.delay / departure);
    printf("   utilization ............. = %6.4f\n", type0Sums.service / departure);
    printf("\n\n");
    printf("Type 1 job statistics\n");
    printf("   Type 1 job proportion ... = %6.4f\n", type1Job / index);
    printf("   average interarrival time = %6.4f\n", type1Sums.interarrival / type1Job);
    printf("   average wait ............ = %6.4f\n", type1Sums.wait / type1Job);
    printf("   average delay ........... = %6.4f\n", type1Sums.delay / type1Job);
    printf("   average service time .... = %6.4f\n", type1Sums.service / type1Job);
    printf("   average # in the node ... = %6.4f\n", type1Sums.wait / departure);
    printf("   average # in the queue .. = %6.4f\n", type1Sums.delay / departure);
    printf("   utilization ............. = %6.4f\n", type1Sums.service / departure);
  }
  fclose(f);
  return (0);
}
