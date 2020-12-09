#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <vector>
#include <chrono>
#include <deque>

#include "utils.cpp"

bool isSumOfTwoValuesInCache(std::vector<u64>& cache, u64 value) {
	for (int i = 0; i < cache.size(); ++i) {
		for (int j = i+1; j < cache.size(); j++) {
			if (cache[i] + cache[j] == value) {
				return true;
			}
		}
	}
	return false;
}

i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();

	auto input = loadInput("Day9_input.txt");

	std::vector<u64> values;

	while (input.cursor != input.end) {
		i32 i = getInt(&input);
		expectByte(&input, '\n');
		
		values.push_back(i);
	}

	u64 cacheSize = 25;
	std::vector<u64> cache;
	cache.reserve(cacheSize);

	u64 i = 0;
	for (i = 0; i < cacheSize; ++i) {
		cache.push_back(values[i]);
	}

	for (; i < values.size(); ++i) {
		if (!isSumOfTwoValuesInCache(cache, values[i])) {
			printf("Part1: %llu\n", values[i]);
			break;
		}
		cache[i%cache.size()] = values[i];
	}

	u64 target = values[i]; 
	u64 targetIndex = i;

	printf("targetIndex = %llu\n", targetIndex);

	// TODO: @performance: just use a vector?
	std::deque<u64> sumSet;
	u64 currentSum = 0;
	for (u64 i = 0; i < targetIndex; i++) {
		sumSet.push_back(values[i]);
		currentSum += values[i];
		if (currentSum == target) {
			printf("current sum is equal to target\n");
			break;
		} else if (currentSum > target) {
			while (currentSum > target) {
				currentSum -= sumSet.front();
				sumSet.pop_front();
			}
			if (currentSum == target) {
				printf("current sum is equal to target\n");
				break;
			}
		}
	}

	u64 max = *std::max_element(sumSet.begin(), sumSet.end());
	u64 min = *std::min_element(sumSet.begin(), sumSet.end());

	printf("Part2: %llu\n", max+min);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);

	return 0;
}