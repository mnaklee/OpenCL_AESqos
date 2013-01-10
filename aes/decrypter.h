extern int nr;

extern volatile unsigned char *output_file_buffer;

extern volatile unsigned char state[500000/4][4];
extern volatile unsigned char temp_buffer[500000/4][4];
extern int offset_p;
//#pragma omp threadprivate(nr,output_file_buffer)
//Functions
int inv_get_sbox_value(int num);
void inv_sub_bytes();
void inv_shift_rows();
void inv_mix_columns();
void inv_cipher(int offset);
