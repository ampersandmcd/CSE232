/*
 * Andrew McDonald
 * CSE 232.006 FS 18
 * Project 02 - K-Hyperperfect Numbers
 */

#include <iostream>
#include <cmath>

using std::cin; using std::cout; using std::endl; using std::sqrt; using std::floor;

int main() {
	long check = 0,												//integer entered which will be checked for k-hyperperfectness
		 k_max = 0;												//integer entered as an inclusive upper bound for k

	cin >> check >> k_max;										//store inputs

	if (check < 6 || k_max < 1) {
		cout << 0;												//no number less than 6 can ever be k-hyperperfect, and k must be at least one
	} else {
		long output = 0;										//will serve as the default output if no k is found such that check is k-hyperperfect; will be reassigned if check is k-hyperperfect for some k
		for (long k = 1; k <= k_max; ++k) {						//iterate through all k's less than or equal to k_max and check if k-hyperperfectness applies 
			long sum = 0;										//variable to store sum of all divisors of the number being checked; will reset with each k being checked
			for (long i = 2; i <= floor(sqrt(check)); ++i) {	//sum all divisors which are not 1 nor the number being checked
				if ((check % i == 0) && (i == (check / i))){
					sum += i;									//here we have a perfect square - only add divisor once, as divisor == factor
				} else if (check % i == 0) {
					sum += i + (check / i);						//here we add both the divisor and its factor on the same iteration to avoid redundancy and increase efficiency; we now must only check for divisors <= floor(sqrt(check))
				}
			}

			long result = sum * k + 1;							//multiply sum of divisors by k and add one to check for k-hyperperfectness

			if (result == check) {								//then, the integer input is k-hyperperfect for the k of this iteration; assign to output to print and break
				output = k;
				break;
			}
		}
		cout << output;
	}
	return 0;
}