#ifndef ENCRYPT_UTILS_H
#define ENCRYPT_UTILS_H

extern void add_round_key(byte_t *block, byte_t *key);

extern void sub_bytes(byte_t *block);
extern void shift_rows(byte_t *block);
extern void mix_columns(byte_t *block);
extern void aes_key_schedule(byte_t *key, int *rcon_index);


extern void inverse_sub_bytes(byte_t *block);
extern void inverse_shift_rows(byte_t *block);
extern void inverse_mix_columns(byte_t *block);
extern void inverse_aes_key_schedule(byte_t *key, int *rcon_index);
extern byte_t *get_round_key_after_encryption(const byte_t *key);

#endif