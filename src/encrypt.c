#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include "../inc/utils.h"
#include "../inc/encrypt_utils.h"

#define MATRIX_SIZE (16)
#define MATRIX_DIMENSION (4)
#define RIJNDAEL_ROUNDS (10)

// djb2 Hashing algorithm - for bytes 1-8 of key
unsigned long hash_djb2(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        // hash = hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

// sdbm Hashing algorithm - for bytes 9-16 of key
unsigned long hash_sdbm(char *str) {
    unsigned long hash = 0;
    int c;
    while ((c = *str++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

// hash function (deterministic) which takes a string and returns 16 byte array.
byte_t *hash_password(char *password) {
    byte_t *key = malloc(MATRIX_SIZE * sizeof(byte_t));
    if (key == NULL) {
        perror("FAILED TO ASSIGN MEMORY TO KEY\n");
        exit(EXIT_FAILURE);
    }
    // hash password - each hashing function returns 64 bit integer,
    // so 2 separate hashing functions are needed for 128 bits
    unsigned long hashed_password1 = hash_djb2(password);
    unsigned long hashed_password2 = hash_sdbm(password);
    // set array values of key - bits 1-8 from dbj2, bits 8-16 from sdbm
    for (int i = 0; i < MATRIX_SIZE / 2; i++) {
        // byte_t value = hashed_password1 & 0xFF;
        key[i] = hashed_password1 & 0xFF;
        key[MATRIX_SIZE/2 + i] = hashed_password2 & 0xFF;
        hashed_password1 >>= 8;
        hashed_password2 >>= 8;
    }
    return key;
}

// executes main algorithm for Rijndael Encryption
void rijndael_cipher(byte_t *block, byte_t *key) {
    int rcon_index = 1;
    add_round_key(block, key);
    for (int i = 0; i < RIJNDAEL_ROUNDS - 1; i++) {
        aes_key_schedule(key, &rcon_index);
        sub_bytes(block);
        shift_rows(block);
        mix_columns(block);
        add_round_key(block, key);
        rcon_index++;
    }
    aes_key_schedule(key, &rcon_index);
    sub_bytes(block);
    shift_rows(block);
    add_round_key(block, key);
}

void rijndael_decipher(byte_t *block, byte_t *key) {
    byte_t *round_key_after_encryption = get_round_key_after_encryption(key);
    // Copy contents of round_key_after_encryption into key
    for (int i = 0; i < MATRIX_SIZE; i++) {
        key[i] = round_key_after_encryption[i];
    }
    int rcon_index = 10;    
    add_round_key(block, round_key_after_encryption);
    inverse_shift_rows(block);
    inverse_sub_bytes(block);
    inverse_aes_key_schedule(round_key_after_encryption, &rcon_index);
    rcon_index--;
    for (int i = 0; i < RIJNDAEL_ROUNDS - 1; i++) {
        add_round_key(block, round_key_after_encryption);
        inverse_mix_columns(block);
        inverse_shift_rows(block);
        inverse_sub_bytes(block);
        inverse_aes_key_schedule(round_key_after_encryption, &rcon_index);
        rcon_index--;
    }
    add_round_key(block, round_key_after_encryption);
    free(round_key_after_encryption);
}

void encrypt(steno_data *encoded_data, char* password) {
    printf("encrypting data ...\n");

    byte_t *key = hash_password(password);
    int i = 0;
    while(i < encoded_data->encoded_size_bytes) {
        if ((encoded_data->encoded_size_bytes - i) > 16) {
            byte_t block[16];
            // Initialize block array with 16 bytes from data
            for (int j = 0; j < MATRIX_SIZE; j++) {
                block[j] = encoded_data->data[i+j];                
            }
            rijndael_cipher(block, key);
            for (int k = 0; k < MATRIX_SIZE; k++) {
                encoded_data->data[i+k] = block[k];
            }
            i += 16;
        } else {
            // Remaining bytes are XOR ciphered to prevent detection
            byte_t xor_key = 255;
            encoded_data->data[i] ^= xor_key;
            i++;
        }
    }
    free(key);
    printf("encrypted successfully\n");
}

void decrypt(steno_data *decoded, char* password) {
    printf("decrypting data ...\n");

    byte_t *key = hash_password(password);
    int i = 0;
    while(i < decoded->encoded_size_bytes) {
        if ((decoded->encoded_size_bytes - i) > 16) {
            byte_t block[16];
            // initialize block array with 16 bytes from data
            for (int j = 0; j < MATRIX_SIZE; j++) {
                block[j] = decoded->data[i+j];                
            }
            rijndael_decipher(block, key);
            for (int k = 0; k < MATRIX_SIZE; k++) {
                decoded->data[i+k] = block[k];
            }
            i += 16;
        } else {
            // remaining bytes are XOR ciphered to be decrypted
            byte_t xor_key = 255;
            decoded->data[i] ^= xor_key;
            i++;
        }
    }
    free(key);

    printf("decrypted successfully\n");
}