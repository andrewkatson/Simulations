/*
 * Implementation for a constrained invesion of a Binomial(100,0.1) to a range of a,b 
 * @author Andrew Katson
 */

#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "rvms.h"
#include "rvgs.h"

#define DATAPOINTS 1000000 //the number of random vairates we want 



long truncatedBinomial(double alpha, double beta){
	double u = Uniform(alpha, 1-beta);
	return idfBinomial(100, 0.1, u);

}

//a function to generate binomials within the truncated range a,b
//also calcualtes their mean and standard deviation 
void evaluateTruncatedBinomial(){
	int a = 4;
 	int b = 16;

 	double alpha = cdfBinomial(100, 0.1, a-1);
 	double beta = 1 - cdfBinomial(100, 0.1, b);

 	long * truncatedArray = malloc(sizeof(long) * DATAPOINTS);

 	int count = 0;
 	double sum = 0;
 	while(count < DATAPOINTS){
 		long truncatedRandom = truncatedBinomial(alpha, beta);
 		sum+=truncatedRandom;
 		truncatedArray[count] = truncatedRandom;
 		printf("%ld\n", truncatedRandom); 
 		count++;
 	}

 	/*
 	//get the mean 
 	double mean = sum/count;

 	//calcualte the standard deviation 
 	double sumOfSquareDifferences = 0.0;

	for(int i = 0; i < count; i++){
		sumOfSquareDifferences += pow((truncatedArray[i] - mean), 2);
	}

	double standardDeviation = sqrt((sumOfSquareDifferences/count));

	printf("Truncated Binomial Distribtuion \n");
	printf("Mean....................%6.2f\n", mean);
	printf("Standard Deviation......%6.2f\n", standardDeviation);
	*/
 	free(truncatedArray);
 	truncatedArray = NULL;
}


//a function to generate binomials in the original range 
//also calculates their mean and standard deviation 
void evaluateNormalBinomial(){

	long * normalArray = malloc(sizeof(long) * DATAPOINTS);

	int count = 0;
	double sum = 0;
	while(count < DATAPOINTS){
		long binomialRandom = Binomial(100, 0.1);
		sum+=binomialRandom;
		normalArray[count] = binomialRandom;
		//printf("%ld\n", binomialRandom);
		count++;
	}
	/*
	//calculate the mean 
	double mean = sum/count;

	//calculate the standard deviation 
	double sumOfSquareDifferences = 0.0;

	for(int i = 0; i < count; i++){
		sumOfSquareDifferences += pow((normalArray[i] - mean), 2);
	}

	double standardDeviation = sqrt((sumOfSquareDifferences/count));

	printf("Not Truncated Binomial Distribtuion \n");
	printf("Mean....................%6.2f\n", mean);
	printf("Standard Deviation......%6.2f\n", standardDeviation);
	*/
	free(normalArray);
	normalArray = NULL;
}

 int main(){
 	evaluateTruncatedBinomial();

 	evaluateNormalBinomial();

 	return 0;
 }