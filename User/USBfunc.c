#include "USBfunc.h"
#include "stdint.h"
#include "ALE_GPS.h"


void addStarttobuffer(BUFFER* buffer, unsigned char c)
{
	buffer->data[buffer->countR++]='D';
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]='a';
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]='t';
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]='a';
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]=c;
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
}

void addStoptobuffer(BUFFER* buffer)
{
	buffer->data[buffer->countR++]='S';
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]='t';
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]='o';
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]='p';
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
}

void add8tobuffer(unsigned char value, BUFFER* buffer)
{
	buffer->data[buffer->countR++]=(value&0xFF);
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
}

void add16tobuffer(unsigned int value,BUFFER* buffer)
{
	buffer->data[buffer->countR++]=((value>>8)&0xFF);
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]=(value&0xFF);
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
}

void add32tobuffer(unsigned long value,BUFFER* buffer)
{
	buffer->data[buffer->countR++]=(value>>24);
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]=((value>>16)&0xFF);
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]=((value>>8)&0xFF);
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	buffer->data[buffer->countR++]=(value&0xFF);
	if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
}

void addGPStobuffer(UbloxM8N* gps,BUFFER* buffer)
{
	int32_t lat=0, lon=0;
	
}

void addGPS0tobuffer(BUFFER* buffer)
{
	uint8_t i=0;
	for(i=0; i<24; i++)
	{
		buffer->data[buffer->countR++]=0;
	  if(buffer->countR==BUFFERSIZE){ buffer->countR=0;}
	}
}


