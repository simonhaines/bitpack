# Bitpack

Bitpack is a C library for space-efficient serialisation of values.

The library works with arrays of bytes. You provide raw data (in the form of an unsigned integer), and the number of bits to pack that data into. Only the specified number of bits of data are added to the array.

The number of bits you specify must be enough to encode the range of values you might provide. Encoding values in the range of 0..10, for example, requires at least four bits (2^4 = 16) whereas using three bits only allows values in the range of 0..7.

## Creating a bitpack

Create a bitpack by providing a pointer to a byte array, and the size of the array. If you are encoding values, ensure the array has been zeroed.

    uint8_t buffer[2] = { 0 };
	bitpack_t pack = bitpack_create(buffer, 2);

## Encoding values

Encode values by providing the value as an unsigned integer and the number of bits to decode.

    bool result = bitpack_write(pack, 7285, 13);  /* Add 0b1110001110101 */

The bits are extracted from the value and added to the end of the pack.

## Decoding values

To retrieve values from a buffer, create a bitpack for the buffer and read.

    uint8_t buffer[2] = { 227, 168 }; /* 11100011 10101000 */
    bitpack_t pack = bitpack_create(buffer, 2);
    unsigned int value = 0;
    bool result = bitpack_read(&pack, &value, 13);  /* value = 7285 */

## Examples and more information

The test file contains some examples, and more information can be found at http://www.scalardata.com/2015/05/space-efficient-data-representation/.

