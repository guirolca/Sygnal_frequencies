#ifndef USBFUNC_H
#define USBFUNC_H

#include "defines.h"
#include "Funciones.h"
#include "main.h"

void addStoptobuffer(BUFFER* buffer);
void addStarttobuffer(BUFFER* buffer, unsigned char c);
void addGPStobuffer(UbloxM8N* gps,BUFFER* buffer);
void addGPS0tobuffer(BUFFER* buffer);
void add8tobuffer(unsigned char value, BUFFER* buffer);
void add16tobuffer(unsigned int value,BUFFER* buffer);
void add32tobuffer(unsigned long value,BUFFER* buffer);

#endif
