#ifndef ALE_GRAPH_H_
#define ALE_GRAPH_H_
#include "stm32fxxx_hal.h"
#include "ALE_LCD.h"

typedef struct{
	uint16_t x0;
	uint16_t y0;
	uint16_t width;
	uint16_t height;
	int16_t xMin;
	int16_t xMax;
	uint8_t xDiv;
	int16_t yMin;
	int16_t yMax;
	uint8_t yDiv;
	char *xlabel;
	char *ylabel;
	uint16_t xeje0;
	uint16_t xejeF;
	uint16_t yeje0;
	uint16_t yejeF;
	float xdiv;
	float ydiv;
	float xdivT;
	float ydivT;
	float mx;
	float bx;
	float my;
	float by;
	
	uint8_t xc;
	uint8_t yc;
	uint8_t xl;
	uint8_t yl;
}GRAPH_t;

void graph_init(GRAPH_t *graph, uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, int16_t xMin, int16_t xMax, uint8_t xDiv, int16_t yMin, int16_t yMax, uint8_t yDiv,char *xlabel,char *ylabel);
void drawGraph(GRAPH_t *graph);
void graphLine(GRAPH_t *graph, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color);
void graphXY(GRAPH_t *graph,int16_t x, int16_t y,uint32_t color);
#endif
