#ifndef ENCRYPT_HPP
#define ENCRYPT_HPP

extern "C"
{
    #include "aes.h"
}

void xcrypt_buffer(uint8_t *key, uint8_t *buffer, size_t buffer_size);

#endif // ENCRYPT_HPP