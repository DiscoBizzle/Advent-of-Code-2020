#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <vector>
#include <chrono>

#include<iostream>

#include "utils.cpp"

enum PassportField {byr, iyr, eyr, hgt, hcl, ecl, pid, cid, invalid};

PassportField stringToField(std::string_view field) {
	if (field == "byr") {
		return byr;
	} else if (field == "iyr") {
		return iyr;
	} else if (field == "eyr") {
		return eyr;
	} else if (field == "hgt") {
		return hgt;
	} else if (field == "hcl") {
		return hcl;
	} else if (field == "ecl") {
		return ecl;
	} else if (field == "pid") {
		return pid;
	} else if (field == "cid") {
		return cid;
	} else {
		std::cout<<"Unrecognized field "<<field<<std::endl;
		assert(false);
		return invalid;
	}
}

u8 countFields(Input *input) {
	u8 setFields = 0;

	while (*input->cursor != 0) {
		std::string_view fieldString = getAlphaNumString(input);

		PassportField field = stringToField(fieldString);

		setFields |= (1 << (u32)field);

		expectByte(input, ':');

		std::string_view entry = getStringUntilByte(input, {' ', '\n'});
		input->cursor++;
		if (peekByte(input) == '\n') {
			input->cursor++;
			return setFields;
		}
	}
	return setFields;
}

bool isValidHex(std::string_view hex) {
	if (hex.length() != 6) {
		return false;
	}
	for (char c: hex) {
		if (!isxdigit(c)) {
			return false;
		}
	}
	return true;
}

bool isValidEyeColor(std::string_view eye) {
	return ((eye == "amb") || (eye == "blu") || (eye == "brn") || (eye == "gry") || (eye == "grn") || (eye == "hzl") || (eye == "oth"));
}

bool isInt(std::string_view entry) {
	for (char c: entry) {
		if (!isdigit(c)) {
			return false;
		}
	}
	return true;
} 

i32 getInt(std::string_view entry) {
	i32 i = 0;  
	for (char c: entry) {
		i *= 10;
		i += (c - '0');
	}
	return i;
}

bool isValidDateBetween(std::string_view entry, i32 lower, i32 upper) {
	if (!isInt(entry)) {
		return false;
	}
	i32 date = getInt(entry);
	return (lower <= date && date <= upper);
}

bool validateEntry(PassportField field, std::string_view entry) {
	switch(field) {
		case byr: {
			return isValidDateBetween(entry, 1920, 2020);
		} break;
		case iyr: {
			return isValidDateBetween(entry, 2010, 2020);
		} break;
		case eyr: {
			return isValidDateBetween(entry, 2020, 2030);
		} break;
		case hgt: {
			if (!isInt(entry.substr(0, entry.length()-2))) {
				return false;
			}
			i32 height = getInt(entry.substr(0, entry.length()-2));

			std::string_view unit = entry.substr(entry.length()-2, 2); 
			if (unit == "cm") {
				return (150 <= height && height <= 193);
			} else if (unit == "in") {
				return (59 <= height && height <= 76);
			}
			return false;
		} break;
		case hcl: {
			if (entry[0] == '#') {
				return isValidHex(entry.substr(1, entry.length()-1));
			}
			return false;
		} break;
		case ecl: {
			return isValidEyeColor(entry);
		} break;
		case pid: {
			return isInt(entry) && entry.length() == 9;
		} break;
		case cid: {
			return true; // anything is valid here
		} break;
		default: {
			return false;
		} break;
	}
	return false;
}

u8 validatePassport(Input *input) {
	u8 validFields = 0;

	while (*input->cursor != 0) {
		std::string_view fieldString = getAlphaNumString(input);

		PassportField field = stringToField(fieldString);
		expectByte(input, ':');
		std::string_view entry = getStringUntilByte(input, {' ', '\n'});
		input->cursor++;

		if (validateEntry(field, entry)) {
			validFields |= (1 << (u32)field);
		}

		if (peekByte(input) == '\n') {
			input->cursor++;
			return validFields;
		}
	}
	return validFields;
}

i32 main(i32 argc, char const *argv[]) {
	auto begin = std::chrono::high_resolution_clock::now();

	auto input = loadInput("Day4_input.txt");

	u32 validPassports = 0;
	while (*(input.cursor) != 0) {
		u8 validFields = validatePassport(&input);
		if (validFields == 0b11111111 || validFields == 0b01111111) {
			validPassports++;
		}
	}

	printf("number of valid passports = %d\n", validPassports);

	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.3f miliseconds.\n", elapsed.count() * 1e-6);

	return 0;
}