#ifndef WPA_PASSPHRASE_H
#define WPA_PASSPHRASE_H

#include <cstdint>
#include <cstdlib>
#include <cstdio>

typedef uint8_t u8;
#include "../../src/sha1.h"

char *encode(const char *ssid, const size_t s_len, const char *passphrase, const size_t p_len);

#endif // WPA_PASSPHRASE_H
