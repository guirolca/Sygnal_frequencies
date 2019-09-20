#ifndef MAIN__H
#define MAIN__H
typedef struct
{
	float Combustions;
	float SR_target;
	unsigned short SR_read;
}TC_Parameters;
typedef struct
{
	unsigned char GNGLL[40]; 
	unsigned char state;
	unsigned char dataavailable;
	unsigned char datavalid;
	unsigned char datamode;
} UbloxM8N;

typedef struct
{
	unsigned char data[BUFFERSIZE];
	unsigned int countW;
	unsigned int countR;
} BUFFER;

#endif
