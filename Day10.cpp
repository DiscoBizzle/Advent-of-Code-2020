#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <vector>
#include <chrono>

#include "utils.cpp"

u64 arrangements(u64 n) {
	switch(n) {
		case 1: {
			return 1;
		} break;
		case 2: {
			return 2;
		} break;
		case 3: {
			return 4;
		} break;
		default:  {
			return arrangements(n-1) + arrangements(n-2) + arrangements(n-3);
		} break;
	}
}

i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();

	auto input = loadInput("Day10_input.txt");

	std::vector<i32> values;

	while (input.cursor != input.end) {
		i32 i = getInt(&input);
		expectByte(&input, '\n');
		
		values.push_back(i);
	}

	std::sort(values.begin(), values.end());

	values.push_back(values[values.size()-1]+3);

	std::vector<i32> differences = values;

	for (int i = differences.size()-1; i > 0; --i) {
		differences[i] -= values[i-1];
	}

	u32 distribution[3] = {0, 0, 0};
	
	for(auto d: differences) {
		printf("%d, ", d);
	}
	printf("\n");

	for (auto d: differences) {
		if (1 <= d && d <= 3) {
			distribution[d-1]++;
		} else {
			printf("invalid difference %d\n", d);
			assert(false);
		}
	}

	printf("num differences = %u, %u, %u\n", distribution[0], distribution[1], distribution[2]);
	printf("d(1) * d(2) = %u\n", distribution[0]*distribution[2]);

	// part 2:

	// notice that in our list of differences, we only have 1's and 3's. This simplifies the problem.

	// we have strings of difference 3 separated by strings of difference 1
	// there is only one way of bridging a gap of 3, so we can treat the strings of 1 as independent.
	// We want to compute the number of ways of rearranging the strings of difference 1
	// we can then multiply the number of ways of rearranging each string together.

	// suppose we have the connectors n, n+1, n+2, ..., n+m
	// denote the number of ways of bridging the gap between n and n+m as w[m]
	// starting at m, our options are:
	//     - attach n+1, leaving us with a string of length m-1
	//     - attach n+2, leaving us with a string of length m-2 
	//     - attach n+2, leaving us with a string of length m-3 

	// so w[m] = w[m-1] + w[m-2] + w[m-3];

	// The base cases are:
    
    // n-3    n    n+1   n+3
	//     3     1     3      // differences
	// we can't delete any of these, because then the difference would become greater than w[1] = 1;
	// 
	// n-3   n   n+1   n+2   n+5
	//     3   1     1     3          
    // we can either leave both intact, or we can delete n+1, leaving a gap of 2, everything else is invalid (2 ways) w[2] = 2;
    //
	// n-3   n   n+1   n+2   n+3   n+5
	//     3   1     1     1     3          
    // we can delete n+1, n+2, both or neither w[3] = 4;
   
   	// from these we can build up all values of w

	u64 combinations = 1;
	u64 currentRunLength = 0;
	for (auto d: differences) {
		if (d != 1) {
			if (currentRunLength > 0) {
				printf("run length = %llu\n", currentRunLength);
				printf("number of combinations from that group = %llu\n", arrangements(currentRunLength));
				combinations *= arrangements(currentRunLength);
				currentRunLength = 0;
			}
		} else {
			currentRunLength++;
		}
	}

	printf("number of arrangements = %llu\n", combinations);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);

	return 0;
}