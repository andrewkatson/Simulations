/* ------------------------------------------------------------------------- 
 * FIFO next event simulation of a single server queue to simulate
 * a sno cone stand if it were operated by two different poetential employees
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <math.h>
#include "../rngs.h"                      /* the multi-stream generator */

#define START         0.0              /* initial time                   */
#define STOP      10000000000000.0              /* terminal (close the door) time */
#define N_INFINITY   (100.0 * STOP)      /* must be much larger than STOP  */

   double Min(double a, double c)
/* ------------------------------
 * return the smaller of a, b
 * ------------------------------
 */
{ 
  if (a < c)
    return (a);
  else
    return (c);
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
/* ---------------------------------------------
 * generate the next arrival time, with rate 1/2
 * ---------------------------------------------
 */ 
{
  SelectStream(0); 
  arrival += Exponential(60);
  return (arrival);
} 


   double GetService(double serviceTimeA, double serviceTimeB)
/* --------------------------------------------
 * generate the next service time with rate 2/3
 * --------------------------------------------
 */ 
{
  SelectStream(4);
  return (Uniform(serviceTimeA, serviceTimeB));
}  

 double GetServiceE(double serviceTime)
/* --------------------------------------------
 * generate the next service time with rate 2/3
 * --------------------------------------------
 */ 
{
  SelectStream(4);
  return (Exponential(serviceTime));
}  
  int main(void)
{

  struct Worker{
    double serviceTimeA;
    double serviceTimeB;
    double serviceTime;
    double perHourWage;
  };

  struct Worker Fran;
  Fran.serviceTimeA = 5;
  Fran.serviceTimeB = 35;
  Fran.serviceTime = 20;
  Fran.perHourWage = 12;
  struct Worker Bill;
  Bill.serviceTimeA = 15;
  Bill.serviceTimeB = 45;
  Bill.serviceTime = 30;
  Bill.perHourWage = 6;

  double priceOfCone = 3; 

  struct Worker employees[2] = {Fran,Bill};

  for(int i = 0; i < 1; i++){
    PlantSeeds(12346789);
    for(int CAPACITY = 1; CAPACITY < 6; CAPACITY ++){
      for(int employeeNum = 0; employeeNum < 2; employeeNum++){
        struct {
          double arrival;                 /* next arrival time                   */
          double completion;              /* next completion time                */
          double current;                 /* current time                        */
          double next;                    /* next (most imminent) event time     */
          double last;                    /* last arrival time                   */
        } t;
        struct {
          double node;                    /* time integrated number in the node  */
          double queue;                   /* time integrated number in the queue */
          double service;                 /* time integrated number in service   */
        } area      = {0.0, 0.0, 0.0};
        long index  = 0;                  /* used to count departed jobs         */
        long number = 0;                  /* number in the node                  */
        long reject = 0;                  /* used to count the number of rejected jobs  */
        
        t.current    = START;           /* set the clock                         */
        t.arrival = START;
        t.arrival    = GetArrival(t.arrival);    /* schedule the first arrival            */
        t.completion = N_INFINITY;        /* the first event can't be a completion */

        while (index < 1000000 && ((t.arrival < STOP) || (number > 0))) {
          t.next          = Min(t.arrival, t.completion);  /* next event time   */

          if (number > 0)  {                               /* update integrals  */
            area.node    += (t.next - t.current) * number;
            area.queue   += (t.next - t.current) * (number - 1);
            area.service += (t.next - t.current);
          }
          t.current       = t.next;                    /* advance the clock */

          if (t.current == t.arrival)  {               /* process an arrival */
            if (number < CAPACITY){
              number++;
              if (number == 1)
                t.completion = t.current + GetServiceE(employees[employeeNum].serviceTime);
                //t.completion = t.current + GetService(employees[employeeNum].serviceTimeA, employees[employeeNum].serviceTimeB);
            }
            else
              reject++;
            t.arrival = GetArrival(t.arrival);
            if (t.arrival > STOP){
              t.last = t.current;
              t.arrival = N_INFINITY;
            }
          }

          else {                                        /* process a completion */
            index++;

            if(index == 1000000){
              t.last      = t.current;
            }
            number--;
            if (number > 0)
              t.completion = t.current + GetServiceE(employees[employeeNum].serviceTime);
              //t.completion = t.current + GetService(employees[employeeNum].serviceTimeA, employees[employeeNum].serviceTimeB);
            else
              t.completion = N_INFINITY;
          }
        } 



        printf("\nfor %ld jobs\n", index);
        printf("\ncapacity %d \n", CAPACITY);
        printf("\n total time (h) %6.2f \n", ((t.last / 60)/60));
        printf("   average interarrival time = %6.2f\n", t.last / index);
        printf("   average wait ............ = %6.2f\n", area.node / index);
        printf("   average delay ........... = %6.2f\n", area.queue / index);
        printf("   average service time .... = %6.2f\n", area.service / index);
        printf("   average # in the node ... = %6.2f\n", area.node / t.current);
        printf("   average # in the queue .. = %6.2f\n", area.queue / t.current);
        printf("   utilization ............. = %6.2f\n", area.service / t.current);
        printf("   number of jobs rejected . = %ld\n", reject);
        printf("   money made .............. = %6.2f\n", index * priceOfCone);
        printf("   money lost .............. = %6.2f\n", reject * priceOfCone);
        printf("   job rejected probability. = %6.4f\n", (double) reject/(reject+index));
      }
    }
  }
  return (0);
}
