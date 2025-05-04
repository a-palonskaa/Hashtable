#ifndef BUCKET_H
#define BUCKET_H

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
} bucket_status_t;

typedef struct node{
	elem_t data;
	struct node* next;
	struct node* prev;
} node_t;

typedef struct {
	node_t* tail;
	size_t size;
} bucket_t;


bucket_status_t ctor(bucket_t* bucket);
void dtor(bucket_t* bucket);

node_t* new_node();
size_t is_empty(bucket_t* bucket);
node_t* get_head(bucket_t* bucket);

bucket_status_t insert_after(bucket_t* bucket, size_t anchor, elem_t elem);
bucket_status_t insert_before(bucket_t* bucket, size_t anchor, elem_t elem);
bucket_status_t delete_at(bucket_t* bucket, size_t anchor);

bucket_status_t push_front(bucket_t* bucket, elem_t elem);
bucket_status_t push_back(bucket_t* bucket, elem_t elem);
bucket_status_t pop_front(bucket_t* bucket);

node_t* get_anchor_node(bucket_t* bucket, size_t anchor);
elem_t previous_elem(bucket_t* bucket, size_t pos);
elem_t next_elem(bucket_t* bucket, size_t pos);

void add_elem_to_bucket(bucket_t* bucket, word_t word, uint64_t hash);
ssize_t find_and_track_elem_by_value(bucket_t* bucket, uint64_t hash);
size_t find_elem_by_value(bucket_t* bucket, uint64_t hash);
ssize_t find_elem_by_index(bucket_t* bucket, size_t index);

ssize_t add_counter_to_index(bucket_t* bucket, size_t index);
void print_info_about_elem(bucket_t* bucket, size_t index);

#endif /* BUCKET_H */
