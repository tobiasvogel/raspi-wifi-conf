/*
 * WPA Supplicant - ASCII passphrase to WPA PSK tool
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint8_t u8;
#include "sha1.h"

size_t os_strlen(const char *s) {
        const char *p = s;
        while (*p)
                p++;
        return p - s;
}

int has_ctrl_char(const u8 *data, size_t len) {
        size_t i;

        for (i = 0; i < len; i++) {
                if (data[i] < 32 || data[i] == 127)
                        return 1;
        }
        return 0;
}

int main(int argc, char *argv[])
{
	unsigned char psk[32];
	int i;
	char *ssid, *passphrase, buf[64], *pos;
	size_t len;

	if (argc < 2) {
		printf("usage: wpa_passphrase <ssid> [passphrase]\n"
			"\nIf passphrase is left out, it will be read from "
			"stdin\n");
		return 1;
	}

	ssid = argv[1];

	if (argc > 2) {
		passphrase = argv[2];
	} else {
		printf("# reading passphrase from stdin\n");
		if (fgets(buf, sizeof(buf), stdin) == NULL) {
			printf("Failed to read passphrase\n");
			return 1;
		}
		buf[sizeof(buf) - 1] = '\0';
		pos = buf;
		while (*pos != '\0') {
			if (*pos == '\r' || *pos == '\n') {
				*pos = '\0';
				break;
			}
			pos++;
		}
		passphrase = buf;
	}

	len = os_strlen(passphrase);
	if (len < 8 || len > 63) {
		printf("Passphrase must be 8..63 characters\n");
		return 1;
	}
	if (has_ctrl_char((u8 *) passphrase, len)) {
		printf("Invalid passphrase character\n");
		return 1;
	}

	pbkdf2_sha1(passphrase, (u8 *) ssid, os_strlen(ssid), 4096, psk, 32);

	printf("network={\n");
	printf("\tssid=\"%s\"\n", ssid);
	printf("\t#psk=\"%s\"\n", passphrase);
	printf("\tpsk=");
	for (i = 0; i < 32; i++)
		printf("%02x", psk[i]);
	printf("\n");
	printf("}\n");

	return 0;
}
