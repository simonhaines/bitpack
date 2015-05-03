#ifndef __bitpack_h
#define __bitpack_h

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint8_t *buffer;
	size_t size;
	size_t cursor;
} bitpack_t;


bitpack_t bitpack_create(uint8_t *buffer, size_t size);

size_t bitpack_count(bitpack_t *pack);

bool bitpack_write(bitpack_t *pack, uint32_t value, size_t bits);

bool bitpack_read(bitpack_t *pack, uint32_t *value, size_t bits);

#endif
