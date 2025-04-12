#ifndef PARSE_TEXT_H
#define PARSE_TEXT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const char* word;
    size_t len;
} word_t;

typedef struct {
    char* buffer;
    word_t* text;
    size_t size;
} text_t;

ssize_t parse_text(FILE* istream, text_t* text);
ssize_t find_file_size(FILE* input_file);

#endif /* PARSE_TEXT_H */
