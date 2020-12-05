#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <vector>
#include <chrono>

#include "utils.cpp"

void printGrid(std::vector<std::string>& grid) {
	for (int j = 0; j < grid.size(); j++) {
		for (int i = 0; i < grid[0].size(); i++) {
			printf("%c", grid[j][i]);
		}
		printf("\n");
	}
}

void printGrid(std::vector<std::string>& grid, vec2 position) {
	for (int j = 0; j < grid.size(); j++) {
		for (int i = 0; i < grid[0].size(); i++) {
			if (position.y == j && position.x == i) {
				if (grid[j][i] == '.') {
					printf("O");
				} else {
					printf("X");
				}
			} else {
				printf("%c", grid[j][i]);
			}
		}
		printf("\n");
	}
}

i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();

	auto input = loadInput("Day3_input.txt");

	std::vector<std::string> grid;

	while (input.cursor < input.end) {
		auto line = getLine(&input);
		grid.push_back(line);
	}

	std::vector<vec2> gradients = {{1,1},{3,1},{5,1},{7,1},{1,2}};


	std::vector<i32> numTrees(gradients.size());

	for (int i = 0; i < gradients.size(); i++) {

		vec2 position = gradients[i];

		while (position.y < grid.size()) {
			if (grid[position.y][position.x] == '#') {
				++numTrees[i];
			}

			position += gradients[i];
			position.x = position.x % grid[0].size();
		}
	}

	printf("number of trees encountered = \n");
	i64 product = 1;
	for (auto n: numTrees) {
		printf("%d, ", n);
		product *= n;
	}
	printf("\n");
	printf("product = %lld \n", product);


	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);

	return 0;
}