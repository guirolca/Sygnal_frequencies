#include "ALE_Buffer.h"

void ALE_Buffer_Putc(buffer_t *buffer, char c){
	buffer->buffer[buffer->pos_in++]=c;
	buffer->size++;
	if (buffer->pos_in==MAXBUFFER){
		buffer->pos_in=0;
	}
	if (buffer->size>=buffer->max_size){
		if (buffer->full_func){
			buffer->full_func((buffer_t *) buffer);
		}
	}
}
void ALE_Buffer_write(buffer_t *buffer, char *data, unsigned int len){
	unsigned int i=0;
	for (i=0;i<len;i++){
		ALE_Buffer_Putc(buffer,data[i]);
	}
}

void ALE_Buffer_Puts(buffer_t *buffer, char *str){
	while (*str){
		ALE_Buffer_Putc(buffer, (char)(*str++));
	}
}

void ALE_Buffer_init(buffer_t *buffer, unsigned int max_size){
	buffer->pos_in=0;
	buffer->pos_out=0;
	buffer->size=0;
	if (max_size>=MAXBUFFER){
		buffer->max_size=MAXBUFFER;
	}else{
		buffer->max_size=max_size;
	}	
}

int ALE_Buffer_Getc(buffer_t *buffer, char *c){
	int a=0;
	if (buffer->size>0){
		*c = buffer->buffer[buffer->pos_out++];
		a++;
		buffer->size--;
		if (buffer->pos_out==MAXBUFFER){
			buffer->pos_out=0;
		}
	}
	return a;
}

int ALE_Buffer_Read(buffer_t *buffer, char *c, unsigned int len){
	int a=0,i=0;
	for (i=0;i<len;i++){
		if (buffer->size>0){
			*c++ = buffer->buffer[buffer->pos_out++];
			a++;
			buffer->size--;
			if (buffer->pos_out==MAXBUFFER){
				buffer->pos_out=0;
			}
		}else{
			return a;
		}
	}
	return a;
}
void ALE_Buffer_Flush(buffer_t *buffer){
	int i;
	for(i=0;i<MAXBUFFER;i++){
		buffer->buffer[i]=0;
	}
	buffer->pos_in=0;
	buffer->pos_out=0;
	buffer->size=0;
}

void buffer_append_int32(unsigned char* buffer, int number, unsigned int *index) {
	buffer[(*index)++] = number >> 24;
	buffer[(*index)++] = number >> 16;
	buffer[(*index)++] = number >> 8;
	buffer[(*index)++] = number;
}

void buffer_append_int16(unsigned char* buffer, int number, unsigned int *index) {
	buffer[(*index)++] = number >> 8;
	buffer[(*index)++] = number;
}

void buffer_append_float32(unsigned char* buffer, float number, unsigned int *index, float scale) {
	int num;
	num = (int)(number*scale);
	buffer[(*index)++] = num >> 24;
	buffer[(*index)++] = num >> 16;
	buffer[(*index)++] = num >> 8;
	buffer[(*index)++] = num;
}

float buffer2float16(unsigned char *data, unsigned int *ind, float scale){
	float fdata=0;
	fdata = data[(*ind)++]<<8;
	fdata += data[(*ind)++];
	fdata /=scale;
	return fdata;
}

float buffer2float32(unsigned char *data, unsigned int *ind, float scale){
	float fdata=0;
	fdata = data[(*ind)++]<<24;
	fdata += data[(*ind)++]<<16;
	fdata += data[(*ind)++]<<8;
	fdata += data[(*ind)++];
	fdata /=scale;
	return fdata;
}

int buffer2int32(unsigned char *data, unsigned int *ind){
	int fdata=0;
	fdata = data[(*ind)++]<<24;
	fdata += data[(*ind)++]<<16;
	fdata += data[(*ind)++]<<8;
	fdata += data[(*ind)++];
	return fdata;
}

short int buffer2int16(unsigned char *data, unsigned int *ind){
	short int fdata=0;
	fdata = data[(*ind)++]<<8;
	fdata += data[(*ind)++];
	return fdata;
}


