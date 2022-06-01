#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CHECK(condition, name)                                                 \
	do {                                                                   \
		fprintf(stderr, "%-64s\t", name);                                       \
		if (condition) {                                               \
			fprintf(stderr, "\033[1;92mOK\033[0m\n");                       \
		} else {                                                       \
			fprintf(stderr, "\033[1;31mERR: %s\033[0m\n", strerror(errno));      \
			exit(1);                                               \
		}                                                              \
	} while (false)

static char *
read_file(const char *path, size_t *size)
{
	FILE *file = fopen(path, "r");

	if (file == NULL) {
		return NULL;
	}

	size_t buffer_size = 256;
	char  *buffer      = (char *)malloc(buffer_size);
	char   ch;
	size_t no_read_ch = 0;
	do {
		if (no_read_ch >= buffer_size) {
			buffer_size *= 2;
			buffer = (char *)realloc(buffer, buffer_size);
		}
		ch                   = fgetc(file);
		buffer[no_read_ch++] = ch;
	} while (ch != EOF);

	fclose(file);
	*size = no_read_ch;
	return buffer;
}
