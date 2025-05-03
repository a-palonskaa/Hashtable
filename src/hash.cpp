#include <string.h>
#include <assert.h>
#include "hashtable.h"
#include <immintrin.h>
#include <x86intrin.h>

const size_t SEED = 0xABCD;
const size_t MUL = 0xC6A4A7935BD1E995;
const int BIT_SHIFT = 47;

uint64_t hash(word_t word) {
    assert(word.word != nullptr);

    size_t len = word.len;
    uint64_t hash = SEED ^ (len * MUL);
    const uint8_t* data = (const uint8_t*) word.word;
    uint64_t buffer = 0;

    if (len >= 8) {
        buffer = _mm_cvtsi128_si64(_mm_loadu_si128((const __m128i*)data));
        data += 8;

        buffer *= MUL;
        buffer ^= buffer >> BIT_SHIFT;
        buffer *= MUL;

        hash = (hash ^ buffer) * MUL ;
    }

    len &= 7;

    if (len > 0) {
        buffer = 0;

        switch (len) {
            case 7: buffer |= (uint64_t)data[6] << 48; [[fallthrough]];
            case 6: buffer |= (uint64_t)data[5] << 40; [[fallthrough]];
            case 5: buffer |= (uint64_t)data[4] << 32; [[fallthrough]];
            case 4: buffer |= (uint64_t)data[3] << 24; [[fallthrough]];
            case 3: buffer |= (uint64_t)data[2] << 16; [[fallthrough]];
            case 2: buffer |= (uint64_t)data[1] << 8;  [[fallthrough]];
            case 1: buffer |= (uint64_t)data[0];
        }

        hash ^= buffer;
        hash *= MUL;
    }

    hash ^= hash >> BIT_SHIFT;
    hash *= MUL;
    hash ^=  hash >> BIT_SHIFT;
    return hash;
}
