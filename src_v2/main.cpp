#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "hashtable_v2.h"
#include "logger.h"
#include "parse_text.h"

const char* LOGGER_FILE = "logs/logger_ptrs.html";
const char* TEXT_FILE = "data/meow.txt";
const char* WORDS_FILE = "data/words.txt";
const size_t HASHTABLE_SIZE = 600;             // load factor = 19

int main() {
    FILE* ostream = fopen(LOGGER_FILE, "wb");
    if (ostream == nullptr) {
        LOG(ERROR, "Failed to open logger file\n" STRERROR(errno));
    }
    LoggerSetFile(ostream);
    LoggerSetLevel(INFO);

    text_t text = {};
    FILE* istream = fopen(TEXT_FILE, "r");
    if (istream == nullptr) {
        LOG(ERROR, "Failed to open text file");
        return EXIT_FAILURE;
    }
    parse_text(istream, &text);

    text_t words_to_find = {};
    FILE* words_to_find_istream = fopen(WORDS_FILE, "r");
    if (istream == nullptr) {
        LOG(ERROR, "Failed to open text file");
        free(text.buffer);
        free(text.text);
        return EXIT_FAILURE;
    }
    parse_text(words_to_find_istream, &words_to_find);

    hashtable_t hashtable = {};
    hashtable_ctor(&hashtable, HASHTABLE_SIZE);

    for (size_t i = 0; i < text.size; i++) {
        add_elem(&hashtable, text.text[i]);
    }

    printf("Average bucket size is %f\n", find_avarage_size(&hashtable));

    for (size_t i = 0; i < words_to_find.size; i++) {
        find_elem(&hashtable, words_to_find.text[i]);
    }

    hashtable_dtor(&hashtable);

    free(text.buffer);
    free(text.text);

    free(words_to_find.buffer);
    free(words_to_find.text);

    if (fclose(istream) == EOF) {
        LOG(ERROR, "Failed to close file\n" STRERROR(errno));
    }

    if (fclose(words_to_find_istream) == EOF) {
        LOG(ERROR, "Failed to close file\n" STRERROR(errno));
    }

    if (fclose(ostream) == EOF) {
        fprintf(stderr, "Failed to close file\n" STRERROR(errno));
    }
}




