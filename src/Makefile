default:
	gcc -MMD -O2 -Wall -g -I. -lcrypto -o wpa_passphrase sha1.c sha1-internal.c sha1-pbkdf2.c wpa_passphrase.c -ldl -lz

clean:
	-rm -rf wpa_passphrase
	-rm -rf wpa_passphrase.d
