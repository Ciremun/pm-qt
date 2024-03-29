#ifndef ENCRYPT_HPP
#define ENCRYPT_HPP

extern "C"
{
#include "aes.h"
#include "b64/b64.h"
}

#include <string>

#include "db.hpp"

void xcrypt_buffer(uint8_t *key, uint8_t *buffer, size_t buffer_size);
void decrypt_and_print();
void encrypt_and_write(std::string data_str);
void encrypt_and_write(std::string data_str, std::string label_str);
void encrypt_and_write(const char *label, uint8_t *data, size_t data_size);
unsigned char *decode_line(const char *line);

#endif // ENCRYPT_HPP