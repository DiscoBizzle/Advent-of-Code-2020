#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <vector>
#include <chrono>

#include "utils.cpp"

i32 countChar(char c, std::string_view password) {
	i32 count = 0;
	for (char p: password) {
		if (c == p) {
			count++;
		}
	}
	return count;
}

bool validatePassword(char c, std::string_view password, int lower, int upper) {
	return ((password[lower-1] == c) && (password[upper-1] != c))
	    || ((password[lower-1] != c) && (password[upper-1] == c));
}


i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();

	auto input = loadInput("Day2_input.txt");

	i32 part1 = 0;
	i32 part2 = 0;

	while (input.cursor != input.end) {
		i32 lower = getInt(&input);
		expectByte(&input, '-');
		i32 upper = getInt(&input);
		skipChars(&input, {' '});
		char c = getAlpha(&input);
		expectByte(&input, ':');
		skipChars(&input, {' '});

		std::string_view password = getAlphaNumString(&input);

		i32 count = countChar(c, password);

		if (lower <= count && count <= upper) {
			part1++;
		}

		if (validatePassword(c, password, lower, upper)) {
			part2++;
		}

		expectByte(&input, '\n');
	}

	printf("number of valid passwords (part 1) = %d\n", part1);
	printf("number of valid passwords (part 2) = %d\n", part2);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);

	return 0;
}