#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <vector>
#include <chrono>

#include "utils.cpp"

void processMove(vec2 *position, vec2 *direction, char instruction, i32 n) {
	switch (instruction) {
		case 'N': {
			*position += vec2{0,n};
		} break;
		case 'E': {
			*position += vec2{n,0};
		} break;
		case 'S': {
			*position += vec2{0,-n};
		} break;
		case 'W': {
			*position += vec2{-n,0};
		} break;
		case 'R': {
			rotateClockwise(direction, n);
		} break;
		case 'L': {
			rotateAntiClockwise(direction, n);
		} break;
		case 'F': {
			*position += (*direction) * n;
		} break;
	}
} 

void processWaypointMove(vec2 *shipPosition, vec2 *waypointPosition, char instruction, i32 n) {
	switch (instruction) {
		case 'N': {
			*waypointPosition += vec2{0,n};
		} break;
		case 'E': {
			*waypointPosition += vec2{n,0};
		} break;
		case 'S': {
			*waypointPosition += vec2{0,-n};
		} break;
		case 'W': {
			*waypointPosition += vec2{-n,0};
		} break;
		case 'R': {
			rotateClockwise(waypointPosition, n);
		} break;
		case 'L': {
			rotateAntiClockwise(waypointPosition, n);
		} break;
		case 'F': {
			*shipPosition += (*waypointPosition) * n;
		} break;
	}
} 

i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();

	auto input = loadInput("Day12_input.txt");

	vec2 currentPosition{0, 0};
	vec2 currentDirection{1, 0};

	while (input.cursor != input.end) {
		char c = getByte(&input);
		i64 i = getInt(&input);
		expectByte(&input, '\n');
		
		processMove(&currentPosition, &currentDirection, c, i);	
	}
	printf("Part1: position is {%d, %d}, manhattan distance is %d\n", currentPosition.x, currentPosition.y, abs(currentPosition.x) + abs(currentPosition.y));

	input.cursor = input.data;
	currentPosition = vec2{0, 0};
	vec2 waypointPosition = vec2{10, 1};

	while (input.cursor != input.end) {
		char c = getByte(&input);
		i64 i = getInt(&input);
		expectByte(&input, '\n');
		
		processWaypointMove(&currentPosition, &waypointPosition, c, i);

	}
	printf("Part2: position is {%d, %d}, manhattan distance is %d\n", currentPosition.x, currentPosition.y, abs(currentPosition.x) + abs(currentPosition.y));

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);

	return 0;
}