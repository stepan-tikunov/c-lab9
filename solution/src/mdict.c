#include "mdict.h"

#include "string_utils.h"

struct search_status {
	size_t index;
	bool exists;
};

static struct search_status lower_bound(const struct dict *restrict this, const char *restrict key, size_t left, size_t right) {
	if (right == left) {
		return (struct search_status) {
			.index = left,
			.exists = false
		};
	}

	size_t mid = (left + right) / 2;

	switch (string_compare(key, this->entries[mid].key)) {
	case SCS_EQUALS:
		return (struct search_status) {
			.index = mid,
			.exists = true
		};
	case SCS_LESS:
		return lower_bound(this, key, left, mid);
	case SCS_GREATER:
		return lower_bound(this, key, mid + 1, right);
	}
}

static void shift_right_all_after(struct dict *restrict this, size_t index) {
	for (size_t i = this->size; i && (--i >= index);) {
		this->entries[i + 1] = this->entries[i];
	}
}

static void shift_left_all_after(struct dict *restrict this, size_t index) {
	for (size_t i = index + 1; i < this->size; ++i) {
		this->entries[i - 1] = this->entries[i];
	}
}

enum add_status dict_add_entry(struct dict *restrict this, struct dict_entry new_entry) {
	if (this->size == DICT_MAX_SIZE) {
		return ADD_DICT_FULL;
	}

	struct search_status status = lower_bound(this, new_entry.key, 0, this->size);

	if (status.exists) {
		return ADD_KEY_EXISTS;
	}

	shift_right_all_after(this, status.index);
	this->entries[status.index] = new_entry;
	++this->size;

	return ADD_SUCCESSFUL;
}

bool dict_remove_entry(struct dict *restrict this, const char *restrict key) {
	struct search_status status = lower_bound(this, key, 0, this->size);

	if (!status.exists) {
		return false;
	}

	shift_left_all_after(this, status.index);
	--this->size;

	return true;
}

const struct dict_entry *dict_get(const struct dict *restrict this, const char *restrict key) {
	struct search_status status = lower_bound(this, key, 0, this->size);

	if (!status.exists) {
		return NULL;
	}

	return this->entries + status.index;
}

const struct dict_entry *dict_find_by_value(const struct dict *restrict this, const char *restrict value) {
	for (size_t i = 0; i < this->size; ++i) {
		if (string_compare(this->entries[i].value, value) == SCS_EQUALS) {
			return this->entries + i;
		}
	}

	return NULL;
}

void dict_print(const struct dict *restrict this) {
	if (this->size == 0) {
		printf("Dictionary is empty\n");
		return;
	}

	printf("All entries:\n");
	for (size_t i = 0; i < this->size; ++i) {
		printf("\t%s -> %s\n", this->entries[i].key, this->entries[i].value);
	}
}

bool dict_save(const struct dict *restrict this, file_writer writer) {
	for (size_t i = 0; i < this->size; ++i) {
		char write_buffer[2 * WORD_MAX_LENGTH + 1];
		int wrote_bytes = sprintf(write_buffer, "%s:%s\n", this->entries[i].key, this->entries[i].value);

		if (wrote_bytes < 0) {
			return false;
		}

		file_writer_write(writer, write_buffer);
	}

	return true;
}
