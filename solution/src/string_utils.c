#include "string_utils.h"

#include <stdbool.h>

size_t string_length(const char *restrict string) {
	size_t result = 0;

	while (string[result++]) {}

	return result - 1;
}

enum string_compare_status string_compare(const char *restrict first, const char *restrict second) {
	size_t f_len = string_length(first);
	size_t s_len = string_length(second);

	if (f_len != s_len) {
		return f_len < s_len ? SCS_LESS : SCS_GREATER;
	}

	for (size_t i = 0; i < f_len; ++i) {
		if (first[i] != second[i]) {
			return first[i] < second[i] ? SCS_LESS : SCS_GREATER;
		}
	}

	return SCS_EQUALS;
}

static inline bool is_whitespace(char c) {
	return c == '\t' || c == ' ' || c == '\r' || c == '\n';
}

void string_remove_trailing_whitespace(char *restrict string) {
	size_t length = string_length(string);

	for (size_t i = 0; i < length; ++i) {
		if (is_whitespace(string[i])) {
			string[i] = 0;
			return;
		}
	}
}
