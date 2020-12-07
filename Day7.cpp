#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <vector>
#include <chrono>

#include <iostream>
#include <queue>
#include <unordered_set>

#include "utils.cpp"

struct Node {
	std::string_view color;
	std::vector<u32> downstreamIds;
	std::vector<u32> downstreamNumbers;
	std::vector<u32> upstreamIds;
	std::vector<u32> upstreamNumbers;
};

u32 getNodeIdForColor(std::vector<Node> *graph, std::string_view color) {
	u32 i;
	for (i = 0; i < graph->size(); ++i) {
		if ((*graph)[i].color == color) {
			return i;
		}
	}
	Node newNode;
	newNode.color = color;
	graph->push_back(newNode);
	return i; 
}

void fillInUpstream(std::vector<Node> *graph) {
	for (int up = 0; up < graph->size(); up++) {
		for (int j = 0; j < (*graph)[up].downstreamIds.size(); j++) {
			u32 dsId = (*graph)[up].downstreamIds[j];
			u32 dsN = (*graph)[up].downstreamNumbers[j];
			if(std::find((*graph)[dsId].upstreamIds.begin(), (*graph)[dsId].upstreamIds.end(), up) == (*graph)[dsId].upstreamIds.end()) {
				(*graph)[dsId].upstreamIds.push_back(up);
				(*graph)[dsId].upstreamNumbers.push_back(dsN);
			}
		}
	}
}

u32 countUpstreamNodes(std::vector<Node> graph, u32 startId) {
	std::queue<u32> queue;
	queue.push(startId);
	std::unordered_set<u32> visited;
	while (!queue.empty()) {
		u32 i = queue.front();
		visited.insert(i);
		queue.pop();
		for (u32 up: graph[i].upstreamIds) {
			if (visited.count(up) == 0) {
				queue.push(up);
			}
		}
	}
	return visited.size()-1;
} 

void printGraph(std::vector<Node> graph) {
	for (auto node: graph) {
		// printf("color = %s\n", node.color);
		std::cout<<node.color<<std::endl;
		printf("downstreamIds = ");
		for (auto i: node.downstreamIds) {
			printf("%d ",i);
		}
		printf("\n");
		printf("downstreamNumbers = ");
		for (auto i: node.downstreamNumbers) {
			printf("%d ", i);
		}
		printf("\n");
		printf("upstreamIds = ");
		for (auto i: node.upstreamIds) {
			printf("%d ", i);
		}
		printf("\n");
		printf("upstreamNumbers = ");
		for (auto i: node.upstreamNumbers) {
			printf("%d ", i);
		}
		printf("\n\n");
	}
}

u64 getTotalBagsIncluding(std::vector<Node> graph, u32 index) {
	u32 sum = 0;
	for (u32 i = 0; i < graph[index].downstreamIds.size(); i++) {
		sum += graph[index].downstreamNumbers[i]*getTotalBagsIncluding(graph, graph[index].downstreamIds[i]);
	}
	return sum+1;
}

i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();
	auto input = loadInput("Day7_input.txt");

	std::vector<Node> graph;

	while (peekByte(&input) != 0) {
		std::string_view color = getStringUntilString(&input, " bags contain ");
		u32 id = getNodeIdForColor(&graph, color);
		expectStr(&input, " bags contain ");
		while (peekByte(&input) != '\n') {
			if (isStr(&input, "no other bags.")) {
				input.cursor += strlen("no other bags.");
				break;
			}
			u32 num = getInt(&input);
			skipChars(&input, {' '});

			std::string_view downstreamColor = getStringUntilString(&input, " bag");
			u32 downstreamId = getNodeIdForColor(&graph, downstreamColor);

			graph[id].downstreamIds.push_back(downstreamId);
			graph[id].downstreamNumbers.push_back(num);

			expectStr(&input, " bag");
			skipChars(&input, {'s', ',', '.', ' '});
		}
		expectByte(&input, '\n');
	}

	fillInUpstream(&graph);

	printf("number of bags that can contain the gold bag is %u\n", countUpstreamNodes(graph, getNodeIdForColor(&graph, "shiny gold")));
	
	auto start = getNodeIdForColor(&graph, "shiny gold");
	printf("total number of bags contained in the shiny gold bag is %llu\n", getTotalBagsIncluding(graph, start)-1);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);
	return 0;
}