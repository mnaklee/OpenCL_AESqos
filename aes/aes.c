
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <omp.h>

#include <sched.h>

#include "sha256.h"
#include "common.h"
#include "aes.h"
#include "crypter.h"
#include "decrypter.h"
#include "util.h"


sint64 start_time;
float elapsed_time;
int end_point=FALSE;

unsigned char *hash_password(char *password, size_t size)
{
	unsigned char *hash = (unsigned char *) malloc(size * sizeof(unsigned char));

	SHA256_CONTEXT context;
	sha256_init(&context);
	sha256_write(&context, (unsigned char *) password, strlen(password));
	sha256_final(&context);

	memcpy(hash, sha256_read(&context), size);
	

	return hash;
}

int main(int argc, char *argv[])
{
	if (argc < 5) {
		printf("Usage: claes <encrypt|decrypt> <input_file> <output_file> <key_size> <password>\n");
		return 0;
	}
    cpu_set_t my_set;
    CPU_ZERO(&my_set);
    CPU_SET(4,&my_set);
    CPU_SET(5,&my_set);
    CPU_SET(6,&my_set);
    CPU_SET(7,&my_set);
    sched_setaffinity(0,sizeof(cpu_set_t),&my_set);


    double avg_watts=0;
    int cnt = 0;

    pthread_t powermon_thr;
    powermon_args pa;

	char *action = argv[1];
	filename = argv[2];
	output_filename = argv[3];

//    pragma omp threadprivate(nr,nk,key,input_file_buffer,temp_buffer,output_file_buffer,state)

//    #pragma omp_set_num_threads(4);

	if (strcmp(action, "encrypt") != 0 && strcmp(action, "decrypt") != 0) {
		printf("Specify the right action: crypt or decrypt\n");
		printf("Usage: claes <crypt|decrypt> file\n");
		return 0;
	}

	int key_bits = nr = atoi(argv[4]);

	nk = nr / 32;
	nr = nk + 6;

	char *password = argv[5];
	printf("The password used to encrypt \"%s\" file is '%s'\n", filename, password);
	key = hash_password(password, nk * 4);
	
	printf("password (%d) ", (int)strlen(password));
	for (size_t i = 0; i < strlen(password); ++i)
		printf("%X ", (unsigned char)password[i]);
	printf("\n");
	printf("password hash (%u) ", nk * 4);
	for (size_t i = 0; i < nk * 4; ++i)
		printf("%X ", key[i]);
	printf("\n");

    /* create a power monitor thread. */
    pa.n_samples = 5;
    pa.interval = 1;    //seconds
    pa.avg_watts = &avg_watts;
    
    
    if (!open_files(filename)) {
	    printf("File doesn't exist\n");
   		return 0;
    }
    alloc_files();
    
    key_expansion();

    if (pthread_create(&powermon_thr, NULL, monitor_power, &pa) != 0)
         {
             perror("creating powermon_thr");
             exit(1);
         }

    start_time = now();
    while(cnt<2000)	{
//        fprintf(stderr,"%d\n",cnt);
   	
/*        if (!open_files(filename)) {
   		    printf("File doesn't exist\n");
       		return 0;
   	    }
    	alloc_files();
    
    	key_expansion();*/
    
    	int offset = 0;
        int pos;
        int off_cnt=0;
        int l,k;
        int i,j;
        int result;
//        #pragma omp parallel for schedule(static)
        for (l = 0; l < file_size/16; l++)
        {
        	for (k = 0; k < 16; k++) {
    	    	pos = k % 16;
        		i = pos / 4;
        		j = pos % 4;
//        		offset = off_cnt - pos;
           		temp_buffer[l+i][j] = input_file_buffer[l+k];
//                off_cnt=l+k;        
        		if (pos == 15) {
           			if (strcmp(action, "encrypt") == 0) {
                        result=cipher(l);
        			} else {
//        				inv_cipher(l);
        			}
        
        		}
            }
    	}
        close_file(fdi);
        close_file(fdo);
        cnt++;
    }
    end_point=TRUE;
	elapsed_time = (now() - start_time) / 1000.0;

	if (strcmp(action, "encrypt") == 0) {
		printf("Encrypted file written in \"%s\"\n", output_filename);
	} else {
		printf("Decrypted file written in \"%s\"\n", output_filename);
	}

	printf("--\n");
	printf("### Time of computation %f ms ###\n", elapsed_time/(cnt));
    printf("### Total energy %f ##\n", avg_watts);
    printf("### Total count %d ##\n", cnt);

	return 0;
}
