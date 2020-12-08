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
#include <unordered_map>

#include "utils.cpp"

enum InstructionType {NOP, ACC, JMP, ERR};

struct Instruction {
	InstructionType type;
	i32 operand;
};

InstructionType stringToInstruction(std::string_view str) {
	if (str == "nop") {
		return NOP;
	} else if (str == "acc") {
		return ACC;
	} else if (str == "jmp") {
		return JMP;
	}
	assert(0);
	return ERR;
}

void printInstruction(Instruction instruction) {
	switch(instruction.type) {
		case NOP:{
			printf("nop");
		} break;
		case ACC:{
			printf("acc");
		} break;
		case JMP:{
			printf("jmp");
		} break;
		case ERR:{
			printf("err");
		} break;
	}
	printf(" ");
	printf("%d", instruction.operand);
}

bool hasLoop(std::vector<Instruction>& instructions) {
	i32 accumulator = 0;
	std::vector<u8> executed(instructions.size());
	u64 programCounter = 0;

	while (1) {
		if (executed[programCounter]) {
			printf("already executed instruction %llu, current value of accumulator is %d\n", programCounter, accumulator);
			return true;
		}

		executed[programCounter] = 1;

		Instruction currInstruction = instructions[programCounter];
		switch(currInstruction.type) {
			case NOP:{
				// do nothing
				programCounter++;
			} break;
			case ACC:{
				accumulator += currInstruction.operand;
				programCounter++;
			} break;
			case JMP:{
				programCounter += currInstruction.operand;
			} break;
			default:{
				printf("invalid opcode\n");
				assert(0);
			} break;
		}

		if (programCounter == instructions.size()) {
			printf("program terminated. Value of the accumulator is %d\n", accumulator);
			return false;
		}
	}
}

InstructionType swapNopAndJmp(InstructionType in) {
	return (InstructionType)(2-(i32)in);
}

i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();
	auto input = loadInput("Day8_input.txt");

	std::vector<Instruction> instructions;

	while (peekByte(&input) != 0) {
		InstructionType type = stringToInstruction(getStringUntilByte(&input, {' '}));
		skipChars(&input, {' '});
		i32 operand = getInt(&input);
		expectByte(&input, '\n');
		instructions.push_back(Instruction{type, operand});
	}

	// part 1 - run unmodified
	printf("part 1: ");
	hasLoop(instructions);
	printf("\n");

	// part 2 - find a jump instruction that, when turned into a nop (or vice-versa), allows program to finish
	for (auto& i: instructions) {
		if (i.type != JMP && i.type != NOP) {
			continue;
		}
		i.type = swapNopAndJmp(i.type);
		if (!hasLoop(instructions)) {
			break;
		}
		i.type = swapNopAndJmp(i.type);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);
	return 0;
}