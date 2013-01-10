extern int nr;
extern volatile unsigned char *output_file_buffer;

extern volatile unsigned char state[500000/4][4];
extern volatile unsigned char temp_buffer[500000/4][4];
extern int offset_p;
//#pragma omp threadprivate(nr,output_file_buffer,offset_p)
//Functions
int get_sbox_value(int num);
void sub_bytes();
void shift_rows();
void mix_columns();
int cipher(int offset);
