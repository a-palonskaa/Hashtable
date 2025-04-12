#include <string.h>
#include <assert.h>
#include "hashtable.h"

const size_t SEED = 0xABCD;
const size_t MUL = 0xC6A4A7935BD1E995;
const int BIT_SHIFT = 47;

uint64_t hash(const char* word, size_t len) {
    assert(word != nullptr);

    uint64_t hash = SEED ^ (len * MUL);
    const uint8_t* data = (const uint8_t*) word;
    uint64_t buffer = 0;

    while (len >= 8) {
        memcpy(&buffer, data, sizeof(uint64_t));
        data += 8;
        len -= 8;

        buffer *= MUL;
        buffer ^= buffer >> BIT_SHIFT;
        buffer *= MUL;

        hash ^= buffer;
        hash *= MUL;
    }

    if (len > 0) {
        buffer = 0;
        memcpy(&buffer, data, len);
        hash ^= buffer;
        hash *= MUL;
    }

    hash ^= hash >> BIT_SHIFT;
    hash *= MUL;
    hash ^= hash >> BIT_SHIFT;
    return hash;
}
