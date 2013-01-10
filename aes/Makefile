ROOT_DIR = ..
CC = gcc -std=c99 -g
CFLAGS = -I$(ROOT_DIR)/smartmeter -lpthread -lrt -fopenmp -DOMP_STACKSIZE="1G"
LDFLAGS = -L$(ROOT_DIR)/libs -lsmartmeter -lusb-1.0
SOURCES = aes.c common.c crypter.c decrypter.c sha256.c util.c
TARGET = aes

all:
	$(CC) $(DEFINES) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)
	
clean:
	rm -f $(TARGET) *.o *~
