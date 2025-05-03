#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "bucket.h"
#include "logger.h"

//============================================================================================

bucket_status_t ctor(bucket_t* bucket) {
	assert(bucket != nullptr);
	dtor(bucket);

	bucket->tail = new_node();
	if (bucket->tail == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return MEM_ALLOCATON_ERROR;
	}

	bucket->tail->data.cnt = 0;
	bucket->tail->data.hash = 0;
	bucket->tail->data.word.len = 0;
	bucket->tail->data.word.word = "";
	bucket->tail->next = bucket->tail;
	bucket->tail->prev = nullptr;

	bucket->size = 0;
	return OK;
}

void dtor(bucket_t* bucket) {
	if (bucket == nullptr || bucket->tail == nullptr) return;

	node_t* current_node = bucket->tail;
	node_t* next_node = nullptr;
	while (current_node != nullptr) {
		next_node = current_node->next;
		free(current_node);
		current_node = next_node;
	}

	bucket->size = 0;
}

node_t* new_node() {
	return ((node_t*) calloc(sizeof(node_t), sizeof(char)));
}

//=============================================================================================

node_t* get_anchor_node(bucket_t* bucket, size_t anchor) {
	assert(bucket != nullptr);
	node_t* current_pos = bucket->tail;
	for (size_t i = 0; (i < anchor) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}
	return current_pos;
}

bucket_status_t insert_after(bucket_t* bucket, size_t anchor, elem_t elem) {
	assert(bucket != nullptr);

    if (anchor > bucket->size) {
        LOG(ERROR, "Cannot insert element to %zu, exceeds the size %zu\n", anchor, bucket->size);
        return INDEX_EXCEED_LIST_SIZE;
    }

    node_t* next_node = new_node();
    if (next_node == nullptr) {
        LOG(ERROR, "Memory for the cell was not allocated");
        return MEM_ALLOCATON_ERROR;
    }
    bucket->size++;

    node_t* anchor_node = get_anchor_node(bucket, anchor);

    next_node->data.cnt = elem.cnt;
    next_node->data.hash = elem.hash;
    next_node->data.word.word = elem.word.word;
	next_node->data.word.len = elem.word.len;

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
		bucket->tail = next_node;
	}
    return OK;
}

bucket_status_t push_front(bucket_t* bucket, elem_t elem) {
	assert(bucket != nullptr);

    node_t* next_node = new_node();
    if (next_node == nullptr) {
        LOG(ERROR, "Memory for the cell was not allocated");
        return MEM_ALLOCATON_ERROR;
    }
    bucket->size++;

    node_t* anchor_node = bucket->tail;

    next_node->data.cnt = elem.cnt;
    next_node->data.hash = elem.hash;
    next_node->data.word.word = elem.word.word;
	next_node->data.word.len = elem.word.len;

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
		bucket->tail = next_node;
	}
	return OK;
}


bucket_status_t delete_at(bucket_t* bucket, size_t anchor) {
	assert(bucket != nullptr);
	if (anchor == 0) {
		LOG(ERROR, "Cannot delete a 0 element, numeration starts from 1\n");
		return NULL_INDEX;
	}

	if (anchor >= bucket->size) {
		LOG(ERROR, "Cannot delete a %zu element, exceeds the size %zu\n", anchor, bucket->size);
		return INDEX_EXCEED_LIST_SIZE;
	}

	bucket->size--;
	node_t* anchor_node = get_anchor_node(bucket, anchor);

	anchor_node->prev->next = anchor_node->next;
	anchor_node->next->prev = anchor_node->prev;

	free(anchor_node);
	anchor_node = nullptr;
	return OK;
}

//=============================================================================================

bucket_status_t insert_before(bucket_t* bucket, size_t anchor, elem_t elem) {
	assert(bucket != nullptr);
	return insert_after(bucket, anchor - 1, elem);
}

bucket_status_t push_back(bucket_t* bucket, elem_t elem) {
	assert(bucket != nullptr);
	return insert_after(bucket, bucket->size, elem);
}

bucket_status_t pop_front(bucket_t* bucket) {
	assert(bucket != nullptr);
	return delete_at(bucket, 1);
}


//=============================================================================================

elem_t front(bucket_t* bucket) {
	assert(bucket != nullptr);
	if (bucket->size > 1) {
		return bucket->tail->next->data;
	}

	LOG(ERROR, "List is empty, cannot return a front element\n");
	return {{nullptr, 0}, 0, 0};
}

elem_t back(bucket_t* bucket) {
	assert(bucket != nullptr);
	if (bucket->size > 1) {
		return get_head(bucket)->data;
	}

	LOG(ERROR, "List is empty, cannot return a back element\n");
	return {{nullptr, 0}, 0, 0};
}

elem_t previous_elem(bucket_t* bucket, size_t pos) {
	assert(bucket != nullptr);
	if (pos < 2) {
		LOG(ERROR, "Element %zu does not have a previous\n", pos);
		return {{nullptr, 0}, 0, 0};
	}

	if (pos > bucket->size) {
		LOG(ERROR, "Element %zu exceeds the size of the list\n", pos);
	}
	return get_anchor_node(bucket, pos)->prev->data;
}

elem_t next_elem(bucket_t* bucket, size_t pos) {
	assert(bucket != nullptr);
	if (pos >= bucket->size) {
		LOG(ERROR, "Element %zu exceeds the size of the list\n", pos);
	}
	return get_anchor_node(bucket, pos)->next->data;
}

size_t is_empty(bucket_t* bucket) {
	assert(bucket != nullptr);
	return !(bucket->size);
}

node_t* get_head(bucket_t* bucket) {
	assert(bucket != nullptr);
	node_t* current_pos = bucket->tail;
	for (size_t i = 0; (i <= bucket->size) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}
	return current_pos;
}

//=============================================================================================

size_t find_elem_by_value(bucket_t* bucket, uint64_t hash) {
	assert(bucket != nullptr);

	node_t* current_pos = bucket->tail;
	size_t i = 1;
    while (current_pos && current_pos->data.hash != hash) {
        current_pos = current_pos->next;
        i++;
    }
	return (size_t) current_pos;
}

ssize_t find_and_track_elem_by_value(bucket_t* bucket, uint64_t hash) {
	assert(bucket != nullptr);

	node_t* current_pos = bucket->tail;
	size_t i = 1;
    while (current_pos && current_pos->data.hash != hash) {
        current_pos = current_pos->next;
        i++;
    }
	return current_pos ? (ssize_t) current_pos->data.cnt++ : -1;
}

ssize_t find_elem_by_index(bucket_t* bucket, size_t index) {
	assert(bucket != nullptr);
	if (index > bucket->size) return -1;

	node_t* current_pos = bucket->tail;
	while (index-- > 0) {
        current_pos = current_pos->next;
    }
	return (ssize_t) current_pos->data.hash;
}

ssize_t add_counter_to_index(bucket_t* bucket, size_t index) {
	assert(bucket != nullptr);
	if (index >= bucket->size) return -1;

 	node_t* current = bucket->tail;
    while (index-- > 0) {
        current = current->next;
    }
    current->data.cnt++;
	return 1;
}

// void add_elem_to_bucket(bucket_t* bucket, word_t word, uint64_t hash) {
// 	assert(bucket != nullptr);
// 	assert(word.word != nullptr);
//
// 	ssize_t index = find_elem_by_value(bucket, hash);
// 	if (index != -1) {
// 		add_counter_to_index(bucket, (size_t) index);
// 	}
// 	else {
// 		push_front(bucket, {{word.word, word.len}, 1, hash});
// 	}
// }

//		node_t* next_node = new_node();
//    	if (next_node == nullptr) {
//        	LOG(ERROR, "Memory for the cell was not allocated");
//        	return;
//    	}
//
//    	next_node->data.cnt = 1;
//    	next_node->data.hash = hash;
//    	next_node->data.word.word = word.word;
//    	next_node->data.word.len = word.len;
//
//    	next_node->next = bucket->tail;
//    	next_node->prev = nullptr;
//
//    	if (bucket->tail != nullptr) {
//        	bucket->tail->prev = next_node;
//    	}
//
//    	bucket->tail = next_node;
//    	bucket->size++;

void add_elem_to_bucket(bucket_t* bucket, word_t word, uint64_t hash) {
	assert(bucket != nullptr);
	assert(word.word != nullptr);

	if (find_and_track_elem_by_value(bucket, hash) == -1) {
		push_front(bucket, {{word.word, word.len}, 1, hash});
	}
}


void print_info_about_elem(bucket_t* bucket, size_t index) {
	assert(bucket != nullptr);
	node_t* current_pos = bucket->tail;
	for (size_t i = 1; (i < index) && (current_pos != nullptr); i++) {
		current_pos = current_pos->next;
	}

	printf("word: \"%s\", amount: %zu\n", current_pos->data.word.word, current_pos->data.cnt);
}
