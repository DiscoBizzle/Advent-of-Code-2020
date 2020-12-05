#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <vector>
#include <chrono>

#include "utils.cpp"

i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();
	auto input = loadInput("Day5_input.txt");

	u8 seats [0b1111111111];
	u16 maxId = 0;

	while (*input.cursor != 0) {
		u16 currId = 0;
		for (i32 bit = 9; bit >= 0; bit--) {
			char b = getByte(&input);
			if (b == 'B' || b == 'R') {
				currId |= 1 << bit;
			}
		}

		expectByte(&input, '\n');
		seats[currId] = 1;

		if (currId > maxId) {
			maxId = currId;
		}
	}
	printf("maxId = %d\n", maxId);

	i32 mySeat = 0;
	for (i32 i = 1; i < 0b1111111110; ++i) {
		if (seats[i-1] == 1 && seats[i] == 0) {
			mySeat = i;
			break;
		}
	}
	printf("mySeat = %d\n", mySeat);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);
	return 0;
}