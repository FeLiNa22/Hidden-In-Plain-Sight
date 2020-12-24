#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include "../inc/utils.h"

#define MATRIX_SIZE (16)
#define MATRIX_DIMENSION (4)
#define RIJNDAEL_ROUNDS (10)

// Define Rijndael S-Box (SubBytes Lookup table required for AES algorithm)
static const byte_t aes_s_box[256] =
{
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,
    0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,
    0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,
    0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,
    0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,
    0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,
    0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,
    0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,
    0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,
    0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,
    0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,
    0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,
    0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,
    0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,
    0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,
    0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,
    0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
// Define corresponding inverse S-Box for Rijndael encryption
static const byte_t inverse_aes_s_box[256] =
{
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38,
    0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87,
    0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D,
    0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2,
    0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16,
    0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA,
    0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A,
    0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02,
    0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA,
    0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85,
    0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89,
    0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20,
    0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31,
    0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D,
    0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0,
    0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26,
    0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

// Define rijndael galois field for decryption - used in mix_columns
static const byte_t rijndael_galois_field[16] = 
{   2, 3, 1, 1,
    1, 2, 3, 1,
    1, 1, 2, 3,
    3, 1, 1, 2
};
// Define inverse of rijndael galois field for decryption
static const byte_t inverse_rijndael_galois_field[16] =
{   14, 11, 13, 9,
    9, 14, 11, 13,
    13, 9, 14, 11,
    11, 13, 9, 14
};

// Adds key to block
void add_round_key(byte_t *block, byte_t *key) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        block[i] ^= key[i];
    }
}

// Substitues each byte in block for its value in aes_s_box 
void sub_bytes(byte_t *block) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        byte_t left_nibble = block[i] >> 4;
        byte_t right_nibble = block[i] & 15;
        block[i] = aes_s_box[left_nibble * MATRIX_SIZE + right_nibble];
    }
}

// Applies AES row shifts on block matrix
void shift_rows(byte_t *block) {
    // Row 1 does not get rotated
    // Rotate row 2 by 1 to the left
    byte_t blockA_temp = block[4];
    byte_t blockB_temp = block[5];
    byte_t blockC_temp = block[6];
    byte_t blockD_temp = block[7];
    block[4] = blockB_temp;
    block[5] = blockC_temp;
    block[6] = blockD_temp;
    block[7] = blockA_temp;
    // Rotate row 3 by 2 to the left
    blockA_temp = block[8];
    blockB_temp = block[9];
    blockC_temp = block[10];
    blockD_temp = block[11];
    block[8] = blockC_temp;
    block[9] = blockD_temp;
    block[10] = blockA_temp;
    block[11] = blockB_temp;
    // Rotate row 4 by 3 to the left
    blockA_temp = block[12];
    blockB_temp = block[13];
    blockC_temp = block[14];
    blockD_temp = block[15];
    block[12] = blockD_temp;
    block[13] = blockA_temp;
    block[14] = blockB_temp;
    block[15] = blockC_temp;
}

// helper function to calculate multiplication in Galois Field 2^8
byte_t gmul(byte_t a, byte_t b) {
	byte_t p = 0;
	byte_t hi_bit_set;
	for(byte_t i = 0; i < 8; i++) {
		if((b & 1) == 1) {
			p ^= a;
        }
		hi_bit_set = (a & 0x80);
		a <<= 1;
		if(hi_bit_set == 0x80) {
			a ^= 0x1b;
        }
		b >>= 1;
	}
	return p;
}

void mix_columns_helper(byte_t *block, const byte_t *galois_field) {
    uint16_t mult_matrix[MATRIX_SIZE] = {0};
    for (int row = 0; row < MATRIX_DIMENSION; row++) {
        for (int col = 0; col < MATRIX_DIMENSION; col++) {
            for (int inner = 0; inner < MATRIX_DIMENSION; inner++) {
                int index = row * MATRIX_DIMENSION + col;
                mult_matrix[index] ^= 
                    gmul(galois_field[row * MATRIX_DIMENSION + inner],
                    block[inner * MATRIX_DIMENSION + col]);
            }
        }
    }
    for (int i = 0; i < MATRIX_SIZE; i++) {
        block[i] = mult_matrix[i];
    }
}

// multiplies block by rijndael_galois_field matrix
void mix_columns(byte_t *block) {
    mix_columns_helper(block, rijndael_galois_field);
}

// Helper function for calculating new key in AES Key Schedule
void rotate(byte_t *in) {
    byte_t a;
    a = in[0];
    for(byte_t c = 0; c < 3; c++) {
        in[c] = in[c + 1];
    }
    in[3] = a;
}

// Calculate the rcon used in key expansion
byte_t rcon(byte_t in) {
    byte_t c = 1;
    if(in == 0) {
        return 0; 
    }
    while(in != 1) {
        c = gmul(c, 2);
        in--;
    }
    return c;
}

// Function to calculate next key after a add_round_key() operation
void aes_key_schedule(byte_t *key, int *rcon_index) {
    byte_t new_key[MATRIX_SIZE];
    // Calculate first column of matrix
    byte_t new_key_col[MATRIX_DIMENSION];
    for (int row = 0; row < MATRIX_DIMENSION; row++) {
        new_key_col[row] = key[row * MATRIX_DIMENSION + 3]; // Use 3rd Col of key
    }
    rotate(new_key_col); // Rotate this by 1 (upwards)
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        byte_t left_nibble = new_key_col[i] >> 4;
        byte_t right_nibble = new_key_col[i] & 15; // TODO - change to 0x0F
        new_key_col[i] = aes_s_box[left_nibble * MATRIX_SIZE + right_nibble];
    }
    // Apply XOR operations on new_key_col1
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        new_key_col[i] ^= key[i * MATRIX_DIMENSION];
    }
    new_key_col[0] ^= rcon(*rcon_index);
    // Assign 1st column of matrix
    for (int row = 0; row < MATRIX_DIMENSION; row++) {
        new_key[row * MATRIX_DIMENSION] = new_key_col[row];
    }
    // Assign 2nd column of matrix
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        new_key_col[i] ^= key[i * MATRIX_DIMENSION + 1];
    }
    for (int row = 0; row < MATRIX_DIMENSION; row++) {
        new_key[row * MATRIX_DIMENSION + 1] = new_key_col[row];
    }
    // Assign 3rd column of matrix
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        new_key_col[i] ^= key[i * MATRIX_DIMENSION + 2];
    }
    for (int row = 0; row < MATRIX_DIMENSION; row++) {
        new_key[row * MATRIX_DIMENSION + 2] = new_key_col[row];
    }
    // Assign 4th column of matrix
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        new_key_col[i] ^= key[i * MATRIX_DIMENSION + 3];
    }
    for (int row = 0; row < MATRIX_DIMENSION; row++) {
        new_key[row * MATRIX_DIMENSION + 3] = new_key_col[row];
    }

    for (int i = 0; i < MATRIX_SIZE; i++) {
        key[i] = new_key[i];
    }
}

// Applies inverse AES row shifts on block matrix
void inverse_shift_rows(byte_t *block) {
    // Row 1 does not get rotated
    // Rotate row 2 by 1 to the right
    byte_t blockA_temp = block[4];
    byte_t blockB_temp = block[5];
    byte_t blockC_temp = block[6];
    byte_t blockD_temp = block[7];
    block[4] = blockD_temp;
    block[5] = blockA_temp;
    block[6] = blockB_temp;
    block[7] = blockC_temp;
    // Rotate row 3 by 2 to the right
    blockA_temp = block[8];
    blockB_temp = block[9];
    blockC_temp = block[10];
    blockD_temp = block[11];
    block[8] = blockC_temp;
    block[9] = blockD_temp;
    block[10] = blockA_temp;
    block[11] = blockB_temp;
    // Rotate row 4 by 3 to the right
    blockA_temp = block[12];
    blockB_temp = block[13];
    blockC_temp = block[14];
    blockD_temp = block[15];
    block[12] = blockB_temp;
    block[13] = blockC_temp;
    block[14] = blockD_temp;
    block[15] = blockA_temp;
}

// Substitues each byte in block for its value in aes_s_box 
void inverse_sub_bytes(byte_t *block) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        byte_t left_nibble = block[i] >> 4;
        byte_t right_nibble = block[i] & 15;
        block[i] = inverse_aes_s_box[left_nibble * MATRIX_SIZE + right_nibble];
    }
}

// multiplies block by rijndael_galois_field matrix
void inverse_mix_columns(byte_t *block) {
    mix_columns_helper(block, inverse_rijndael_galois_field);
}

// Decryption uses Round Keys in reverse order
// Get Round Key after encryption of block
byte_t *get_round_key_after_encryption(const byte_t *key) {
    byte_t *round_key_after_encryption = malloc(MATRIX_SIZE * sizeof(byte_t));
    if (round_key_after_encryption == NULL) {
        perror("FAILED TO ASSIGN MEMORY DURING KEY DECRYPTION\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MATRIX_SIZE; i++) {
        round_key_after_encryption[i] = key[i];
    }
    int rcon_index = 1;
    for (int i = 0; i < RIJNDAEL_ROUNDS; i++) {
        aes_key_schedule(round_key_after_encryption, &rcon_index);
        rcon_index++;
    }

    return round_key_after_encryption;
}

// Gets previous round key by performing inverse operation of aes_key_schedule
void inverse_aes_key_schedule(byte_t *key, int *rcon_index) {
    byte_t prev_key[MATRIX_SIZE];
    // calculate 4th column of previous matrix (XOR 3rd and 4th column of key)
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        prev_key[i * MATRIX_DIMENSION + 3]
            = key[i * MATRIX_DIMENSION + 3] ^ key[i * MATRIX_DIMENSION + 2];
    }
    // calculate 3rd column of previous matrix (XOR 2nd and 3rd column of key)
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        prev_key[i * MATRIX_DIMENSION + 2]
            = key[i * MATRIX_DIMENSION + 2] ^ key[i * MATRIX_DIMENSION + 1];
    }
    // calculate 2nd column of previous matrix (XOR 1st and 2nd column of key)
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        prev_key[i * MATRIX_DIMENSION + 1]
            = key[i * MATRIX_DIMENSION + 1] ^ key[i * MATRIX_DIMENSION];
    }
    // calculate 1st column of previous matrix (XOR 1st and 2nd column of key)
    byte_t prev_key_col4[MATRIX_DIMENSION]; // store 4th column for easier calculation
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        prev_key_col4[i] = prev_key[i * MATRIX_DIMENSION + 3];
    }
    rotate(prev_key_col4);
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        byte_t left_nibble = prev_key_col4[i] >> 4;
        byte_t right_nibble = prev_key_col4[i] & 15;
        prev_key_col4[i] = aes_s_box[left_nibble * MATRIX_SIZE + right_nibble];
    }
    prev_key_col4[0] ^= rcon(*rcon_index);
    for (int i = 0; i < MATRIX_DIMENSION; i++) {
        prev_key[i * MATRIX_DIMENSION]
            = prev_key_col4[i] ^ key[i * MATRIX_DIMENSION];
    }
    for (int i = 0; i < MATRIX_SIZE; i++) {
        key[i] = prev_key[i];
    }
}