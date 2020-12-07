#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <string>
#include <initializer_list>

typedef uint8_t u8;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;

void printBinary(u32 n) {
	for (i32 i = 31; i >= 0; i--) {
		u32 bit = (n & 1<<i) >> i;
		printf("%u", bit);
	}
}

struct Input {
	char *data;
	char *cursor;
	char *end; 
	u64 size;
};

Input loadInput(const char *fileName) {
	FILE* fileHandle = fopen(fileName, "r");
	fseek(fileHandle, 0, SEEK_END);
    
    Input input;
    input.size = ftell(fileHandle);
    input.data = input.cursor = (char*)malloc((input.size+1)*sizeof(char));
    input.end = input.cursor+input.size;

    rewind(fileHandle);
    fread(input.data, sizeof(char), input.size, fileHandle);
    fclose(fileHandle);

    input.data[input.size] = 0; // null terminate for convenience
    // fwrite(input.data, sizeof(char), input.size, stdout);

    return input;
}

void printInput(Input *input) {
	fwrite(input->data, sizeof(char), input->size, stdout);
}

char getAlpha(Input *input) {
	if (isalpha(*input->cursor)) {
		char out = *(input->cursor++);
		return out;
	} else {
		u64 position = input->cursor-input->data; 
		printf("Requested alpha. Byte at %llu is %02x, (%c).\n", position, *input->cursor, *input->cursor);
		assert(false);
		return 0;
	}
}

char peekAlpha(Input *input) {
	if (isalpha(*input->cursor)) {
		char out = *(input->cursor);
		return out;
	} else {
		u64 position = input->cursor-input->data; 
		printf("Requested alpha. Byte at %llu is %02x, (%c).\n", position, *input->cursor, *input->cursor);
		assert(false);
		return 0;
	}
}

char peekByte(Input *input) {
	char out = *(input->cursor);
	return out;
}

char getByte(Input *input) {
	char out = *(input->cursor++);
	return out;
}

void expectByte(Input *input, char expected) {
	if (*input->cursor != expected) {
		u64 position = input->cursor-input->data; 
		printf("Requested %02x (%c). Byte at %llu is %02x, (%c).\n", expected, expected, position, *input->cursor, *input->cursor);
		assert(false);
		return;
	}
	input->cursor++;
}

i32 getInt(Input *input) {
	if (!('0' <= *input->cursor && *input->cursor <= '9')) {
		u64 position = input->cursor-input->data; 
		printf("Requested i32. Byte at %llu is %02x, (%c).\n", position, *input->cursor, *input->cursor);
		assert(false);
		return 0;
	}

	i32 number = 0;
	while ('0' <= *input->cursor && *input->cursor <= '9') {
		number = number*10;
		number += (*input->cursor-'0');
		input->cursor++;
	}
	return number;
}

std::string_view getStringUntilByte(Input *input, std::initializer_list<char> skip) {
	u32 length = 0;
	while (std::find(skip.begin(), skip.end(), *(input->cursor+length)) == skip.end()) {
		length++;
	}
	std::string_view str(input->cursor, length);
	input->cursor += length;
	return str;
}

std::string_view getStringUntilString(Input *input, const char* string) {
	char *found = strstr(input->cursor, string);
	if (found != NULL) {
		u64 len = (found - input->cursor);
		std::string_view result(input->cursor, len);
		input->cursor = found;
		return result;
	} else {
		printf("Error in getStringUntilString, couldn't find string \"%s\".", string);
		assert(false);
	}
}

void skipChars(Input *input, std::initializer_list<char> l) {
	while (std::find(l.begin(), l.end(), *input->cursor) != l.end()) {
		input->cursor++;
	}
}

void expectStr(Input *input, const char *expectedString) {
	if (strncmp(input->cursor, expectedString, strlen(expectedString)) != 0) {
		u64 position = (input->cursor)-(input->data); 
		printf("Expected string \"%s\", but string at %llu is \"%.*s\".\n", expectedString, position, (i32)strlen(expectedString), input->cursor);
		assert(false);
	} else {
		input->cursor += strlen(expectedString);
	}
} 

bool isStr(Input *input, const char *queryString) {
	return (strncmp(input->cursor, queryString, strlen(queryString)) == 0);
}

std::string_view getAlphaNumString(Input *input) {
	u64 len = 0;
	while (isalnum(*(input->cursor+len))) {
		len++;
	}
	std::string_view str(input->cursor, len);
	input->cursor += len;
	return str;
}

std::string_view getDigitString(Input *input) {
	u64 len = 0;
	while (isdigit(*(input->cursor+len))) {
		len++;
	}
	std::string_view str(input->cursor, len);
	input->cursor += len;
	return str;
}

std::string getLine(Input *input) {
	u64 len = 0;
	while (*(input->cursor+len) != '\n') {
		++len;
	}
	std::string str(input->cursor, len);
	input->cursor += (len + 1);
	return str;
}

struct vec2 {
	i32 x;
	i32 y;

	vec2 operator+(vec2 b) {
		return vec2{x+b.x, y+b.y};
	}
	vec2& operator+=(vec2 b) {
		x += b.x;
		y += b.y;
		return *this;
	}
};

struct Grid {
	u32 w, h, stride, offsetX, offsetY;
	char *data;

	Grid(u32 _w, u32 _h, u32 _offsetX, u32 _offsetY)
	: w(_w), h(_h), stride(_w), offsetX(_offsetX), offsetY(_offsetY) {
		data = (char*)calloc(w*h, sizeof(char));
	}

	// Grid(char *_data)
	// : w(0), h(0), 
	// data(_data)

	~Grid() { free(data); }

	void insert(i32 x, i32 y, char val) {
		data[(offsetX + x) + w*(offsetY + y)] = val;
	}
	void insert(vec2 v, char val) {
		insert(v.x, v.y, val);
	}

	char get(i32 x, i32 y) {
		return data[(offsetX + x) + stride*(offsetY + y)];
	}
	char get(vec2 v) { return get(v.x, v.y); }

	char& operator()(i32 x, i32 y) {
		return data[(offsetX + x) + stride*(offsetY + y)];
	}
	char& operator()(vec2 v) {
		return operator()(v.x, v.y);
	}

	void operator=(std::initializer_list<char> l) {
		int i = 0;
		for (auto el: l) {
			data[i++] = el;
		}
	}
};

// Grid loadGrid(const char *fileName) {
// 	FILE* fileHandle = fopen(fileName, "r");
// 	fseek(fileHandle, 0, SEEK_END);
    
//     size = ftell(fileHandle);
//     data = cursor = (char*)malloc((input.size+1)*sizeof(char));
//     end = cursor+size;

//     // we assume that the input is actually a grid
//     u32 w = 0;
//     while (*cursor != '\n') {
//     	w++;
//     	cursor++;
//     }
    


//     rewind(fileHandle);
//     fread(input.data, sizeof(char), input.size, fileHandle);
//     fclose(fileHandle);

//     input.data[input.size] = 0; // null terminate for convenience
//     // fwrite(input.data, sizeof(char), input.size, stdout);

//     return input;
// }

void printGrid(Grid *grid) {
	for (int y = 0; y < grid->h; y++) {
		for (int x = 0; x < grid->w; x++) {
			printf("%d ", grid->data[x + grid->w*y]);
		}
		printf("\n");
	}
}