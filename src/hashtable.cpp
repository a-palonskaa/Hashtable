#include <assert.h>
#include "logger.h"
#include "hashtable.h"
#include "backet.h"

list_status_t hashtable_t::hashtable_ctor(size_t size) {
    hashtable = (backet_t*) calloc(size, sizeof(backet_t));
    if (hashtable == nullptr) {
        LOG(ERROR, "Failed to allocate memoyr");
        hashtable_size = 0;
        return MEM_ALLOCATON_ERROR;
    }
    hashtable_size = size;
    return OK;
}

void hashtable_t::hashtable_dtor() {
    for (size_t i = 0; i < hashtable_size; i++) {
        hashtable[i].dtor();
    }

    free(hashtable);
    hashtable = nullptr;
    hashtable_size = 0;
}

void hashtable_t::add_elem(const char* word, size_t len) {
    assert(word != nullptr);
    uint64_t word_hash = hash(word, len);
    hashtable[word_hash % hashtable_size].add_elem_to_backet(word, len, word_hash);
}

bool hashtable_t::find_elem(const char* word, size_t len) {
    assert(word != nullptr);
    uint64_t word_hash = hash(word, len);

    ssize_t index = hashtable[word_hash % hashtable_size].find_elem_by_value(word_hash);
    return (index == -1) ? false : true;
}

void hashtable_t::print_info_about_elem(const char* word, size_t len) {
    assert(word != nullptr);
    uint64_t word_hash = hash(word, len);

    ssize_t index = hashtable[word_hash % hashtable_size].find_elem_by_value(word_hash);
    if (index == -1) {
        printf("No elem was found\n");
    }
    else {
        hashtable[word_hash % hashtable_size].print_info_about_elem((size_t) index);
    }
}

ssize_t hashtable_t::is_elem_present(const char* word, size_t len) {
    assert(word != nullptr);
    uint64_t word_hash = hash(word, len);

    ssize_t index = hashtable[word_hash % hashtable_size].find_elem_by_value(word_hash);
    if (index == -1) {
        printf("No\n");
    }
    return index;
}

double hashtable_t::find_avarage_size() {
    size_t unique_words = 0;

    for (size_t i = 0; i < hashtable_size; i++) {
        unique_words += hashtable[i].get_size();
    }

    return (double) unique_words / (double) hashtable_size;
}
