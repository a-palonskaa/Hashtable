#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <gtest/gtest.h>
#include "hashtable_v2.h"
#include "parse_text.h"
#include "logger.h"

const char* LOGGER_FILE = "logs/logger_ptrs.html";
const char* TEXT_FILE = "data/meow.txt";
const char* WORDS_FILE = "data/words.txt";
const size_t HASHTABLE_SIZE = 600;             // load factor = 19


class HashTableTest : public ::testing::Test {
protected:
    hashtable_t hashtable;

    void SetUp() override {
      hashtable_ctor(&hashtable, 600);
    }

    void TearDown() override {
      hashtable_dtor(&hashtable);
    }

};

TEST_F(HashTableTest, AddSignleElem) {
    add_elem(&hashtable, {"cat", 3});
    EXPECT_STREQ(hashtable.list->data.word.word, "cat");
}

TEST_F(HashTableTest, AddFindSignleElem) {
    add_elem(&hashtable, {"cat", 3});
    EXPECT_EQ(find_elem(&hashtable, {"cat", 3}), 1);
}

TEST_F(HashTableTest, AddDuplicateElem) {
    add_elem(&hashtable, {"word", 4});
    add_elem(&hashtable, {"word", 4});

    EXPECT_EQ(hashtable.list->data.cnt, 2);
    EXPECT_EQ(find_elem(&hashtable, {"word", 4}), 2);
}

TEST_F(HashTableTest, AddFindElem) {
    add_elem(&hashtable, {"word", 4});
    add_elem(&hashtable, {"cat", 3});
    add_elem(&hashtable, {"impression", 10});
    add_elem(&hashtable, {"cat", 3});

    EXPECT_EQ(find_elem(&hashtable, {"dog", 3}), 0);
    EXPECT_EQ(find_elem(&hashtable, {"fish", 4}), 0);
    EXPECT_EQ(find_elem(&hashtable, {"word", 4}), 1);
    EXPECT_EQ(find_elem(&hashtable, {"impression", 10}), 1);
    EXPECT_EQ(find_elem(&hashtable, {"cat", 3}), 2);
}

TEST_F(HashTableTest, MyProg) {
    FILE* ostream = fopen(LOGGER_FILE, "wb");
    if (ostream == nullptr) {
        LOG(ERROR, "Failed to open logger file\n" STRERROR(errno));
    }
    LoggerSetFile(ostream);
    LoggerSetLevel(INFO);

    text_t text = {};
    FILE* istream = fopen(TEXT_FILE, "r");
    if (istream == nullptr) {
        LOG(ERROR, "Failed to open text file");
    }
    parse_text(istream, &text);

    text_t words_to_find = {};
    FILE* words_to_find_istream = fopen(WORDS_FILE, "r");
    if (istream == nullptr) {
        LOG(ERROR, "Failed to open text file");
        free(text.buffer);
        free(text.text);
    }
    parse_text(words_to_find_istream, &words_to_find);

    for (size_t i = 0; i < text.size; i++) {
        add_elem(&hashtable, text.text[i]);
    }

    EXPECT_EQ(find_elem(&hashtable, {"did", 3}), 420);
    EXPECT_EQ(find_elem(&hashtable, {"and", 3}), 8059);
    EXPECT_EQ(find_elem(&hashtable, {"away", 4}), 329);
    EXPECT_EQ(find_elem(&hashtable, {"gtar", 4}), 0);
    EXPECT_EQ(find_elem(&hashtable, {"travesti", 8}), 0);
    EXPECT_EQ(find_elem(&hashtable, {"kettleholder", 12}), 1);
    EXPECT_EQ(find_elem(&hashtable, {"niminypiminy", 12}), 1);
    EXPECT_EQ(find_elem(&hashtable, {"incontrovertible", 16}), 0);

    EXPECT_EQ(hashtable.hasht[0].size, 14);
    EXPECT_EQ(hashtable.hasht[189].size, 20);
    EXPECT_EQ(hashtable.hasht[50].size, 14);

    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[0]), hash({"did", 3})), 420);
    for (size_t i = 1; i < 600; i++) {
      EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[i]), hash({"did", 3})), 0);
    }

    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"already", 7})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"avigdors", 8})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"bountiful", 9})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"clothesbrush", 12})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"crossly", 7})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"dollars", 7})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"extra", 5})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"lapping", 7})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"magnificent", 11})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"nectar", 6})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"palest", 6})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"refined", 7})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"separation", 10})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"setting", 7})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"snowball", 8})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"survey", 6})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"tenderhearted", 13})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"throng", 6})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"usso", 4})) > 0, 1);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"yesterday", 9})) > 0, 1);

    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"did", 3})) > 0, 0);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"to", 2})) > 0, 0);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"leave", 5})) > 0, 0);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"off", 3})) > 0, 0);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"boyish", 6})) > 0, 0);
    EXPECT_EQ(find_elem_by_value(&(hashtable.hasht[189]), hash({"tricks", 6})) > 0, 0);


    free(text.buffer);
    free(text.text);

    free(words_to_find.buffer);
    free(words_to_find.text);

    if (fclose(istream) == EOF) {
        LOG(ERROR, "Failed to close file\n" STRERROR(errno));
    }

    if (fclose(words_to_find_istream) == EOF) {
        LOG(ERROR, "Failed to close file\n" STRERROR(errno));
    }

    if (fclose(ostream) == EOF) {
        fprintf(stderr, "Failed to close file\n" STRERROR(errno));
    }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
