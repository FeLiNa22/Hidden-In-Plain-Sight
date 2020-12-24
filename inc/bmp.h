#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#include <stdio.h>
#include "utils.h"

/* Struct is in little endian form */
typedef struct { // total: 54 bytes
    short_t type; // magic identifier: 0x4d42
    word_t size; // file size in bytes
    word_t reserved; // not used
    word_t offset; // offset to image data in bytes from beginning of file (54 bytes)
    word_t dib_header_size; // DIB Header size in bytes (40 bytes)
    word_t width_px; // width of the image
    word_t height_px; // height of image
    short_t num_planes; // number of color planes
    short_t bits_per_pixel; // bits per pixel
    word_t compression; // compression type
    word_t image_size_bytes; // image size in bytes
    word_t x_resolution_ppm; // pixels per meter
    word_t y_resolution_ppm; // pixels per meter
    word_t num_colors; // number of colors
    word_t important_colors; // important colors
} BMPHeader;

/* Structure to represent BMP image */
typedef struct {
    BMPHeader *header;
    byte_t *data;
} BMPImage;

/**
 * @brief Loads a BMP image into memory
 *
 * @param the path where the BMP image is located
 *
 * @returns pointer to BMPImage struct with data from input image
 */
extern BMPImage *load_BMP(char *BMP_PATH);

/**
 * @brief Frees a BMPImage struct and associated data from memory
 *
 * @param pointer to BMP image struct
 *
 * @returns void
 */
extern void delete_BMP(BMPImage *bmp);

/**
 * @brief Saves a BMP image to the output path
 *
 * @param pointer to BMPImage to output
 * @param path to output file to
 *
 * @returns EXIT_SUCCESS if saving BMP was successful
 */
extern void save_BMP(BMPImage *bmp, char *filename);

#endif
