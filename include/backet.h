#ifndef LIST_H
#define LIST_H

#include <stdio.h>

const ssize_t PREV_POISON_VALUE = -1;

typedef struct {
	const char* word;
	size_t len;
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
	node* next;
	node* prev;
} node_t;

typedef enum {
    NO_ERRORS            = 0,
    FILE_OPEN_ERROR      = 1,
    IMAGE_CREATION_ERROR = 2,
} error_t;

typedef struct {
	node_t* tail;
	size_t size;
} backet_t;


list_status_t ctor(backet_t* backet);
void dtor(backet_t* backet);

node_t* get_head(backet_t* backet);

list_status_t insert_after(backet_t* backet, size_t anchor, elem_t elem);
list_status_t insert_before(backet_t* backet, size_t anchor, elem_t elem);
list_status_t delete_at(backet_t* backet, size_t anchor);

list_status_t push_front(backet_t* backet, elem_t elem);
list_status_t push_back(backet_t* backet, elem_t elem);
list_status_t pop_front(backet_t* backet);

elem_t front(backet_t* backet);
elem_t back(backet_t* backet);

elem_t previous_elem(backet_t* backet, size_t pos);
elem_t next_elem(backet_t* backet, size_t pos);

size_t is_empy(backet_t* backet);

ssize_t find_elem_by_value(backet_t* backet, uint64_t hash);
ssize_t find_elem_by_index(backet_t* backet, size_t index);
ssize_t add_counter_to_index(backet_t* backet, size_t index);
void print_info_about_elem(backet_t* backet, size_t index);
void add_elem_to_backet(backet_t* backet, const char* word, size_t len, uint64_t hash);

node_t* get_anchor_node(backet_t* backet, size_t anchor);
node_t* new_node(backet_t* backet);

#endif /* LIST_H */
