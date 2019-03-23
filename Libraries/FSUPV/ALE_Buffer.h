

#define MAXBUFFER 2048
typedef struct{
	char buffer[MAXBUFFER];
	unsigned int pos_in;
	unsigned int pos_out;
	unsigned int size;
	unsigned int max_size;
	void(*full_func)(void *params);
}buffer_t;

void ALE_Buffer_Putc(buffer_t *buffer, char c);
void ALE_Buffer_Puts(buffer_t *buffer, char *str);
void ALE_Buffer_write(buffer_t *buffer, char *data, unsigned int len);
void ALE_Buffer_init(buffer_t *buffer, unsigned int max_size);
int ALE_Buffer_Read(buffer_t *buffer, char *c, unsigned int len);
int ALE_Buffer_Getc(buffer_t *buffer, char *c);
void ALE_Buffer_Flush(buffer_t *buffer);
void buffer_append_int32(unsigned char* buffer, int number, unsigned int *index);
void buffer_append_int16(unsigned char* buffer, int number, unsigned int *index);
float buffer2float16(unsigned char *data, unsigned int *ind, float scale);
float buffer2float32(unsigned char *data, unsigned int *ind, float scale);
int buffer2int32(unsigned char *data, unsigned int *ind);
short int buffer2int16(unsigned char *data, unsigned int *ind);
void buffer_append_float32(unsigned char* buffer, float number, unsigned int *index, float scale);
