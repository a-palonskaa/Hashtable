#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <cstdint>
#include "parse_text.h"
#include "logger.h"

ssize_t find_file_size(FILE* input_file) {
    assert(input_file != nullptr);

    struct stat file_data = {};
    if (fstat(fileno(input_file), &file_data) == -1) {
        LOG(ERROR, "FAILED TO READ THE FILE\n" STRERROR(errno));
        return -1;
    }

    if (file_data.st_size == 0) {
        LOG(WARNING, "EMPTY FILE\n" STRERROR(errno));
    }

    LOG(INFO, "FILE SIZE: %zu\n", file_data.st_size);
    return (ssize_t) file_data.st_size;
}

ssize_t parse_text(FILE* istream, text_t* text) {
    assert(istream != nullptr);
    ssize_t file_size = find_file_size(istream);

    if (file_size == -1) {
        return -1;
    }

    text->text = (word_t*) calloc((size_t) file_size, sizeof(word_t));
    if (text->text == nullptr) {
        return -1;
    }
    text->size = (size_t) file_size;

    text->buffer = (char*) calloc((size_t) file_size, sizeof(word_t));
    if (text->buffer == nullptr) {
        return -1;
    }

    size_t bytes_read = fread(text->buffer, 1, (size_t) file_size, istream);
    if (bytes_read != (size_t)file_size) {
        return -1;
    }
    text->buffer[file_size] = '\0';

    size_t word_index = 0;
    char* word_start = NULL;
    for (size_t i = 0; i < (size_t) file_size; i++) {
        if (!isspace(text->buffer[i])) {
            if (word_start == NULL) {
                word_start = text->buffer + i;
            }
        } else {
            if (word_start != NULL) {
                text->text[word_index].word = (const char*) word_start;
                text->text[word_index].len = (uintptr_t) text->buffer + i - (uintptr_t) word_start;
                *(word_start + text->text[word_index].len) = '\0';
                word_index++;
                word_start = NULL;
            }
        }
    }

    text->text = (word_t*) realloc(text->text, word_index * sizeof(word_t));
    text->size = word_index;
    return (ssize_t) word_index;
}
