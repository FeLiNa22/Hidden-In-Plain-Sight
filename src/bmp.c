#include <stdlib.h>
#include <stdio.h>
#include "../inc/bmp.h"

static BMPImage *create_BMP(BMPHeader *header, byte_t *data) {
    BMPImage *bmp = (BMPImage *) malloc(sizeof(BMPImage));
    if (bmp == NULL) {
        perror("Error reserving memory for BMP header struct\n");
        exit(-1);
    }
    bmp->header = header;
    bmp->data = data;
    return bmp;
}

void delete_BMP(BMPImage *bmp) {
    free(bmp->header);
    free(bmp->data);
    free(bmp);
}

static BMPHeader *alloc_header(void) {
    BMPHeader *bmp_header = (BMPHeader *) malloc(sizeof(BMPHeader));
    if (bmp_header == NULL) {
        perror("Error reserving memory for BMP header struct\n");
        exit(-1);
    }
    return bmp_header;
}

static byte_t *alloc_data(word_t image_size_bytes) {
    byte_t *data = (byte_t *) calloc(image_size_bytes, sizeof(byte_t));
    if (data == NULL) {
        perror("Error reserving memory for BMP image data\n");
        exit(-1);
    }
    return data;
}

BMPImage *load_BMP(char *BMP_PATH) {
    // open bitmap image file
    FILE *fptr;
    if ((fptr = fopen(BMP_PATH, "rb")) == NULL) {
        perror("Error opening BMP image\n");
        return NULL;
    }
    // reserve memory for header struct
    BMPHeader *header = alloc_header();
    // load header information into struct
    if (fread(header, sizeof(BMPHeader), 1, fptr) < 1) {
        perror("Error loading header info\n");
        // couldn't load header into struct
        free(header);
        fclose(fptr);
        exit(-1);
    }

    // reserve memory for pixel data
    byte_t *data = alloc_data(header->image_size_bytes);
    // sets pos in file to start of image data
    fseek(fptr, header->offset, SEEK_SET);
    // load img data into structure
    if (fread(data, 1, header->image_size_bytes, fptr) < 1) {
        perror("Error loading image data\n");
        // couldn't load img data into struct
        free(header);
        free(data);
        fclose(fptr);
        exit(-1);
    }
    BMPImage *bmp = create_BMP(header, data);
    fclose(fptr);
    return bmp;
}

void save_BMP(BMPImage *bmp, char *filename) {
    printf("saving %s ...\n", filename);

    FILE *fptr;
    if ((fptr = fopen(filename, "wb")) == NULL) {
        perror("Error creating bmp file");
        delete_BMP(bmp);
        exit(-1);
    }
    // writes the header
    if (fwrite(bmp->header, sizeof(BMPHeader), 1, fptr) < 1) {
        perror("Error writing header to bmp file");
        delete_BMP(bmp);
        exit(-1);
    }
    // moves pointer to offset
    fseek(fptr, bmp->header->offset, SEEK_SET);
    // writes the image data
    if (fwrite(bmp->data, bmp->header->image_size_bytes, 1, fptr) < 1) {
        perror("Error writing data to bmp file");
        delete_BMP(bmp);
        exit(-1);
    }
    // saves the file
    fclose(fptr);
    printf("saved %s successfully\n", filename);
}