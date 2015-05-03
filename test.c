#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bitpack.h"

/* MinUnit: http://www.jera.com/techinfo/jtns/jtn002.html */
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); \
                               if (message) return message; } while (0)

static char* test_write() {
	uint8_t buffer[2] = { 0 };
	bitpack_t pack = bitpack_create(buffer, 2);

	mu_assert("error writing 4 bits", bitpack_write(&pack, 10, 4));
	mu_assert("error writing 10 bits", bitpack_write(&pack, 1021, 10));
	mu_assert("error writing 2 bits", bitpack_write(&pack, 3, 2));

	mu_assert("bad buffer[0] value", buffer[0] == 175);
	mu_assert("bad buffer[1] value", buffer[1] == 247);
	return 0;
}

static char* test_read() {
	/* 0b10101111, 0b11110111 */
	uint8_t buffer[2] = { 175, 247 };
	bitpack_t pack = bitpack_create(buffer, 2);

	uint32_t value;
	mu_assert("error reading 4 bits", bitpack_read(&pack, &value, 4));
	mu_assert("bad value decoded, expecting 10", value == 10);
	mu_assert("error reading 10 bits", bitpack_read(&pack, &value, 10));
	mu_assert("bad value decoded, expecting 1021", value == 1021);
	mu_assert("error reading 2 bits", bitpack_read(&pack, &value, 2));
	mu_assert("bad value decoded, expecting 10", value == 3);
	return 0;
}

bool encode_geolocation(bitpack_t *pack, double latitude, double longitude) {
	/* Latitude sign */
	if (!bitpack_write(pack, (latitude < 0) ? 1 : 0, 1)) {
		return false;
	}
	/* Latitude degrees */
	if (!bitpack_write(pack, trunc(abs(latitude)), 7)) {
		return false;
	}
	/* Latitude decimal */
	if (!bitpack_write(pack, abs(latitude * 10000) % 10000, 14)) {
		return false;
	}

	/* Longitude sign */
	if (!bitpack_write(pack, (longitude < 0) ? 1 : 0, 1)) {
		return false;
	}
	/* Longitude degrees */
	if (!bitpack_write(pack, trunc(abs(longitude)), 8)) {
		return false;
	}
	/* Longitude decimal */
	if (!bitpack_write(pack, abs(longitude * 10000) % 10000, 14)) {
		return false;
	}

	return true;
}

bool decode_geolocation(bitpack_t *pack, double *latitude, double *longitude) {
	uint32_t sign, degrees, decimal;
	if (!bitpack_read(pack, &sign, 1)) {
		return false;
	}
	if (!bitpack_read(pack, &degrees, 7)) {
		return false;
	}
	if (!bitpack_read(pack, &decimal, 14)) {
		return false;
	}
	*latitude = degrees + (decimal / 10000.0);
	if (sign == 1) {
		*latitude = 0 - *latitude;
	}

	if (!bitpack_read(pack, &sign, 1)) {
		return false;
	}
	if (!bitpack_read(pack, &degrees, 8)) {
		return false;
	}
	if (!bitpack_read(pack, &decimal, 14)) {
		return false;
	}
	*longitude = degrees + (decimal / 10000.0);
	if (sign == 1) {
		*longitude = 0 - *longitude;
	}
	return true;
}

static char* test_geolocation() {
	uint8_t buffer[6] = { 0 };
	bitpack_t pack = bitpack_create(buffer, 6);
	double latitude = -41.235259;
	double longitude = +146.412828;

	mu_assert("error encoding geolocation",
			encode_geolocation(&pack, latitude, longitude));

	pack = bitpack_create(buffer, 6);
	mu_assert("error decoding geolocation",
			decode_geolocation(&pack, &latitude, &longitude));
	mu_assert("bad latitude decoded", latitude == -41.2352);
	mu_assert("bad longitude decoded", longitude == +146.4128);
	return 0;
}

static char* all_tests() {
	mu_run_test(test_write);
	mu_run_test(test_read);
	mu_run_test(test_geolocation);
	return 0;
}

int main(int argc, char **argv) {
	char *result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	} else {
		printf("ALL TESTS PASSED\n");
	}

	return result != 0;
}
