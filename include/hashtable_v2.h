#ifndef HASHTABLE_V2_H
#define HASHTABLE_V2_H

#include <stdio.h>
#include <stdint.h>
#include "list.h"
#include "parse_text.h"

typedef struct {
    bucket_t* hasht;
    node_t* list;
    size_t hashtable_size;
} hashtable_t;

uint64_t hash(word_t word);

list_status_t hashtable_ctor(hashtable_t* hashtable, size_t size);
void hashtable_dtor(hashtable_t* hashtable);

void add_elem(hashtable_t* hashtable, word_t word);
double find_avarage_size(const hashtable_t* hashtable);

size_t find_elem(const hashtable_t* hashtable, word_t word);
size_t is_elem_present(const hashtable_t* hashtable, word_t word);
void print_info_about_elem(const hashtable_t* hashtable, word_t word);

#endif /* HASHTABLE_V2_H */
