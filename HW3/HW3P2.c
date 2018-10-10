 //Simulator to verify the steady state statistics of the single
 //server model in example 3.1.3

#include <stdio.h>
#include <stdlib.h>
#include <math.h>                                             
#include "../../rng.c"

#define LAST         10000L                   /* number of jobs processed */ 
#define START        0.0                      /* initial time             */ 

long Equilikely (long a, long b){
	return (a+ (long) ((b-a+ 1) * Random()));
}


long Geometric(double p)     
/* ---------------------------------------------------
 * generate an Geometric random variate, use 0.0 < p < 1.0 
 * ---------------------------------------------------
 *
*/
{
return ((long) (log(1.0 - Random()) / log(p)));
}

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


   double GetArrival(double arrival)
/* ------------------------------
 * generate the next arrival time
 * ------------------------------
 */ 
{      
                                    

  arrival += Exponential(2.0);
  return (arrival);
}


   double GetService(void)
/* ------------------------------
 * generate the next service time
 * ------------------------------
 */ 
{
    long k;
	double sum = 0.0;
	long tasks = 1 + Geometric(0.9);
	for (k = 0; k < tasks; k++){
		sum += Uniform(0.1, 0.2);
	}
	return (sum);
}


  int main(void)
{

  time_t t;

   FILE *f;
   f = fopen("serviceTimesP2.txt", "a");


    
    
  for(int i = 0; i < 3; i++){
    //initialize random number generator
    long changeSeed = Equilikely(0.1, 1000);
	PutSeed(time(&t) * changeSeed);

	long   index     = 0;                         /* job index            */
	double arrival   = START;                     /* time of arrival      */
	double delay;                                 /* delay in queue       */
	double service;                               /* service time         */
	double wait;                                  /* delay + service      */
	double departure = START;                     /* time of departure    */
	struct {                                      /* sum of ...           */
	    double delay;                               /*   delay times        */
	    double wait;                                /*   wait times         */
	    double service;                             /*   service times      */
	    double interarrival;                        /*   interarrival times */
	} sum = {0.0, 0.0, 0.0};  


	while (index < LAST) {
		index++;
		arrival      = GetArrival(arrival);
	if (arrival < departure)
		delay      = departure - arrival;         /* delay in queue    */
	else
		delay      = 0.0;                         /* no delay          */
		service      = GetService();
		wait         = delay + service;
		departure    = arrival + wait;              /* time of departure */
		sum.delay   += delay;
		sum.wait    += wait;
		sum.service += service;

		if (i == 0){
			fprintf(f, "%6.2f\n", service);
		}
	} 
	sum.interarrival = arrival - START;



	long *seedUsed = malloc(sizeof(long));
	GetSeed(seedUsed);
	printf("With seed %ld\n", *seedUsed);
	printf("\nfor %ld jobs\n", index);
	printf("   average interarrival time = %6.4f\n", sum.interarrival / index);
	printf("   average wait ............ = %6.4f\n", sum.wait / index);
	printf("   average delay ........... = %6.4f\n", sum.delay / index);
	printf("   average service time .... = %6.4f\n", sum.service / index);
	printf("   average # in the node ... = %6.4f\n", sum.wait / departure);
	printf("   average # in the queue .. = %6.4f\n", sum.delay / departure);
	printf("   utilization ............. = %6.4f\n", sum.service / departure);
	printf("\n");
	free(seedUsed);
	}
	fclose(f);
  return (0);
}
