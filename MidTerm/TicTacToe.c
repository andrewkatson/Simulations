/*
 * Monte-Carlo simulation of the game tic tac toe with two players placing at random
 * Included is a calculated axiomatic probability for tic tac toe
 * @author Andrew Katson
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>  
#include "string.h"
#include "math.h"
#include "../rngs.h"

#define START 0 
#define END 10000000  //number of simulated games 

double Equilikely (double a, double b){
  /* ---------------------------------------------------
   * generate an Equilikely random variate, use a < b
   * ---------------------------------------------------
   */
	return (a+ (long) ((b-a+ 1) * Random()));
}

//return 'n' if no one has won on a diagonal and either 'x' or 'o' if a team did 
char winDiagonal(char board[3][3]){
	char diag = board[0][0];
	if (diag == board[1][1] && diag == board[2][2]){
		return diag;
	}
	diag = board[0][2];
	if(diag == board[1][1] && diag == board [2][0]){
		return diag;
	}
	return 'n';
}

//return 'n' if no one has won on a vertical and either 'x' or 'o' if a team did 
char winVertical(char board[3][3]){
	for(int col = 0; col < 3; col++){
		char colchar = board[0][col];
		int matchingchars = 0;
		if(colchar == 'n'){
			continue;
		}
		for(int row = 0; row < 3; row++){
			if(colchar == board[row][col]){
				matchingchars++;
			}
		}
		if(matchingchars == 3){
			return colchar;
		}
	}
	return 'n';
}

//return 'n' if no one has won on a vertical and either 'x' or 'o' if a team did 
char winHorizontal(char board[3][3]){
	for(int row = 0; row < 3; row++){
		char rowchar = board[row][0];
		if(rowchar == 'n'){
			continue;
		}
		int matchingchars = 0;
		for(int col = 0; col < 3; col++){
			if(rowchar == board[row][col]){
				matchingchars++;
			}
		}
		if(matchingchars == 3){
			return rowchar;
		}
	}
	return 'n';
}

//change the turn to the other team 
char newTurn(char oldTurn){
	return 'x' == oldTurn ? 'o' : 'x';
}

//check for a win and return the character of the winning side if there is one
//if not then return 'n'
char checkWin(char board[3][3]){
	char potentialWin = winHorizontal(board);

	//check for a win in any direction
	//if there is none check if the board is full
	//if it is then you count this as a tie
	if(potentialWin == 'n'){
		potentialWin = winVertical(board);
		if(potentialWin == 'n'){
			potentialWin = winDiagonal(board);
		}
	}
	return potentialWin;
}

//print the statistics from the passed game tallies 
void printStats(int numXWins, int numOWins, int numTies, int games){
	printf("\nFor %d games\n", games);
	printf("\tNumber of Ties %d\n", numTies);
	printf("\tNumber of X Wins %d\n", numXWins);
	printf("\tNumber of O Wins %d\n", numOWins);
	printf("\tProbabiltiy of Tie %6.4f\n", (double)numTies/games);
	printf("\tProbabiltiy of X Win %6.4f\n", (double)numXWins/games);
	printf("\tProbabiltiy of O Win %6.4f\n", (double)numOWins/games);
}


//play random tic tac toe games over and over with multiple different seeds 
void simTicTacToe(){
	time_t t;

	printf("Simulated Games: \n\n");

	//run three times with different seeds
	for(int seed = 0; seed < 3; seed++){
		PutSeed(time(&t));
		double stream1 = (double)2*seed;
		double stream2 = (double)2*seed+1;

		//ties, and wins by either player
		int numTies = 0;
		int numXWins = 0;
		int numOWins = 0;

		//games played 
		int games = 0;
		//run a set number of simulated games 
		for(int game = START; game < END; game++){
			games++;
			//always starts out as x's turn 
			char turn = 'x';
			//board for tic tac toe where 'n' indicates no placement 
			char board[3][3] = {{'n','n','n'}, {'n','n','n'}, {'n','n','n'}};

			//let there be nine placements on the board (then the board is full)
			for(int placements = 0; placements < 9; placements++){

				//use different streams for the random row and column chosen to 
				//make them independent
				SelectStream(stream1);
				int randRow = (int) Equilikely(0,2);
				SelectStream(stream2);
				int randCol = (int) Equilikely(0,2);

				//pick a random spot until we find a random row and column space that is unoccupied 
				while(board[randRow][randCol] != 'n'){	
					SelectStream(stream1);
					randRow = (int) Equilikely(0,2);
					SelectStream(stream2);
					randCol = (int) Equilikely(0,2);

				}

				//place the piece 
				board[randRow][randCol] = turn;

				char potentialWin = checkWin(board);

				if(potentialWin == 'n' && placements == 8){
					numTies++;
					break;
				}
				if(potentialWin == 'x'){
					numXWins++;
					break;
				}
				if(potentialWin == 'o'){
					numOWins++;
					break;
				}

				//change the turns
				turn = newTurn(turn);
			}
		}


		printStats(numXWins, numOWins, numTies, games);
	}
}

//calculate the axiomatic probability of x win, o win, and tie by generating all possible boards 
void calcAxiomaticProbability(){
	//ties, and wins by either player in the number of total moves to that point they won 
	int numTies = 0;
	int numXWins5Moves = 0;
	int numOWins6Moves = 0;
	int numXWins7Moves = 0;
	int numOWins8Moves = 0;
	int numXWins9Moves = 0;

	//games played 
	int games = 0;

	//board for tic tac toe where 'n' indicates no placement 
	char board[3][3] = {{'n','n','n'}, {'n','n','n'}, {'n','n','n'}};
	for(int firstX = 0; firstX < 9; firstX++){
		int firstXrow = firstX / 3;
		int firstXcol = firstX % 3;

		board[firstXrow][firstXcol] = 'x';

		for(int firstO = 0; firstO < 9; firstO++){

			int firstOrow = firstO / 3;
			int firstOcol = firstO % 3;

			if(board[firstOrow][firstOcol] != 'n'){
				continue;
			}

			board[firstOrow][firstOcol] = 'o';

			for(int secondX = 0; secondX < 9; secondX++){
				int secondXrow = secondX / 3;
				int secondXcol = secondX % 3;

				if(board[secondXrow][secondXcol] != 'n'){
					continue;
				}

				board[secondXrow][secondXcol] = 'x';

				for(int secondO = 0; secondO < 9; secondO++){

					int secondOrow = secondO / 3;
					int secondOcol = secondO % 3;

					if(board[secondOrow][secondOcol] != 'n'){
						continue;
					}

					board[secondOrow][secondOcol] = 'o';

					for(int thirdX = 0; thirdX < 9; thirdX++){

						int thirdXrow = thirdX / 3;
						int thirdXcol = thirdX % 3;

						if(board[thirdXrow][thirdXcol] != 'n'){
							continue;
						}

						board[thirdXrow][thirdXcol] = 'x';

						//we check a win after the thirdX is placed to avoid continuing to place 
						char potentialWin = checkWin(board);

						if(potentialWin == 'x'){
							numXWins5Moves++;
							games++;
							//we want to reset the board for the next iteration 
							board[thirdXrow][thirdXcol] = 'n';
							continue;
						}

						for(int thirdO = 0; thirdO < 9; thirdO++){
							int thirdOrow = thirdO / 3;
							int thirdOcol = thirdO % 3;

							if(board[thirdOrow][thirdOcol] != 'n'){
								continue;
							}

							board[thirdOrow][thirdOcol] = 'o';

							//we check a win after the thirdO is placed to avoid continuing to place 
							potentialWin = checkWin(board);

							if(potentialWin == 'o'){
								numOWins6Moves++;
								games++;
								//we want to reset the board for the next iteration 
								board[thirdOrow][thirdOcol] = 'n';
								continue;
							}
							for(int fourthX = 0; fourthX < 9; fourthX++){
								int fourthXrow = fourthX / 3;
								int fourthXcol = fourthX % 3;

								if(board[fourthXrow][fourthXcol] != 'n'){
									continue;
								}

								board[fourthXrow][fourthXcol] = 'x';

								//we check a win after the fourthX is placed to avoid continuing to place 
								potentialWin = checkWin(board);

								if(potentialWin == 'x'){
									numXWins7Moves++;
									games++;
									//we want to reset the board for the next iteration 
									board[fourthXrow][fourthXcol] = 'n';
									continue;
								}

								for(int fourthO = 0; fourthO < 9; fourthO++){
									int fourthOrow = fourthO / 3;
									int fourthOcol = fourthO % 3;

									if(board[fourthOrow][fourthOcol] != 'n'){
										continue;
									}

									board[fourthOrow][fourthOcol] = 'o';

									//we check a win after the fourthO is placed to avoid continuing to place 
									potentialWin = checkWin(board);

									if(potentialWin == 'o'){
										numOWins8Moves++;
										games++;
										//we want to reset the board for the next iteration 
										board[fourthOrow][fourthOcol] = 'n';
										continue;
									}
									for(int fifthX = 0; fifthX < 9; fifthX++){
										int fifthXrow = fifthX / 3;
										int fifthXcol = fifthX % 3;

										if(board[fifthXrow][fifthXcol] != 'n'){
											continue;
										}

										board[fifthXrow][fifthXcol] = 'x';

										//we check a win after the fifthX is placed to avoid continuing to place 
										potentialWin = checkWin(board);

										if(potentialWin == 'x'){
											numXWins9Moves++;
											games++;
										}
										//check for a tie if this is the last placement 
										if(potentialWin == 'n'){
											numTies++;
											games++;
										}

										//we want to reset the board for the next iteration 
										board[fifthXrow][fifthXcol] = 'n';
									}
									//we want to reset the board for the next iteration 
									board[fourthOrow][fourthOcol] = 'n';
								}
								//we want to reset the board for the next iteration 
								board[fourthXrow][fourthXcol] = 'n';
							}
							//we want to reset the board for the next iteration 
							board[thirdOrow][thirdOcol] = 'n';
						}
						//we want to reset the board for the next iteration 
						board[thirdXrow][thirdXcol] = 'n';
					}
					//we want to reset the board for the next iteration 
					board[secondOrow][secondOcol] = 'n';
				}
				//we want to reset the board for the next iteration 
				board[secondXrow][secondXcol] = 'n';
			}
			//we want to reset the board for the next iteration 
			board[firstOrow][firstOcol] = 'n';
		}
		//we want to reset the board for the next iteration 
		board[firstXrow][firstXcol] = 'n';
	}

	printf("\n\nAxiomatic Probability Calculations\n\n");

	//weight the moves by the number of moves remaining on the win
	int numXWins = numXWins5Moves*4*3*2*1 + numXWins7Moves*2*1 + numXWins9Moves * 1;
	int numOWins = numOWins6Moves*3*2*1 + numOWins8Moves*1;

	//we excluded games where one side won more quickly and then weighted the games 
	//that ended more quickly. so we to include every possible game
	int allPossibleGames = 9*8*7*6*5*4*3*2*1;

	printStats(numXWins, numOWins, numTies, allPossibleGames);
}

int main(void){

	simTicTacToe();

	calcAxiomaticProbability();

	return 0;
}