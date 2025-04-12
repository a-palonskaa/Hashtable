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

class backet_t {
public:
	backet_t() = default;
    ~backet_t() = default;

	list_status_t ctor();
	void dtor();

	node_t* get_head();
	node_t* get_tail();

	list_status_t insert_after(size_t anchor, elem_t elem);
	list_status_t insert_before(size_t anchor, elem_t elem);
	list_status_t delete_at(size_t anchor);

	list_status_t push_front(elem_t elem);
	list_status_t push_back(elem_t elem);
	list_status_t pop_front();

	elem_t front();
	elem_t back();

	elem_t previous_elem(size_t pos);
	elem_t next_elem(size_t pos);

	size_t get_size();
	size_t is_empy();

	ssize_t find_elem_by_value(uint64_t hash);
	ssize_t find_elem_by_index(size_t index);
	ssize_t add_counter_to_index(size_t index);
	void print_info_about_elem(size_t index);
	void add_elem_to_backet(const char* word, size_t len, uint64_t hash);
private:
	node_t* get_anchor_node(size_t anchor);
	node_t* new_node();

	node_t* tail;
	size_t size;
};

#endif /* LIST_H */
