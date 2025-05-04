#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "list.h"
#include "logger.h"

//============================================================================================

list_status_t ctor(node_t** tail) {
	assert(tail != nullptr);
	dtor(tail);

    *tail = new_node();
	if (*tail == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return MEM_ALLOCATON_ERROR;
	}

	(*tail)->data.cnt = 0;
	(*tail)->data.hash = 0;
	(*tail)->data.word.len = 0;
	(*tail)->data.word.word = "";
	(*tail)->next = nullptr;
	(*tail)->prev = nullptr;

	return OK;
}

void dtor(node_t** tail) {
    if (tail == nullptr) return;

    node_t* node = *tail;
    node_t* next = nullptr;
    while (node != nullptr) {
        next = node->next;
        free(node);
        node = next;
    }

    *tail = nullptr;
}

node_t* new_node() {
	return ((node_t*) calloc(sizeof(node_t), sizeof(char)));
}

//=============================================================================================

node_t* insert_after(node_t* node, node_t** head, elem_t elem) {
    assert(node != nullptr);

    node_t* next_node = new_node();
    if (next_node == nullptr) {
        LOG(ERROR, "Memory for the cell was not allocated");
        return *head;
    }
    if (node->next == nullptr) {
        node->next = next_node;
        next_node->next = nullptr;
        next_node->prev = node;
        *head = next_node;
    }
    else {
        next_node->next = node->next;
        node->next->prev = next_node;
        node->next = next_node;
        next_node->prev = node;
    }
    next_node->data.cnt = elem.cnt;
    next_node->data.hash = elem.hash;
    next_node->data.word.word = elem.word.word;
	next_node->data.word.len = elem.word.len;
    return *head;
}

ssize_t find_and_track_elem_by_value(bucket_t* bucket, uint64_t hash) {
	assert(bucket != nullptr);

	node_t* current_pos = bucket->node;
	size_t i = 0;
    while (current_pos &&  i < bucket->size && current_pos->data.hash != hash) {
        current_pos = current_pos->next;
        i++;
    }
	return i != bucket->size ? current_pos->data.hash == hash ? (ssize_t) current_pos->data.cnt++ : -1 : -1;
}

size_t add_elem_to_bucket(bucket_t* bucket, node_t** head, word_t word, uint64_t hash) {
	assert(bucket != nullptr);
	assert(word.word != nullptr);

	if (find_and_track_elem_by_value(bucket, hash) == -1) {
		insert_after(bucket->node, head, {{word.word, word.len}, 1, hash});
        return 1;
	}
    return 0;
}

size_t find_elem_by_value(bucket_t* bucket, uint64_t hash) {
	assert(bucket != nullptr);

	node_t* current_pos = bucket->node;
	size_t i = 0;
    while (current_pos && (i < bucket->size) && (current_pos->data.hash != hash)) {
        current_pos = current_pos->next;
        i++;
    }
	return i != bucket->size ? current_pos->data.hash == hash ? (ssize_t) current_pos->data.cnt : 0 : 0;
}
