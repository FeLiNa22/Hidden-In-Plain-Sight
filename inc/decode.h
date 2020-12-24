#ifndef STENOGRAPHY_DECODE_H
#define STENOGRAPHY_DECODE_H

#include "bmp.h"

/**
 * @brief Decodes a previously encoded BMP image
 *
 * @param path where BMP image lies
 * @param password to decode file (used in encryption phase)
 *
 * @returns void
 */
extern int decode_BMP(char *BMP_PATH, char *password);

extern steno_data *decode(BMPImage *bmp);

#endif //STENOGRAPHY_DECODE_H
