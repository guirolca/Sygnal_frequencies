#include "bufferFuncs.h"



void buffer_append_int32(unsigned char * buffer, int number, int *index) {
	buffer[(*index)++] = number >> 24;
	buffer[(*index)++] = number >> 16;
	buffer[(*index)++] = number >> 8;
	buffer[(*index)++] = number;
}

void buffer_append_int16(unsigned char * buffer, int number, int *index) {
	buffer[(*index)++] = number >> 8;
	buffer[(*index)++] = number;
}

void buffer_append_float32(unsigned char * buffer, float number, float scale, int *index){
	buffer_append_int32(buffer, (signed int)(number * scale), index);
}
	
void buffer_append_float16(unsigned char * buffer, float number, float scale, int *index){
	buffer_append_int16(buffer, (signed short int)(number * scale), index);
}

void buffer_append(unsigned char * buffer, unsigned char *data, int len, int *index){
	int i;
	for (i=0;i<len;i++){
		buffer[(*index)++] = *(data+i);
	}
}
