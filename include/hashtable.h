#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdio.h>
#include <stdint.h>
#include "backet.h"

uint64_t hash(const char* word, size_t len);

class hashtable_t {
public:
    hashtable_t() = default;
    ~hashtable_t() = default;

    list_status_t hashtable_ctor(size_t size);
	void hashtable_dtor();

    double find_avarage_size();
    void add_elem(const char* word, size_t len);

    bool find_elem(const char* word, size_t len);
    void print_info_about_elem(const char* word, size_t len);
    ssize_t is_elem_present(const char* word, size_t len);
private:
    backet_t* hashtable;
    size_t hashtable_size;
};

#endif /* HASHTABLE_H */
