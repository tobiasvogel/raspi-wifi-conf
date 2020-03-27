default:
	g++ -Wall -c -I. -I../../src ../../src/sha1.c ../../src/sha1-internal.c ../../src/sha1-pbkdf2.c wpa_passphrase.cpp -fpermissive -fPIC
	g++ -shared -o _libwpa_passphrase.so *.o -lcrypto -ldl -lz

static:
	g++ -Wall -c -I. -I../../src ../../src/sha1.c ../../src/sha1-internal.c ../../src/sha1-pbkdf2.c wpa_passphrase.cpp -fpermissive -fPIC
	ar crf _libwpa_passphrase.a *.o
	
clean:
	-rm -rf sha1.o
	-rm -rf sha1-internal.o
	-rm -rf sha1-pbkdf2.o
	-rm -rf wpa_passphrase.o
	-rm -rf _libwpa_passphrase.so
	-rm -rf _libwpa_passphrase.a
