#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "backet.h"
#include "logger.h"

//============================================================================================

list_status_t ctor(backet_t* backet) {
	assert(backet != nullptr);
	dtor(backet);

	backet->tail = new_node(backet);
	if (backet->tail == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return MEM_ALLOCATON_ERROR;
	}

	backet->tail->data.cnt = 0;
	backet->tail->data.hash = 0;
	backet->tail->data.len = 0;
	backet->tail->data.word = "";
	backet->tail->next = backet->tail;
	backet->tail->prev = nullptr;

	backet->size = 0;
	return OK;
}

void dtor(backet_t* backet) {
	if (backet == nullptr || backet->tail == nullptr) return;

	node_t* current_node = backet->tail;
	for (size_t i = 0; (i < backet->size) && (current_node != nullptr); i++) {
		node_t* next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}

	backet->size = 0;
}

node_t* new_node(backet_t* backet) {
	assert(backet != nullptr);
	return ((node_t*) calloc(sizeof(node_t), sizeof(char)));
}

//=============================================================================================

node_t* get_anchor_node(backet_t* backet, size_t anchor) {
	assert(backet != nullptr);
	node_t* current_pos = backet->tail;
	for (size_t i = 0; (i < anchor) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}
	return current_pos;
}

list_status_t insert_after(backet_t* backet, size_t anchor, elem_t elem) {
	assert(backet != nullptr);

    if (anchor > backet->size) {
        LOG(ERROR, "Cannot insert element to %zu, exceeds the size %zu\n", anchor, backet->size);
        return INDEX_EXCEED_LIST_SIZE;
    }

    node_t* next_node = new_node(backet);
    if (next_node == nullptr) {
        LOG(ERROR, "Memory for the cell was not allocated");
        return MEM_ALLOCATON_ERROR;
    }
    backet->size++;

    node_t* anchor_node = get_anchor_node(backet, anchor);

    next_node->data.cnt = elem.cnt;
    next_node->data.hash = elem.hash;
    next_node->data.len = elem.len;
    next_node->data.word = elem.word;

    if (anchor_node != nullptr) {
        next_node->next = anchor_node->next;
        if (anchor_node->next != nullptr) {
            anchor_node->next->prev = next_node;
        }
        anchor_node->next = next_node;
        next_node->prev = anchor_node;
    } else {
        next_node->next = nullptr;
        next_node->prev = nullptr;
    }

	if (anchor == 0 && anchor_node == nullptr) {
		backet->tail = next_node;
	}
    return OK;
}

list_status_t delete_at(backet_t* backet, size_t anchor) {
	assert(backet != nullptr);
	if (anchor == 0) {
		LOG(ERROR, "Cannot delete a 0 element, numeration starts from 1\n");
		return NULL_INDEX;
	}

	if (anchor >= backet->size) {
		LOG(ERROR, "Cannot delete a %zu element, exceeds the size %zu\n", anchor, backet->size);
		return INDEX_EXCEED_LIST_SIZE;
	}

	backet->size--;
	node_t* anchor_node = get_anchor_node(backet, anchor);

	anchor_node->prev->next = anchor_node->next;
	anchor_node->next->prev = anchor_node->prev;

	free(anchor_node);
	anchor_node = nullptr;
	return OK;
}

//=============================================================================================

list_status_t insert_before(backet_t* backet, size_t anchor, elem_t elem) {
	assert(backet != nullptr);
	return insert_after(backet, anchor - 1, elem);
}

list_status_t push_front(backet_t* backet, elem_t elem) {
	assert(backet != nullptr);
	return insert_after(backet, 0, elem);
}

list_status_t push_back(backet_t* backet, elem_t elem) {
	assert(backet != nullptr);
	return insert_after(backet, backet->size, elem);
}

list_status_t pop_front(backet_t* backet) {
	assert(backet != nullptr);
	return delete_at(backet, 1);
}


//=============================================================================================

elem_t front(backet_t* backet) {
	assert(backet != nullptr);
	if (backet->size > 1) {
		return backet->tail->next->data;
	}

	LOG(ERROR, "List is empty, cannot return a front element\n");
	return {nullptr, 0, 0, 0};
}

elem_t back(backet_t* backet) {
	assert(backet != nullptr);
	if (backet->size > 1) {
		return get_head(backet)->data;
	}

	LOG(ERROR, "List is empty, cannot return a back element\n");
	return {nullptr, 0, 0, 0};
}

elem_t previous_elem(backet_t* backet, size_t pos) {
	assert(backet != nullptr);
	if (pos < 2) {
		LOG(ERROR, "Element %zu does not have a previous\n", pos);
		return {nullptr, 0, 0, 0};
	}

	if (pos > backet->size) {
		LOG(ERROR, "Element %zu exceeds the size of the list\n", pos);
	}
	return get_anchor_node(backet, pos)->prev->data;
}

elem_t next_elem(backet_t* backet, size_t pos) {
	assert(backet != nullptr);
	if (pos >= backet->size) {
		LOG(ERROR, "Element %zu exceeds the size of the list\n", pos);
	}
	return get_anchor_node(backet, pos)->next->data;
}

size_t is_empy(backet_t* backet) {
	assert(backet != nullptr);
	return !(backet->size - 1);
}

node_t* get_head(backet_t* backet) {
	assert(backet != nullptr);
	node_t* current_pos = backet->tail;
	for (size_t i = 0; (i <= backet->size) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}
	return current_pos;
}

//=============================================================================================

ssize_t find_elem_by_value(backet_t* backet, uint64_t hash) {
	assert(backet != nullptr);
	node_t* current_pos = backet->tail;
	for (size_t i = 1; (i <= backet->size) && (current_pos != nullptr); i++) {
		if (current_pos->data.hash == hash) {
			return (ssize_t) i;
		}
		current_pos = current_pos->next;
	}
	return -1;
}

ssize_t find_elem_by_index (backet_t* backet, size_t index) {
	assert(backet != nullptr);
	if (index > backet->size) {
		return -1;
	}

	node_t* current_pos = backet->tail;
	for (size_t i = 1; (i <= index) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}
	return (ssize_t) current_pos->data.hash;
}

ssize_t add_counter_to_index(backet_t* backet, size_t index) {
	assert(backet != nullptr);
	if (index > backet->size) {
		return -1;
	}

	node_t* current_pos = backet->tail;
	for (size_t i = 2; (i <= index) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}

	current_pos->data.cnt++;
	return 1;
}

void add_elem_to_backet(backet_t* backet, const char* word, size_t len, uint64_t hash) {
	assert(backet != nullptr);
	assert(word != nullptr);

	ssize_t index = find_elem_by_value(backet, hash);
	if (index == -1) {
		push_front(backet, {word, len, 1, hash});
	}
	else {
		add_counter_to_index(backet, (size_t) index);
	}
}

void print_info_about_elem(backet_t* backet, size_t index) {
	assert(backet != nullptr);
	node_t* current_pos = backet->tail;
	for (size_t i = 1; (i < index) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}

	printf("word: \"%s\", amount: %zu\n", current_pos->data.word, current_pos->data.cnt);
}
