#ifndef MDICT_H
#define MDICT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "file_writer.h"

enum {
	WORD_MAX_LENGTH = 31,
	DICT_MAX_SIZE = 100
};

struct dict_entry {
	char key[WORD_MAX_LENGTH];
	char value[WORD_MAX_LENGTH];
};

struct dict {
	struct dict_entry entries[DICT_MAX_SIZE];
	size_t size;
};

inline struct dict dict_create() {
	return (struct dict) {0};
}

enum add_status {
	ADD_SUCCESSFUL,
	ADD_DICT_FULL,
	ADD_KEY_EXISTS
};

enum add_status dict_add_entry(struct dict *restrict this, struct dict_entry new_entry);
bool dict_remove_entry(struct dict *restrict this, const char *restrict key);
const struct dict_entry *dict_get(const struct dict *restrict this, const char *restrict key);
const struct dict_entry *dict_find_by_value(const struct dict *restrict this, const char *restrict value);
void dict_print(const struct dict *restrict this);
bool dict_save(const struct dict *restrict this, file_writer writer);

#endif // MDICT_H
