#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../inc/decode.h"
#include "../inc/encrypt.h"

steno_data *decode(BMPImage *bmp) {
    printf("decoding image ...\n");


    int header_offset_bytes = sizeof(word_t) + sizeof(byte_t);
    steno_data *meta = create_steno_elem();

    /* Get meta data from BMP*/
    memcpy(meta, bmp->data, header_offset_bytes);

    // create output array
    byte_t *data = calloc(meta->encoded_size_bytes, sizeof(byte_t));
    if (data == NULL) {
        perror("Error allocating memory to encode array");
        delete_BMP(bmp);
        exit(-1);
    }

    //  tracks pos through decoded file
    int pos = 0;
    int i = header_offset_bytes;
    /* Decode data from file by extracting bits from each pixel*/
    while (pos / 8 < meta->encoded_size_bytes) {
        char extracted_bits = bmp->data[i] & ((1U << meta->bit_encoding_level) - 1);
        data[pos / 8] |= extracted_bits << (pos % 8);
        pos += meta->bit_encoding_level;
        i++;
    }
    meta->data = data;

    printf("decoded image successfully\n");

    return meta;
}

int decode_BMP(char *BMP_PATH, char *password) {
    printf("opening %s with passwd %s\n", BMP_PATH, password);

    BMPImage *bmp;
    // loads BMP image into memory
    if ((bmp = load_BMP(BMP_PATH)) == NULL) {
        perror("Error loading BMP image");
        return EXIT_FAILURE;
    }

    // retrieves an array of the decoded data from the image
    steno_data *decoded = decode(bmp);

    // If password argument given
    if (strcmp(password, "\0") != 0) {
        // Gavin's decode algorithm
        decrypt(decoded, password);
    }

    /* Pulls the original filename from the decoded data */
    char *filename = strtok((char *) decoded->data, "\n");

    printf("saving decoded file %s ... \n", filename);

    FILE *out_fptr = fopen(filename, "wb");

    // creates the output file
    if (out_fptr == NULL) {
        printf("Error creating output file : %s\n", filename);
        delete_BMP(bmp);
        delete_steno_elem(decoded);
        fclose(out_fptr);
        return EXIT_FAILURE;
    }

    // write decoded data to output file
    if (fwrite(decoded->data + strlen(filename) + 1, sizeof(byte_t),
               decoded->encoded_size_bytes - strlen(filename) - 1, out_fptr) < 1) {
        perror("Error opening output file\n");
        delete_BMP(bmp);
        delete_steno_elem(decoded);
        fclose(out_fptr);
        return EXIT_FAILURE;
    }

    // saves the decoded file
    fclose(out_fptr);

    printf("saved decoded file %s\n", filename);

    /* safely free structure */
    delete_steno_elem(decoded);

    // frees the bmp data from memory
    delete_BMP(bmp);


    return EXIT_SUCCESS;
}