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

u8 getElement(std::vector<std::string>& grid, i32 j, i32 i) {
	if (i < 0 || j < 0 || j >= grid.size() || i >= grid[0].size()) {
		return 0;
	} else {
		return grid[j][i];
	}
}

u8 countNeighbours(std::vector<std::string>& grid, i32 j, i32 i) {
	u8 numNeighbours = 0;
	numNeighbours += (getElement(grid, j-1, i-1) == '#');
	numNeighbours += (getElement(grid, j,   i-1) == '#');
	numNeighbours += (getElement(grid, j+1, i-1) == '#');
	numNeighbours += (getElement(grid, j-1, i)   == '#');
	numNeighbours += (getElement(grid, j+1, i)   == '#');
	numNeighbours += (getElement(grid, j-1, i+1) == '#');
	numNeighbours += (getElement(grid, j,   i+1) == '#');
	numNeighbours += (getElement(grid, j+1, i+1) == '#');
	return numNeighbours;
}

u8 countOccupiedSeatsSeenFrom(std::vector<std::string>& grid, i32 j, i32 i) {
	std::vector<vec2> dirs = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};

	u8 numSeen = 0;
	for (vec2 d: dirs) {
		vec2 inspected = vec2{i,j}+d;
		while (u8 seen = getElement(grid, inspected.y, inspected.x)) {
			if (seen == '#') {
				numSeen++;
				break;
			} else if (seen == 'L') {
				break;
			} else if (seen == '.') {
				inspected+=d;
				continue;
			}
		}
	}
	return numSeen;
}

u64 part1(std::vector<std::string> grid) {
	std::vector<std::string> nextGrid = grid;
	// nextGrid[0][0] = '#';
	u64 numIterations = 0;

	while (grid != nextGrid || numIterations == 0) {
		grid = nextGrid;
		for (int j = 0; j < grid.size(); j++) {
			for (int i = 0; i < grid[0].size(); i++) {
				if (grid[j][i] == 'L') {
					u8 numNeighbours = countNeighbours(grid, j, i);
					if (numNeighbours == 0) {
						nextGrid[j][i] = '#';
					}
				} else if (grid[j][i] == '#') {
					u8 numNeighbours = countNeighbours(grid, j, i);
					if (numNeighbours >= 4) {
						nextGrid[j][i] = 'L';
					}
				}
			}
		}
	
		numIterations++;
	}

	u64 numOccupied = 0;
	for (int j = 0; j < grid.size(); j++) {
		for (int i = 0; i < grid[0].size(); i++) {
			numOccupied += (grid[j][i] == '#') ? 1 : 0;
		}
	}
	return numOccupied;
}

u64 part2(std::vector<std::string> grid) {
	std::vector<std::string> nextGrid = grid;
	u64 numIterations = 0;

	while (grid != nextGrid || numIterations == 0) {
		grid = nextGrid;
		for (int j = 0; j < grid.size(); j++) {
			for (int i = 0; i < grid[0].size(); i++) {
				if (grid[j][i] == 'L') {
					u8 numNeighbours = countOccupiedSeatsSeenFrom(grid, j, i);
					if (numNeighbours == 0) {
						nextGrid[j][i] = '#';
					}
				} else if (grid[j][i] == '#') {
					u8 numNeighbours = countOccupiedSeatsSeenFrom(grid, j, i);
					if (numNeighbours >= 5) {
						nextGrid[j][i] = 'L';
					}
				}
			}
		}
	
		numIterations++;
	}

	u64 numOccupied = 0;
	for (int j = 0; j < grid.size(); j++) {
		for (int i = 0; i < grid[0].size(); i++) {
			numOccupied += (grid[j][i] == '#') ? 1 : 0;
		}
	}
	return numOccupied;
}

i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();

	auto input = loadInput("Day11_input.txt");

	std::vector<std::string> grid;

	while (input.cursor < input.end) {
		auto line = getLine(&input);
		grid.push_back(line);
	}

	u64 numOccupied = part1(grid);
	printf("part 1: numOccupied = %llu\n", numOccupied);

	numOccupied = part2(grid);
	printf("part 2: numOccupied = %llu\n", numOccupied);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);

	return 0;
}