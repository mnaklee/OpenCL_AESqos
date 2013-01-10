#include "crypter.h"
#include "common.h"

#include <stdio.h>
int offset_p;
//#pragma omp threadprivate(offset_p)
int get_sbox_value(int num)
{
	int sbox[256] = {
		//0     1    2      3     4    5     6     7      8     9      A     B     C     D     E     F
		0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,	//0
		0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,	//1
		0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,	//2
		0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,	//3
		0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,	//4
		0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,	//5
		0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,	//6
		0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,	//7
		0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,	//8
		0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,	//9
		0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,	//A
		0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,	//B
		0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,	//C
		0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,	//D
		0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,	//E
		0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16        //F
	};			
	return sbox[num];
}

// The sub_bytes Function Substitutes the values in the
// state matrix with values in an S-box.
void sub_bytes()
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			state[offset_p+i][j] = get_sbox_value(state[offset_p+i][j]);

		}
	}
}

// The shift_rows() function shifts the rows in the state to the left.
// Each row is shifted with different offset.
// Offset = Row number. So the first row is not shifted.
void shift_rows()
{
	unsigned char temp;

	// Rotate second row 1 columns to left   
	temp = state[offset_p+1][0];
	state[offset_p+1][0] = state[offset_p+1][1];
	state[offset_p+1][1] = state[offset_p+1][2];
	state[offset_p+1][2] = state[offset_p+1][3];
	state[offset_p+1][3] = temp;

	// Rotate third row 2 columns to left  
	temp = state[offset_p+2][0];
	state[offset_p+2][0] = state[offset_p+2][2];
	state[offset_p+2][2] = temp;

	temp = state[offset_p+2][1];
	state[offset_p+2][1] = state[offset_p+2][3];
	state[offset_p+2][3] = temp;

	// Rotate fourth row 3 columns to left
	temp = state[offset_p+3][0];
	state[offset_p+3][0] = state[offset_p+3][3];
	state[offset_p+3][3] = state[offset_p+3][2];
	state[offset_p+3][2] = state[offset_p+3][1];
	state[offset_p+3][1] = temp;
}

// MixColumns function mixes the columns of the state matrix
void mix_columns()
{
	int i;
	unsigned char tmp, tm, t;
	for (i = 0; i < 4; i++) {
		t = state[offset_p+0][i];
		tmp = state[offset_p+0][i] ^ state[offset_p+1][i] ^ state[offset_p+2][i] ^ state[offset_p+3][i];
		tm = state[offset_p+0][i] ^ state[offset_p+1][i];
		tm = xtime(tm);
		state[offset_p+0][i] ^= tm ^ tmp;
		tm = state[offset_p+1][i] ^ state[offset_p+2][i];
		tm = xtime(tm);
		state[offset_p+1][i] ^= tm ^ tmp;
		tm = state[offset_p+2][i] ^ state[offset_p+3][i];
		tm = xtime(tm);
		state[offset_p+2][i] ^= tm ^ tmp;
		tm = state[offset_p+3][i] ^ t;
		tm = xtime(tm);
		state[offset_p+3][i] ^= tm ^ tmp;
	}
}

// Cipher is the main function that encrypts the PlainText.
int cipher(int offset)
{
	int i, j, round = 0;
    offset_p=offset;
	//Copy the input PlainText to state array.
	for (i = offset; i < offset+4; i++) {
		for (j = 0; j < 4; j++) {
			state[i][j] = temp_buffer[i][j];
		}
	}

#ifdef SHIFT_ROWS
	for (round = 0; round <= nr; round++)
		shift_rows();
#elif defined(MIX_COLUMNS)
	for (round = 0; round <= nr; round++)
		mix_columns();
#elif defined(ADD_ROUND_KEY)
	for (round = 0; round <= nr; round++)
		add_round_key(nr);
#elif defined(SUB_BYTES)
	for (round = 0; round <= nr; round++)
		sub_bytes();
#else
	// Add the First round key to the state before starting the rounds.
	add_round_key(0);

	// There will be nr rounds.
	// The first nr-1 rounds are identical.
	// These nr-1 rounds are executed in the loop below.
	for (round = 1; round < nr; round++) {
		sub_bytes();
		shift_rows();
		mix_columns();
		add_round_key(round);
	}

	// The last round is given below.
	// The MixColumns function is not here in the last round.
	sub_bytes();
	shift_rows();
	add_round_key(nr);
#endif
	// The encryption process is over.
	// Copy the state array to output array.
/*	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			output_file_buffer[offset + i * 4 + j] = state[offset+i][j];
		}
	}*/
    return 0;
}
