#ifndef STENOGRAPHY_ENCODE_H
#define STENOGRAPHY_ENCODE_H

#include "bmp.h"

/**
 * @brief Encodes a bmp image
 *
 * @param BMP image to encode
 * @param data to encode into image
 *
 * @returns void
 */
extern void encode(BMPImage *bmp, steno_data *encoded_data);

/**
 * @brief Outputs the encoded BMP image
 *
 * @param input BMP image file path
 * @param file to encode into image
 * @param output BMP image file path
 * @param password to encrypt data with
 *
 * @returns EXIT_SUCCESS if successful
 */
extern int encode_BMP(char *BMP_PATH, char *INP_PATH, char *OUT_PATH, char *password);

#endif
