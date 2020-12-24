#include <string.h>
#include <stdlib.h>
#include <libgen.h>

#include "../inc/encode.h"
#include "../inc/encrypt.h"

void encode(BMPImage *bmp, steno_data *encoded_data) {
    printf("encoding image ...\n");

    bmp->data[0] &= 0xf0;                                   // clears 4 LSB bits
    bmp->data[0] |= encoded_data->bit_encoding_level;       // stores bit encoding level in 4 LSB bits
    memcpy(&bmp->data[1], &encoded_data->encoded_size_bytes, sizeof(word_t));  // stores encoding size

    //  tracks pos through encoded data
    int pos = 0;
    // tracks pos in image data
    int i = sizeof(byte_t) + sizeof(word_t);
    while (pos / 8 < encoded_data->encoded_size_bytes) {
        bmp->data[i] &= ~((1U << encoded_data->bit_encoding_level) - 1); // clears LSB
        bmp->data[i] |=
                ((1U << encoded_data->bit_encoding_level) - 1) &
                (encoded_data->data[pos / 8] >> (pos % 8)); // sets LSB to encoded
        pos += encoded_data->bit_encoding_level;
        i++;
    }
    printf("encoded image successfully\n");
}

static byte_t calculate_bit_encoding_level(word_t encoding_size_bytes, word_t image_size) {
    // DEFAULT bit encoding level
    byte_t bit_encoding_level = 200;
    // calculates smallest bit encryption level to encode all the data
    // The smaller the level the better results (harder to detect image is encoded)
    for (byte_t i = 8; i > 0; i >>= 1) {
        if (encoding_size_bytes <= (image_size * i) / 8) {
            bit_encoding_level = i;
        }
    }
    // if bit encryption level = 200, then input file size is too large to encode in the image
    if (bit_encoding_level == 200) {
        perror("File is too large to encode in the image (please use a larger image)\n");
        printf("Image size : %d , Encode size : %d \n", image_size, encoding_size_bytes);
        exit(EXIT_FAILURE);
    }
    return bit_encoding_level;
}

int encode_BMP(char *BMP_PATH, char *INP_PATH, char *OUT_PATH, char *password) {

    printf("opening %s and %s\n", BMP_PATH, INP_PATH);

    BMPImage *bmp;
    // loads BMP image into memory
    if ((bmp = load_BMP(BMP_PATH)) == NULL) {
        perror("Error loading BMP image\n");
        delete_BMP(bmp);
        return EXIT_FAILURE;
    }

    FILE *inp_fptr;
    // reads the file with data to encode in the image
    if ((inp_fptr = fopen(INP_PATH, "rb")) == NULL) {
        perror("Error opening file to encode\n");
        delete_BMP(bmp);
        return EXIT_FAILURE;
    }

    // gets the file size in bytes */
    fseek(inp_fptr, 0, SEEK_END);
    word_t input_file_size = ftell(inp_fptr);
    fseek(inp_fptr, 0, SEEK_SET);

    // gets name of file input, this is encoded into the image for recreation later
    char *base_name = basename(INP_PATH);
    char ch = '\n'; // append breakpoint (needed for decoding the filename later)
    strncat(base_name, &ch, 1);

    steno_data *encoded_data = create_steno_elem();

    // size of total data that is encoded in the image
    encoded_data->encoded_size_bytes = input_file_size + strlen(base_name);
    // calculates image size subtracting the reserved space for bit_encoding_level (1st pixel)
    // and encoded_size_bytes (4 pixels)
    word_t image_size = bmp->header->image_size_bytes - sizeof(byte_t) - sizeof(word_t);

    // sets the bit encoding level
    encoded_data->bit_encoding_level = calculate_bit_encoding_level(encoded_data->encoded_size_bytes, image_size);

    // reserve space for data to encode
    encoded_data->data = calloc(encoded_data->encoded_size_bytes, sizeof(byte_t));
    if (encoded_data->data == NULL) {
        perror("Error allocating memory to data array\n");
        exit(EXIT_FAILURE);
    }
    strncpy((char *) encoded_data->data, base_name, strlen(base_name)); // stores name of input file in data array
    // load input data into structure array
    if (fread(encoded_data->data + strlen(base_name), sizeof(byte_t), input_file_size, inp_fptr) < 1) {
        perror("Error opening input file\n");
        exit(EXIT_FAILURE);
    }

    // If password argument given
    if (strcmp(password, "\0") != 0) {
        // Gavin's encryption algorithm
        encrypt(encoded_data, password);
    }

    // save the encoded file safely
    fclose(inp_fptr);

    // encode from the array into the image
    encode(bmp, encoded_data);


    // save the encoded image to out path
    save_BMP(bmp, OUT_PATH);

    // safely free structure
    delete_steno_elem(encoded_data);

    // frees the bmp data from memory
    delete_BMP(bmp);
    return EXIT_SUCCESS;
}