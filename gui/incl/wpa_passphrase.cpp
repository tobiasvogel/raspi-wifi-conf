#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef uint8_t u8;
#include "sha1.h"
#include "wpa_passphrase.h"

char *bin2hex(const unsigned char *bin, size_t len) {

	char *out;

	size_t i;

	if (bin==NULL || len==0) {
		return NULL;
	}

	out = (char *)malloc(len*2+1);
	for (i=0; i <len; i++) {
		out[i*2]   = "0123456789ABCDEF"[bin[i] >> 4];
		out[i*2+1] = "0123456789ABCDEF"[bin[i] & 0x0F];
	}
	out[len*2] = '\0';
	return out;

}


char *encode(const char *ssid, const size_t s_len, const char *passphrase, const size_t p_len) {

	unsigned char psk[32];

	pbkdf2_sha1(passphrase, (u8 *) ssid, s_len, 4096, psk, 32);

	char *psk_array = bin2hex(&psk[0], 32);

	return psk_array;

}
