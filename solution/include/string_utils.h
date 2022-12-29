#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdlib.h>

enum string_compare_status {
	SCS_LESS = -1,
	SCS_EQUALS,
	SCS_GREATER
};

size_t string_length(const char *restrict string);
enum string_compare_status string_compare(const char *restrict first, const char *restrict second);
void string_remove_trailing_whitespace(char *restrict string);

#endif // STRING_UTILS_H
