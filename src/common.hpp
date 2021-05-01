#ifndef COMMON_HPP
#define COMMON_HPP

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

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

#define LMAX 255
#define DATA_STORE ".data"

ssize_t getline(char **lineptr, size_t *n, FILE *stream);
void read_file(const char *fp, char ***lines, size_t *lsize);
char *read_file_as_str(const char *fp, size_t *nch);
void write_file(const char *fp, const char *mode, void *data);
char** decrypt_and_print(uint8_t *key, size_t *total_lines, char *find_label);
void encrypt_and_write(uint8_t *key, uint8_t *data, size_t data_length);
void encrypt_and_replace(char *find_label, char *data, uint8_t *aes_key);

#endif // COMMON_HPP