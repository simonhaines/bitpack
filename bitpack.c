#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "bitpack.h"

bitpack_t bitpack_create(uint8_t *buffer, size_t size) {
	bitpack_t bitpack = { buffer, size, 0 };
	return bitpack;
}

size_t bitpack_count(bitpack_t *pack) {
	return (pack->cursor / 8) + ((pack->cursor % 8 > 0) ? 1 : 0);
}

bool bitpack_write(bitpack_t *pack, uint32_t value, size_t bits) {
	if (bits <= 32 && (pack->cursor + bits) <= (pack->size * 8)) {
		while (true) {
			size_t index = pack->cursor / 8;
			size_t width = 8 - (pack->cursor % 8);
			unsigned int mask = (1 << width) - 1;

			if (bits > width) {
				pack->buffer[index] |= (value >> (bits - width)) & mask;
				pack->cursor += width;
				bits -= width;
			} else {
				pack->buffer[index] |= ((value & mask) << (width - bits));
				pack->cursor += bits;
				return true;
			}
		}
	} else {
		return false;
	}
}

bool bitpack_read(bitpack_t *pack, uint32_t *value, size_t bits) {
	if (bits <= 32 && (pack->cursor + bits) <= (pack->size * 8)) {
		*value = 0;
		while (true) {
			size_t index = pack->cursor / 8;
			size_t width = 8 - (pack->cursor % 8);
			unsigned int mask = (1 << width) - 1;

			if (bits > width) {
				*value |= (pack->buffer[index] & mask) << (bits - width);
				pack->cursor += width;
				bits -= width;
			} else {
				*value |= (pack->buffer[index] >> (width - bits)) & mask;
				pack->cursor += bits;
				return true;
			}
		}
	} else {
		return false;
	}
}

