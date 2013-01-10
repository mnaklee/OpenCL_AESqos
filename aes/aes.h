extern int nr;
extern int nk;
extern unsigned char *key;

extern int fdi, fdo;
extern char *filename;
extern char *output_filename;
extern int file_size;
extern int offset_p;
extern volatile unsigned char *input_file_buffer;
extern volatile unsigned char temp_buffer[500000/4][4];
extern volatile unsigned char *output_file_buffer;

//#pragma omp threadprivate(nr,nk,key,offset_p,input_file_buffer,output_file_buffer)
