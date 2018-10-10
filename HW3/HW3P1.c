/*This program uses a Monte Carlo simulation to calculate 
 *the probability that given a $10 bill is picked 
 * from a compartment that the other compartment
 * is also a $10
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include "../../rng.c"

const int MAX_SIM = 0x0FFFFFFF;
int simnum = 0; //number of the simulation
int sim10  = 0; //number of times we get a box with a $10 and a compartment with a $10
int sim1010 = 0; //number of times we get a $10 in one compartment with a $10 in the other compartment
long Equilikely (long a, long b){
	return (a+ (long) ((b-a+ 1) * Random()));
}


int main(){
	time_t t;


	for(int i = 0; i < 3; i++){
		//initialize random number generator
		PutSeed(time(&t));

		while(simnum < MAX_SIM){
			long box = Equilikely(0, 2);
			long compartment = Equilikely(0, 1);

			if(box == 1){
				sim10++;
				sim1010++;
			}
			else if(box == 2){
				if(compartment == 0){
					sim10++;
				}
			}
			simnum ++;

		}
		float probability = (float) sim1010 / sim10;
		printf("The probability was %6.6f\n", probability);
		simnum = 0;
		sim10 = 0;
		sim1010 = 0;
	}
}