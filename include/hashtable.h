#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdint.h>
#include "backet.h"
#include "parse_text.h"

typedef struct {
    backet_t* hasht;
    size_t hashtable_size;
} hashtable_t;

uint64_t hash(const char* word, size_t len);

list_status_t hashtable_ctor(hashtable_t* hashtable, size_t size);
void hashtable_dtor(hashtable_t* hashtable);

double find_avarage_size(hashtable_t* hashtable);
void add_elem(hashtable_t* hashtable, const char* word, size_t len);

bool find_elem(hashtable_t* hashtable, const char* word, size_t len);
void print_info_about_elem(hashtable_t* hashtable, const char* word, size_t len);
ssize_t is_elem_present(hashtable_t* hashtable, word_t word);

#endif /* HASHTABLE_H */
