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

	auto input = loadInput("Day1_input.txt");

	std::vector<i32> values;

	while (input.cursor != input.end) {
		i32 i = getInt(&input);
		expectByte(&input, '\n');
		
		values.push_back(i);
	}

	for (int i = 0; i < values.size(); ++i) {
		for (int j = i+1; j < values.size(); j++) {
			if (values[i] + values[j] == 2020) {
				printf("%d + %d  = 2020, %d * %d = %d \n.", values[i], values[j], values[i], values[j], values[i]*values[j]);
			}
		}
	}

	for (int i = 0; i < values.size(); ++i) {
		for (int j = i+1; j < values.size(); j++) {
			for (int k = j+1; k < values.size(); k++)
			if (values[i] + values[j] + values[k] == 2020) {
				printf("%d + %d + %d  = 2020, %d * %d * %d = %d \n.", values[i], values[j], values[k], values[i], values[j], values[k], values[i]*values[j]*values[k]);
			}
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);

	return 0;
}