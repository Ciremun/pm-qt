#ifndef COMMON_HPP
#define COMMON_HPP

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

extern "C"
{
#include "b64/b64.h"
#include "aes.h"
}

#include <QString>

#include "db.hpp"

#define LMAX 255
#define DATA_TABLE "data"

void decrypt_and_print(char *label);
void encrypt_and_write(uint8_t *data, size_t data_length);
void encrypt_and_replace(char *label, char *data, size_t data_length);
unsigned char* decode_line(const char *line);

#endif // COMMON_HPP