#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "hashtable.h"
#include "logger.h"
#include "parse_text.h"

const char* LOGGER_FILE = "logs/logger_ptrs.html";
const char* TEXT_FILE = "meow.txt";
const char* WORDS_FILE = "words.txt";

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
    }
    parse_text(istream, &text);



    text_t words_to_find = {};
    FILE* words_to_find_istream = fopen(WORDS_FILE, "r");
    if (istream == nullptr) {
        LOG(ERROR, "Failed to open text file");
    }
    parse_text(words_to_find_istream, &words_to_find);

    hashtable_t hashtable{};
    hashtable.hashtable_ctor(600);

    for (size_t i = 0; i < text.size; i++) {
        hashtable.add_elem(text.text[i].word, text.text[i].len);
    }

    printf("av backet size is %f\n", hashtable.find_avarage_size());

    for (size_t i = 0; i < words_to_find.size; i++) {
        hashtable.is_elem_present(words_to_find.text[i].word, words_to_find.text[i].len); // LINK -  into struct
    }

    hashtable.hashtable_dtor();
    free(text.buffer);
    free(text.text);

    free(words_to_find.buffer);
    free(words_to_find.text);

    fclose(istream);
    fclose(ostream);
}




