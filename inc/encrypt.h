#ifndef ENCRYPT_H
#define ENCRYPT_H

#include "encode.h"

/**
 * @brief Encrypts a given portion of data using AES
 *
 * @param encoded data to encrypt
 * @param encryption key to encrypt data with
 *
 * @returns void
 */
extern void encrypt(steno_data *encoded_data, char* key);

/**
 * @brief Decrypts a given portion of data using AES
 *
 * @param encoded data to decrypt
 * @param encryption key to decrypt data with
 *
 * @returns void
 */
extern void decrypt(steno_data *encoded_data, char* key);


#endif //ENCRYPT_H