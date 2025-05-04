#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <cstdint>
#include "parse_text.h"

typedef struct {
	word_t word;
	size_t cnt;
	uint64_t hash;
} elem_t;

typedef int (*compare_t) (void* elem1, void* elem2);

typedef enum {
	OK                     = 0,
	MEM_ALLOCATON_ERROR    = 1,
	LIST_IS_FULL           = 2,
	INDEX_EXCEED_LIST_SIZE = 3,
	NULL_INDEX             = 4,
	INCORRECT_FREE_CELL    = 5,
	SIZE_EXCEEDS_CAPACITY  = 6,
	NULL_FREE_CELL         = 7,
} list_status_t;

typedef struct node{
	elem_t data;
	struct node* next;
	struct node* prev;
} node_t;

typedef struct {
    node_t* node;
    size_t size;
} bucket_t;

list_status_t ctor(node_t** tail);
void dtor(node_t** tail);

node_t* new_node();
node_t* insert_after(node_t* node, node_t** head, elem_t elem);
size_t add_elem_to_bucket(bucket_t* bucket, node_t** head, word_t word, uint64_t hash);

ssize_t find_and_track_elem_by_value(bucket_t* bucket, uint64_t hash);
size_t find_elem_by_value(bucket_t* bucket, uint64_t hash);

#endif /* LIST_H */
