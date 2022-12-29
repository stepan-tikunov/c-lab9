#include "run_modes.h"

#include "mdict.h"

int demo() {

	struct dict dict = dict_create();

	struct dict_entry entries[] = {
		{"cat", "kot"},
		{"dog", "sobaka"},
		{"turtle", "cherepakha"},
		{"snake", "zmeya"}
	};

	for (size_t i = 0; i < sizeof(entries) / sizeof(struct dict_entry); ++i) {
		dict_add_entry(&dict, entries[i]);
		fprintf(stderr, "Used dict_add_entry to add word \"%s\" with \"%s\" translation\n", entries[i].key, entries[i].value);
	}

	dict_print(&dict);
	fprintf(stderr, "Used dict_print to print all entries\n");

	char *keys[] = {"cat", "kot", "dog", "sobaka"};

	for (size_t i = 0; i < sizeof(keys) / sizeof(char *); ++i) {
		const struct dict_entry *entry = dict_get(&dict, keys[i]);
		fprintf(stderr, "Used dict_get to get entry with english word \"%s\", got ", keys[i]);
		if (entry) {
			fprintf(stderr, "entry that consists of word \"%s\" with \"%s\" translation\n", entry->key, entry->value);
		} else {
			fprintf(stderr, "no entry\n");
		}
	}

	char *values[] = {"cat", "kot", "dog", "sobaka"};

	for (size_t i = 0; i < sizeof(values) / sizeof(char *); ++i) {
		const struct dict_entry *entry = dict_find_by_value(&dict, values[i]);
		fprintf(stderr, "Used dict_find_by_value to get entry with russian word \"%s\", got ", values[i]);
		if (entry) {
			fprintf(stderr, "entry that consists of word \"%s\" with \"%s\" translation\n", entry->key, entry->value);
		} else {
			fprintf(stderr, "no entry\n");
		}
	}

	char *remove_keys[] = {"cat", "kot", "dog", "sobaka"};
	for (size_t i = 0; i < sizeof(remove_keys) / sizeof(char *); ++i) {
		bool result = dict_remove_entry(&dict, remove_keys[i]);
		fprintf(stderr, "Used dict_remove_entry to remove entry with english word \"%s\", ", values[i]);
		if (result) {
			fprintf(stderr, "removed successfully\n");
		} else {
			fprintf(stderr, "couldn't remove\n");
		}
		dict_print(&dict);
		fprintf(stderr, "Used dict_print to print all entries\n");
	}

	file_writer writer = file_writer_open("dict.txt");
	if (writer.value) {
		bool result = dict_save(&dict, writer);
		fprintf(stderr, "Used dict save to save dict into file \"dict.txt\", ");
		if (result) {
			fprintf(stderr, "saved successfully\n");
		} else {
			fprintf(stderr, "could not save\n");
		}

		file_writer_close(&writer);
	} else {
		fprintf(stderr, "Could not open \"dict.txt\" to write\n");
	}

	return 0;
}
