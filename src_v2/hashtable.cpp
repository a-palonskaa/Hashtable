#include <assert.h>
#include "logger.h"
#include "hashtable_v2.h"
#include "list.h"

list_status_t hashtable_ctor(hashtable_t* hashtable, size_t size) {
    assert(hashtable != nullptr);
    hashtable->hasht = (bucket_t*) calloc(size, sizeof(bucket_t));
    if (hashtable->hasht == nullptr) {
        LOG(ERROR, "Failed to allocate memoyr");
        hashtable->hashtable_size = 0;
        return MEM_ALLOCATON_ERROR;
    }
    ctor(&(hashtable->list));
    hashtable->hashtable_size = size;
    return OK;
}

void hashtable_dtor(hashtable_t* hashtable) {
    if (hashtable == nullptr) return;

    dtor(&(hashtable->list));

    free(hashtable->hasht);
    hashtable->hasht = nullptr;
    hashtable->hashtable_size = 0;
}

void add_elem(hashtable_t* hashtable, word_t word) {
    assert(hashtable != nullptr);
    assert(word.word != nullptr);

    uint64_t word_hash = hash(word);
    size_t index = word_hash % hashtable->hashtable_size;

    if (hashtable->hasht[index].node == nullptr) {
        insert_after(hashtable->list, &(hashtable->list), {word, 1, word_hash});
        hashtable->hasht[index].size = 1;
        hashtable->hasht[index].node = hashtable->list;
    }
    else {
        hashtable->hasht[index].size += add_elem_to_bucket(&(hashtable->hasht[index]), &(hashtable->list), word, word_hash);
    }
}

size_t find_elem(const hashtable_t* hashtable, word_t word) {
    assert(hashtable != nullptr);
    assert(word.word != nullptr);

    uint64_t word_hash = hash(word);

    return find_elem_by_value(&(hashtable->hasht[word_hash % hashtable->hashtable_size]), word_hash);
}


double find_avarage_size(const hashtable_t* hashtable) {
    assert(hashtable != nullptr);

    size_t unique_words = 0;

    for (size_t i = 0; i < hashtable->hashtable_size; i++) {
        unique_words += hashtable->hasht[i].size;
    }
    return hashtable->hashtable_size ? ((double) unique_words / (double) hashtable->hashtable_size) : 0;
}
