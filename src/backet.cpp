#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "backet.h"
#include "logger.h"

//============================================================================================

list_status_t backet_t::ctor() {
	dtor();

	tail = new_node();
	if (tail == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return MEM_ALLOCATON_ERROR;
	}

	tail->data.cnt = 0;
	tail->data.hash = 0;
	tail->data.len = 0;
	tail->data.word = "";
	tail->next = tail;
	tail->prev = nullptr;

	size = 0;
	return OK;
}

void backet_t::dtor() {
	if (tail == nullptr) {
		return;
	}

	node_t* current_node = tail;
	for (size_t i = 0; (i < size) && (current_node != nullptr); i++) {
		node_t* next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}

	size = 0;
}

node_t* backet_t::new_node() {
	return ((node_t*) calloc(sizeof(node_t), sizeof(char)));
}

//=============================================================================================

node_t* backet_t::get_anchor_node(size_t anchor) {
	node_t* current_pos = tail;
	for (size_t i = 0; (i < anchor) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}
	return current_pos;
}

list_status_t backet_t::insert_after(size_t anchor, elem_t elem) {
    if (anchor > size) {
        LOG(ERROR, "Cannot insert element to %zu, exceeds the size %zu\n", anchor, get_size());
        return INDEX_EXCEED_LIST_SIZE;
    }

    node_t* next_node = new_node();
    if (next_node == nullptr) {
        LOG(ERROR, "Memory for the cell was not allocated");
        return MEM_ALLOCATON_ERROR;
    }
    size++;

    node_t* anchor_node = get_anchor_node(anchor);

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
		tail = next_node;
	}
    return OK;
}

list_status_t backet_t::delete_at(size_t anchor) {
	if (anchor == 0) {
		LOG(ERROR, "Cannot delete a 0 element, numeration starts from 1\n");
		return NULL_INDEX;
	}

	if (anchor >= size) {
		LOG(ERROR, "Cannot delete a %zu element, exceeds the size %zu\n", anchor, get_size());
		return INDEX_EXCEED_LIST_SIZE;
	}

	size--;
	node_t* anchor_node = get_anchor_node(anchor);

	anchor_node->prev->next = anchor_node->next;
	anchor_node->next->prev = anchor_node->prev;

	free(anchor_node);
	anchor_node = nullptr;
	return OK;
}

//=============================================================================================

list_status_t backet_t::insert_before(size_t anchor, elem_t elem) {
	return insert_after(anchor - 1, elem);
}

list_status_t backet_t::push_front(elem_t elem) {
	return insert_after(0, elem);
}

list_status_t backet_t::push_back(elem_t elem) {
	return insert_after(size, elem);
}

list_status_t backet_t::pop_front() {
	return delete_at(1);
}


//=============================================================================================

elem_t backet_t::front() {
	if (size > 1) {
		return tail->next->data;
	}

	LOG(ERROR, "List is empty, cannot return a front element\n");
	return {nullptr, 0, 0, 0};
}

elem_t backet_t::back() {
	if (size > 1) {
		return get_head()->data;
	}

	LOG(ERROR, "List is empty, cannot return a back element\n");
	return {nullptr, 0, 0, 0};
}

elem_t backet_t::previous_elem(size_t pos) {
	if (pos < 2) {
		LOG(ERROR, "Element %zu does not have a previous\n", pos);
		return {nullptr, 0, 0, 0};
	}

	if (pos > size) {
		LOG(ERROR, "Element %zu exceeds the size of the list\n", pos);
	}
	return get_anchor_node(pos)->prev->data;
}

elem_t backet_t::next_elem(size_t pos) {
	if (pos >= size) {
		LOG(ERROR, "Element %zu exceeds the size of the list\n", pos);
	}
	return get_anchor_node(pos)->next->data;
}

size_t backet_t::get_size() {
	return size - 1;
}

size_t backet_t::is_empy() {
	return !(size - 1);
}

node_t* backet_t::get_head() {
	node_t* current_pos = tail;
	for (size_t i = 0; (i <= size) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}
	return current_pos;
}

node_t* backet_t::get_tail() {
	return tail->next;
}

//=============================================================================================

ssize_t backet_t::find_elem_by_value(uint64_t hash) {
	node_t* current_pos = tail;
	for (size_t i = 1; (i <= size) && (current_pos != nullptr); i++) {
		if (current_pos->data.hash == hash) {
			return (ssize_t) i;
		}
		current_pos = current_pos->next;
	}
	return -1;
}

ssize_t backet_t::find_elem_by_index (size_t index) {
	if (index > size) {
		return -1;
	}

	node_t* current_pos = tail;
	for (size_t i = 1; (i <= index) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}
	return (ssize_t) current_pos->data.hash;
}

ssize_t backet_t::add_counter_to_index(size_t index) {
	if (index > size) {
		return -1;
	}

	node_t* current_pos = tail;
	for (size_t i = 2; (i <= index) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}

	current_pos->data.cnt++;
	return 1;
}

void backet_t::add_elem_to_backet(const char* word, size_t len, uint64_t hash) {
	assert(word != nullptr);

	ssize_t index = find_elem_by_value(hash);
	if (index == -1) {
		push_front({word, len, 1, hash});
	}
	else {
		add_counter_to_index((size_t) index);
	}
}

void backet_t::print_info_about_elem(size_t index) {
	node_t* current_pos = tail;
	for (size_t i = 1; (i < index) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}

	printf("word: \"%s\", amount: %zu\n", current_pos->data.word, current_pos->data.cnt);
}
