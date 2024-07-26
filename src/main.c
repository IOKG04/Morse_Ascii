#define MAX_D 6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINT_L fprintf(stderr, "%d\n", __LINE__)

const u_int8_t morse_codes[26] = {
    0b00100010, // a
    0b01000001, // b
    0b01000101, // c
    0b00110001, // d
    0b00010000, // e
    0b01000100, // f
    0b00110011, // g
    0b01000000, // h
    0b00100000, // i
    0b01001110, // j
    0b00110101, // k
    0b01000010, // l
    0b00100011, // m
    0b00100001, // n
    0b00110111, // o
    0b01000110, // p
    0b01001011, // q
    0b00110010, // r
    0b00110000, // s
    0b00010001, // t
    0b00110100, // u
    0b01001000, // v
    0b00110110, // w
    0b01001001, // x
    0b01001101, // y
    0b01000011, // z
    //lllldddd
};

typedef struct{
    u_int8_t l, d;
} mc_dat;

mc_dat get_dat(size_t ind){
    mc_dat outp = {0, 0};
    outp.l = morse_codes[ind] >> 4;
    u_int8_t d = morse_codes[ind] % 0x10;
    outp.d |= (d << 7) & 0b10000000;
    outp.d |= (d << 5) & 0b01000000;
    outp.d |= (d << 3) & 0b00100000;
    outp.d |= (d << 1) & 0b00010000;
    return outp;
}

u_int16_t lshift(u_int16_t nr, int8_t shift){
    if(shift >= 0) return nr << shift;
    else return nr >> (-shift);
}

void encode(char dest[MAX_D], const char src[2]){
    size_t ind[MAX_D] = {};
    u_int16_t inp = (src[0] << 9) | (src[1] << 2);

    while(ind[0] < 26){
	int i, l = 0;
	u_int16_t d = 0;
	for(i = 0; i < MAX_D - 1; ++i){
	    mc_dat dat = get_dat(ind[i]);
	    d |= lshift(dat.d, (8 - l));
	    l += dat.l;
	    if(l >= 14) break;
	}
	d &= 0b1111111111111100;
	if(d == inp){
	    for(int j = 0; j <= i; ++j){
		dest[j] = ind[j] + 65;
	    }
	    return;
	}
	
	for(int k = 0; k < MAX_D; ++k){
	    ind[k]++;
	    if(ind[k] < 26) break;
	    else ind[k] = 0;
	}
    }
    printf("cant find a combo 3:\n");
}

int main(int argc, char **argv){
    char *str = "wow ure smart :3";
    int len = strlen(str);

    for(int i = 0; i < len; i += 2){
	char dest[MAX_D] = {};
	char src[2] = {};
	memcpy(src, str + i, 2);
	encode(dest, src);
	for(int j = 0; j < MAX_D; ++j){
	    if(dest[j] == 0) break;
	    printf("%c", dest[j]);
	}
	printf(" ");
    }
    printf("\n");

    return 0;
}
