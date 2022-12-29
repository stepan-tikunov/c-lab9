#include "file_writer.h"

#include "string_utils.h"

file_writer file_writer_open(const char *restrict filename) {
	return (file_writer) {
		.value = fopen(filename, "w")
	};
}

void file_writer_close(file_writer *restrict this) { 
	fclose(this->value);
	this->value = NULL;
}

bool file_writer_write(file_writer this, const char *restrict buffer) {
	size_t length = string_length(buffer);

	return length == fwrite(buffer, 1, length, this.value);
}
