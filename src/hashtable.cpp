#include <assert.h>
#include "logger.h"
#include "hashtable.h"
#include "backet.h"

list_status_t hashtable_ctor(hashtable_t* hashtable, size_t size) {
    assert(hashtable != nullptr);
    hashtable->hasht = (backet_t*) calloc(size, sizeof(backet_t));
    if (hashtable->hasht == nullptr) {
        LOG(ERROR, "Failed to allocate memoyr");
        hashtable->hashtable_size = 0;
        return MEM_ALLOCATON_ERROR;
    }
    hashtable->hashtable_size = size;
    return OK;
}

void hashtable_dtor(hashtable_t* hashtable) {
    if (hashtable == nullptr) return;
    for (size_t i = 0; i < hashtable->hashtable_size; i++) {
        dtor(&(hashtable->hasht[i]));
    }

    free(hashtable->hasht);
    hashtable->hasht = nullptr;
    hashtable->hashtable_size = 0;
}

void add_elem(hashtable_t* hashtable, const char* word, size_t len) {
    assert(hashtable != nullptr);
    assert(word != nullptr);
    uint64_t word_hash = hash(word, len);
    add_elem_to_backet(&(hashtable->hasht[word_hash % hashtable->hashtable_size]), word, len, word_hash);
}

bool find_elem(hashtable_t* hashtable, const char* word, size_t len) {
    assert(hashtable != nullptr);
    assert(word != nullptr);
    uint64_t word_hash = hash(word, len);

    ssize_t index = find_elem_by_value(&(hashtable->hasht[word_hash % hashtable->hashtable_size]), word_hash);
    return (index == -1) ? false : true;
}

void print_info_about_elem(hashtable_t* hashtable, const char* word, size_t len) {
    assert(hashtable != nullptr);
    assert(word != nullptr);
    uint64_t word_hash = hash(word, len);

    ssize_t index = find_elem_by_value(&(hashtable->hasht[word_hash % hashtable->hashtable_size]), word_hash);
    if (index == -1) {
        printf("No elem was found\n");
    }
    else {
        print_info_about_elem(&(hashtable->hasht[word_hash % hashtable->hashtable_size]), (size_t) index);
    }
}

ssize_t is_elem_present(hashtable_t* hashtable, word_t word) {
    assert(hashtable != nullptr);
    assert(word.word != nullptr);
    uint64_t word_hash = hash(word.word, word.len);

    ssize_t index = find_elem_by_value(&(hashtable->hasht[word_hash % hashtable->hashtable_size]), word_hash);
    if (index == -1) {
        printf("No\n");
    }
    return index;
}

double find_avarage_size(hashtable_t* hashtable) {
    assert(hashtable != nullptr);
    size_t unique_words = 0;

    for (size_t i = 0; i < hashtable->hashtable_size; i++) {
        unique_words += hashtable->hasht[i].size;
    }

    return (double) unique_words / (double) hashtable->hashtable_size;
}
