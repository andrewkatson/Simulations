/*
 * Monte Carlo simulation to simulate the probability of different test scores for a given set of test questions 
 * that are part of a fictional set of standaradized test questions. The questions fall into two types Class I and Class II. 
 * There were 90 Class I and 30 Class II problems. The student had a higher probability of getting a good grade on 
 * problems listed in the set of Class I problems. 
 * @author Andrew Katson
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include "string.h"
#include "math.h"
#include "../rngs.h"

#define START 0 
#define END 100000  //number of simulated test sets 
#define MAXQUESTIONSPICKED 12 //number of questions to select for each run
#define PASSING 36 //the score needed to pass 

double Equilikely (double a, double b){
  /* ---------------------------------------------------
   * generate an Equilikely random variate, use a < b
   * ---------------------------------------------------
   */
	return (a+ (long) ((b-a+ 1) * Random()));
}

/*
 * @return int[]: an array with test questions indicated by 1 and 2 (for classes 1 and 2 type questions)
 */
int *  setUpTestQuestions(){

	static int questions[120];

	//the total number of class I questions we want
	int classIMax = 90; 
	//the toatl number of class II questions we want 
	int classIIMax = 30;

	for(int i = 0; i < 120; i++){
		if (classIMax >0){
			questions[i] = 0;
			classIMax--;
		}
		else if(classIIMax > 0){
			questions[i] = 1;
			classIIMax--;
		}
		else{
			printf("Question error");
		}
	}
	return questions;
}

/*
 * @return int (0 is false, >0 is true): whether the current index has been used 
 */
int isSelected(int randQuestionIndex, int usedSpaces[4]){

	int usedSpacesIndex = randQuestionIndex/32;
	int usedSpacesSlot = randQuestionIndex%32;
	return ((1 << usedSpacesSlot) & usedSpaces[usedSpacesIndex]);
}


int main(){

	//counters for passing and failing grades 
	int failingGrades =0;
	int passGrades = 0;

	//the the grades for each student for each type of question
	int classIGrades[10] = {4,4,4,4,4,4,3,3,3,2};
	int classIIGrades[10] = {3,2,2,2,2,1,1,1,1,0};
	int classGrades[2][10] = {{4,4,4,4,4,4,3,3,3,2}, {3,2,2,2,2,1,1,1,1,0}};

	//setup an array with 90 questions of type I and 30 questions of type 2
	int * questions = setUpTestQuestions();

	time_t t;
	for(int simulation = 0; simulation < END; simulation++){
		SelectStream(100);
		//since this takes no time we multiply the current seed to be a new seed
		double multiplyBy = Equilikely(0.1,100);
		PutSeed(time(&t) * multiplyBy);

		//each bit will mark whether we have used an index of the questions array (i.e. already selected the problem)
		int usedSpaces[4] = {0};
		//the cumulative score 
		long cumulativeScore = 0;

		for(int pickQuestion = 0; pickQuestion < MAXQUESTIONSPICKED; pickQuestion++){

			//used to select the index of a random question 
			int randQuestionIndex;
			//select a differnt stream for pickinga question 
			SelectStream(2*pickQuestion);
			//loop until we get an unused problem
			do{	
				randQuestionIndex = (int) Equilikely(0, 119);
			}while(isSelected(randQuestionIndex, usedSpaces) != 0);

			//mark the problem as used 
			int usedSpacesIndex = randQuestionIndex/32;
			int usedSpacesSlot = randQuestionIndex%32;
			usedSpaces[usedSpacesIndex] |= (1<<usedSpacesSlot);
			
			//the class of the problem
			int problemClass = questions[randQuestionIndex];

			//select a differnt stream for picking a grade
			SelectStream(2*pickQuestion+1);
			//generate a random number to select the grade from this problem 
			int gradeIndex = (int) Equilikely(0,9);

			//add the grade to the tally 
			cumulativeScore+= classGrades[problemClass][gradeIndex];

		}
		
		printf("%ld\n", cumulativeScore); 

		//used to calculate the probability and check 
		if(cumulativeScore >= PASSING){
			passGrades++;
		}
		else{
			failingGrades++;
		}
	}

	printf("probability of passing %6.2f\n",(double) passGrades / END );
	printf("probability of failing %6.2f\n",(double)  failingGrades / END);
}
