/*
 * Generate a u and find the probabilties of the values it returns 
 * @author Andrew Katson
 */

#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "../rngs.h"


#define START 0
#define END 10000000


double Uniform(double a, double b)           
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{                                         
  return (a + (b - a) * Random());    
}


double generateValue(){
	return ceil(3+2*pow(Uniform(0,1),2));
}

int main(){

	for(int i = START; i < END; i++){
		printf("%6.2f", generateValue());
	}

	return 0;
}