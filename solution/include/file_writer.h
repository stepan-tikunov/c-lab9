#ifndef FILE_WRITER_H
#define FILE_WRITER_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
	FILE *value;
} file_writer;

file_writer file_writer_open(const char *restrict filename);
void file_writer_close(file_writer *restrict this);
bool file_writer_write(file_writer this, const char *restrict buffer);

#endif // FILE_WRITER_H
