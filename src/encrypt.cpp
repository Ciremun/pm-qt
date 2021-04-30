#include "encrypt.hpp"

struct AES_ctx ctx;
uint8_t aes_iv[] = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};

void xcrypt_buffer(uint8_t *key, uint8_t *buffer, size_t buffer_size)
{
    AES_init_ctx_iv(&ctx, key, aes_iv);
    AES_CTR_xcrypt_buffer(&ctx, buffer, buffer_size);
}