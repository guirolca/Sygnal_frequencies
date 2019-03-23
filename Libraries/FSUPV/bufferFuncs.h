void buffer_append_int32(unsigned char * buffer, int number, int *index);
void buffer_append_int16(unsigned char * buffer, int number, int *index);
void buffer_append_float32(unsigned char * buffer, float number, float scale, int *index);
void buffer_append_float16(unsigned char * buffer, float number, float scale, int *index);
void buffer_append(unsigned char * buffer, unsigned char *data, int len, int *index);
