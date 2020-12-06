#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <vector>
#include <chrono>

#include <iostream>

#include "utils.cpp"

i32 part1(Input *input) {
	i32 total = 0;
	while (*input->cursor != 0) {
		u32 answers = 0;
		while (*input->cursor != 0) {
			char b = getByte(input);
			if (b == '\n') {
				if (peekByte(input) == '\n') {
					input->cursor++;
					break;
				}
			} else {
				answers = answers | 1<<(b-'a');
			}
		}

		for (i32 i = 0; i < 26; i++) {
			total += (answers >> i) & 1;
		}
	}
	return total;
}

i32 part2(Input *input) {
	i32 total = 0;
	while (*input->cursor != 0) {
		u32 groupAnswers = 0xFFFFFFFF;
		while (*input->cursor != 0) {
			std::string_view person = getLine(input);
			if (person.length() == 0) {
				break;
			}
			u32 personAnswers = 0;
			for (char c: person) {
				personAnswers |= 1<<(c-'a');
			}
			groupAnswers &= personAnswers;
		}

		for (i32 i = 0; i < 26; i++) {
			total += (groupAnswers >> i) & 1;
		}
	}
	return total;
}


i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();
	auto input = loadInput("Day6_input.txt");

	printf("part1 = %d\n", part1(&input));
	input.cursor = input.data;

	printf("part2 = %d\n", part2(&input));

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);
	return 0;
}