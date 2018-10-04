/*
 * This program calculates one full period multipler for a specified m. 
 * Then, it finds all the other full period multipliers.
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//the largest possible positive integer
const int LARGEST_INT = 0x00007FFF;


//Find the largest prime number less than or equal to LARGEST_INT
int findLargestPrime(){
	//since each bit in this array will be used to mark whether a number 
	//has been identified as non-prime we want the number of integers in 
	//the array to only be enough to store a 1/0 for each integer
	int slots = ceil(((double)LARGEST_INT) / 32.0);
	int * nums;	
	nums = (int *)malloc(sizeof(int) * slots);
	int i = 2;
	int icheck = i;

	//turn all the slots into 0
	for(int e = 0; e < slots; e++){
		nums[e] = 0;
	}

	//Sieve of Erasthoenes to find mark all non-primes 
	//in our array of bits
	while (i <= LARGEST_INT){
		//mark all numbers that are multiples of i
		for(int j = i+i; j <= LARGEST_INT; j+=i){

			//find and mark the bit corresponding to this 
			//multiple of i so that 
			//we know it is not a prime
			nums[j/32] |= (1<<(j%32));
			
		}
		
		int currSlot = i / 32;
		int currPos = i % 32;
		//set i to the next largest number that is not marked
		for(int p = currSlot; p < slots; p++){
			for(int w = 0; w < 32; w++){
				//if we are in the same slot as i and are 
				//a smaller number
				//we do not check
				if(p <= currSlot && w <= currPos){
					continue;
				}
				int check = nums[p] & (1 << w);
				if(check == 0){
					i = p * 32 + w;
					p = slots;
					break;
				}
			}
		}
		//if we do not find a number larger than the 
		//current largest prime we leave
		if(i == icheck){
			break;
		}
		else{
			icheck = i;
		}
	}

	int foundLargestPrime = 0;
	for(int z = slots - 1; z >=  0; z--){
		for (int j = 31; j >= 0; j--){
			int check = nums[z] & (1 << j);
			int currNum = i = z * 32 + j;
			if(check == 0){
				if(currNum <= LARGEST_INT){
					i = currNum;
					foundLargestPrime = 1;
					break;
				}
			}
		}
		if(foundLargestPrime){
			break;
		}
	}
	free(nums);
	return i;
}

//calculate all the full period multipliers and return a reference to an array of them
int * calcFullPeriodMultipliers(int m){
	int * nums = (int *)malloc(sizeof(int) * LARGEST_INT * 1/2);

	for(int i = 0; i < LARGEST_INT * 1/2; i++){
		nums[i] = 0;
	}


	int a = 2;
	int x = a;
	int numMultipliers = 0;
	while(a < m ){
		int p = 1;
		x = a;

		while(x != 1){
			p++;
			int y = x * a;
			//overflow catch
			if (x != 0 && y / x != a) {
			    break;
			}

			x = (x * a) % m;
		}
		//if a is a full period multiplier
		if(p == m - 1){
			//printf("%d, ",a);
			numMultipliers++;
			nums[numMultipliers] = a; 
		}
		a++;

	}
	//store the number of multipliers at the start of the array
	nums[0] = numMultipliers;

	return nums;
}

//return the greatest common divisor 
int gcd(int n1, int n2){
	int i, gcd;

    for(i=1; i <= n1 && i <= n2; ++i)
    {
        // Checks if i is factor of both integers
        if(n1%i==0 && n2%i==0)
            gcd = i;
    }

    return gcd;
}

//calculate what power i that a can be raised to before 
//it is larger than m. This will allow us
// to treat numbers that have higher powers of i but 
//the same remainder from modm the same.
// this reduces double counting and eliminates isseus with overflow.
int getPowerRepeating(int a, int m){
	int i = 0;

	int apow = a;

	while(apow < m){
		apow *= a;
		i++;
	}
	return i;
}

//calculate all the full period multipliers and add the exponent 
//they are raised to to the int array so 
//that any overflow their calculation would cause is avoided
int getAllFullPeriodMultiplers(int * arr, int firstFullPeriodMultiplier, 
	int m, int powerRepeating){
	//store the power of the first full period multiplier
	arr[1] = 1;
	int numFullPeriodMultipliers  = 1;

	int i = 1;
	int x = firstFullPeriodMultiplier;

	while(x != 1){

		if(gcd(i, m - 1) == 1){
			//mark it as used
			arr[i%powerRepeating] = 1;
			//we are counting them as unique even tho 
			//they are technically repeating
			numFullPeriodMultipliers++;
		}
		
		i++;
		int y = x * firstFullPeriodMultiplier;
		//overflow catch
		if (x != 0 && y / x != firstFullPeriodMultiplier) {
		    break;
		}
		x = (firstFullPeriodMultiplier * x ) % m;
	}
	return numFullPeriodMultipliers;
}


//return the number of modulus compatible full period multipliers and 
//set the int array of full period multipliers
// to only contain them and not every single full period multiplier
int getModulusCompatible(int * fullPeriodMultpliers, 
	double numUniqueFullPeriodMultpliers, double M){

	int index = 1;
	int writeover = 1;	//index of the current index to be written 
	//over (will lag behind the index of which 
    //full period multiplier is being checked)
	int numModulusCompatible = 0;
	while(index < numUniqueFullPeriodMultpliers){

		int currFullPeriodMultiplier = fullPeriodMultpliers[index];		
		double val = M / (double)(currFullPeriodMultiplier);
		int q = (int)floor(val);
		int r = ((int) M) % currFullPeriodMultiplier;

		
		//criteria for modulus compatability
		if(r < q){

			fullPeriodMultpliers[writeover] = currFullPeriodMultiplier;
			writeover++;
			numModulusCompatible++;
		}
		index++;
	}

	//set any remaining spots to zero that are not holding 
	//any modulus compatible full period multipliers
	while(writeover < index){
		fullPeriodMultpliers[writeover] = 0;
		writeover++;
	}

	return numModulusCompatible;
}

//print all modulus compatible full period multipliers
void printAllModulusCompatible(int * fullPeriodMultpliers, 
	int numModulusCompatible){
	for(int i = 1; i < numModulusCompatible - 1; i++){
		printf(" %d,", fullPeriodMultpliers[i]);
	}

	printf(" %d\n",
		fullPeriodMultpliers[numModulusCompatible - 1]);
}

int main(void){
	printf("Largest num %d\n", LARGEST_INT);

	int M = findLargestPrime();

	printf("The largest prime we found was %d\n", M);

	int * fullPeriodMultpliers = calcFullPeriodMultipliers(M);

	//grab the number of multipliers stored at the front of the array
	int numFullPeriodMultipliers = fullPeriodMultpliers[0];

	printf("There are %d full period multipliers\n", 
		numFullPeriodMultipliers);

	int numModulusCompatible = getModulusCompatible(fullPeriodMultpliers,
	 (double) numFullPeriodMultipliers, (double) M);

	printf("There are %d modulus compatible full period multipliers\n",
	 numModulusCompatible);

	printAllModulusCompatible(fullPeriodMultpliers, numModulusCompatible);

	free(fullPeriodMultpliers);

}