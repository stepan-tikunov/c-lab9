#include "run_modes.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mdict.h"
#include "string_utils.h"

extern inline struct dict dict_create();

enum menu_option {
	MO_ADD = 1,
	MO_REMOVE,
	MO_TRANSLATE_ENG_RUS,
	MO_TRANSLATE_RUS_ENG,
	MO_PRINT_DICT,
	MO_SAVE_DICT,
	MO_EXIT,
	MO_BAD
};

enum {
	EXIT_STATUS_FAILURE = -1,
	EXIT_STATUS_SUCCESS
};

static void print_menu() {
	printf("Menu options:\n");
	printf("\t1. Add new entry to the dictionary;\n");
	printf("\t2. Remove an entry from the dictionary;\n");
	printf("\t3. Translate a word from English to Russian;\n");
	printf("\t4. Translate a word from Russian to English;\n");
	printf("\t5. Print the dictionary;\n");
	printf("\t6. Save the dictionary to file;\n");
	printf("\t7. Exit the program;\n");
}

static enum menu_option menu() {
	print_menu();
	printf("Select one of these options: ");

	uint32_t option_code;

	const size_t BUFFER_SIZE = 10;
	char buffer[BUFFER_SIZE];
	char *input = fgets(buffer, BUFFER_SIZE, stdin);

	if (input == NULL) {
		return MO_EXIT;
	}

	int scanf_status = sscanf(input, "%" SCNu32, &option_code);

	if (scanf_status == 0) {
		if (strlen(input) == BUFFER_SIZE) {
			while (((char) fgetc(stdin)) != '\n') {}
		}
		return MO_BAD;
	}

	return (enum menu_option) option_code;
}

static void read_word(const char *restrict message, char *restrict word) {
	printf("%s", message);
	if (!fgets(word, WORD_MAX_LENGTH, stdin)) {
		fprintf(stderr, "Couldn't read word");
		exit(EXIT_STATUS_FAILURE);
	}
	string_remove_trailing_whitespace(word);
}

static struct dict_entry read_entry() {
	struct dict_entry result;

	read_word("Enter english word: ", result.key);
	read_word("Enter russian translation: ", result.value);

	return result;
}

static void add_words(struct dict *restrict dict) {
	struct dict_entry new_entry = read_entry();

	switch (dict_add_entry(dict, new_entry)) {
	case ADD_SUCCESSFUL:
		printf("Added successfully\n");
		break;
	case ADD_KEY_EXISTS:
		printf("This english word is already in dictionary\n");
		break;
	case ADD_DICT_FULL:
		printf("Dictionary is full\n");
	}
}

static void remove_words(struct dict *restrict dict) {
	char key[WORD_MAX_LENGTH];
	read_word("Enter english word to remove from dictionary: ", key);

	if (dict_remove_entry(dict, key)) {
		printf("Removed successfully\n");
	} else {
		printf("Word not found in dictionary\n");
	}
}

static void translate_to_russian(struct dict *restrict dict) {
	char key[WORD_MAX_LENGTH];
	read_word("Enter english word to translate: ", key);

	const struct dict_entry *entry = dict_get(dict, key);

	if (entry) {
		printf("Russian translation: %s\n", entry->value);
	} else {
		printf("Word not found in dictionary\n");
	}
}

static void translate_to_english(struct dict *restrict dict) {
	char value[WORD_MAX_LENGTH];
	read_word("Enter russian word to translate: ", value);

	const struct dict_entry *entry = dict_find_by_value(dict, value);

	if (entry) {
		printf("English translation: %s\n", entry->key);
	} else {
		printf("Word not found in dictionary");
	}
}

static void save_dict(struct dict *restrict dict) {
	char filename[WORD_MAX_LENGTH];
	read_word("Enter file name: ", filename);

	file_writer writer = file_writer_open(filename);

	if (writer.value) {
		dict_save(dict, writer);
		file_writer_close(&writer);
	} else {
		fprintf(stderr, "Could not open file for write\n");
	}
}

int interactive() {
	struct dict dict = dict_create();

	for (;;) {
		switch (menu()) {
		case MO_ADD:
			add_words(&dict);
			break;
		case MO_REMOVE:
			remove_words(&dict);
			break;
		case MO_TRANSLATE_ENG_RUS:
			translate_to_russian(&dict);
			break;
		case MO_TRANSLATE_RUS_ENG:
			translate_to_english(&dict);
			break;
		case MO_PRINT_DICT:
			dict_print(&dict);
			break;
		case MO_SAVE_DICT:
			save_dict(&dict);
			break;
		case MO_EXIT:
			return 0;
		default:
			fprintf(stderr, "Select one of provided options\n");
		}
	}
}
