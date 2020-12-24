#ifndef STENOGRAPHY_UTILS_H
#define STENOGRAPHY_UTILS_H

#include <stdint.h>

typedef uint32_t word_t;
typedef uint16_t short_t;
typedef uint8_t byte_t;

/* Struct to represent steganography data */
#pragma pack(1)
typedef struct {
    byte_t bit_encoding_level: 4;
    byte_t reserved: 4;
    word_t encoded_size_bytes;
    byte_t *data;
} steno_data;

/**
 * @brief Creates and initialises a new steganography element
 *
 * @returns a steno_data struct
 */
extern steno_data *create_steno_elem(void);

/**
 * @brief Safely frees a steno_data element from memory
 *
 * @param the element to free
 *
 * @returns void
 */
extern void delete_steno_elem(steno_data *elem);

#endif
