#include "ALE_Graph.h"

void graph_init(GRAPH_t *graph,uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, int16_t xMin, int16_t xMax, uint8_t xDiv, int16_t yMin, int16_t yMax, uint8_t yDiv,char *xlabel,char *ylabel){
	char str[5];
	sprintf(str,"%d",xMin);
	graph->xc=strlen(str);
	sprintf(str,"%d",xMax);
	if (strlen(str)>graph->xc){
		graph->xc=strlen(str);
	}
	sprintf(str,"%d",yMin);
	graph->yc=strlen(str);
	sprintf(str,"%d",yMax);
	if (strlen(str)>graph->yc){
		graph->yc=strlen(str);
	}
	graph->xl=strlen(xlabel);
	graph->yl=strlen(ylabel);
	graph->x0=x0;
	graph->y0=y0;
	graph->width=width;
	graph->height=height;
	graph->xMin=xMin;
	graph->xMax=xMax;
	graph->xDiv=xDiv;
	graph->yMin=yMin;
	graph->yMax=yMax;
	graph->yDiv=yDiv;
	graph->xlabel=xlabel;
	graph->ylabel=ylabel;
	graph->xeje0=graph->x0+graph->yc*7+10+15;
	graph->yeje0=graph->y0+graph->height-25-10;
	graph->xejeF=graph->x0+graph->width-5-(graph->xc*7)/2;
	graph->yejeF=graph->y0+10;
	graph->xdiv=(graph->xejeF-graph->xeje0)/(float)graph->xDiv;
	graph->ydiv=(graph->yeje0-graph->yejeF)/(float)graph->yDiv;
	graph->xdivT=(graph->xMax-graph->xMin)/(float)graph->xDiv;
	graph->ydivT=(graph->yMax-graph->yMin)/(float)graph->yDiv;
	graph->mx = (float)(graph->xejeF-graph->xeje0)/(float)(graph->xMax-graph->xMin);
	graph->my = (float)(graph->yejeF-graph->yeje0)/(float)(graph->yMax-graph->yMin);
	graph->bx = (float)graph->xeje0-graph->mx*(float)(graph->xMin);
	graph->by = (float)graph->yeje0-graph->my*(float)(graph->yMin);
	
}
	
void drawGraph(GRAPH_t *graph){ 
	int i=0;
	char str[5];

	
	
	TM_LCD_DrawFilledRectangle(graph->x0,graph->y0,graph->width,graph->height,LCD_COLOR_BLACK);
	TM_LCD_DrawRectangle(graph->x0,graph->y0,graph->width,graph->height,LCD_COLOR_YELLOW);
	TM_LCD_DrawLine(graph->xeje0,graph->yeje0,graph->xeje0,graph->yejeF,LCD_COLOR_WHITE);
	TM_LCD_DrawLine(graph->xeje0,graph->yeje0,graph->xejeF,graph->yeje0,LCD_COLOR_WHITE);
	TM_LCD_SetFont(&TM_Font_7x10);
	TM_LCD_SetColors(LCD_COLOR_WHITE,LCD_COLOR_BLACK);
	for (i=0;i<=graph->yDiv;i++){
		TM_LCD_DrawLine(graph->xeje0-5,graph->yeje0-(uint16_t)(i*graph->ydiv),graph->xeje0+5,graph->yeje0-(uint16_t)(i*graph->ydiv),LCD_COLOR_WHITE);
		sprintf(str,"%d",graph->yMin+(int16_t)(i*graph->ydivT));
		TM_LCD_SetXY(graph->xeje0-7*strlen(str)-5,graph->yeje0-(uint16_t)(i*graph->ydiv)-5);
		TM_LCD_Puts(str);
	}
	for (i=0;i<=graph->xDiv;i++){
		TM_LCD_DrawLine(graph->xeje0+(uint16_t)(i*graph->xdiv),graph->yeje0-5,graph->xeje0+(uint16_t)(i*graph->xdiv),graph->yeje0+5,LCD_COLOR_WHITE);
		sprintf(str,"%d",graph->xMin+(int16_t)(i*graph->xdivT));
		TM_LCD_SetXY(graph->xeje0+(uint16_t)(i*graph->xdiv)-(strlen(str)*7)/2,graph->yeje0+5),
		TM_LCD_Puts(str);
	}
	TM_LCD_SetXY(graph->xeje0-7*graph->yc-20,(graph->yeje0+graph->yejeF)/2+(graph->yl*7)/2);
	ALE_LCD_Puts_du(graph->ylabel);
	TM_LCD_SetXY((graph->xejeF+graph->xeje0)/2-(7*graph->xl)/2,graph->yeje0+5+15);
	TM_LCD_Puts(graph->xlabel);
}
void graphXY(GRAPH_t *graph,int16_t x, int16_t y,uint32_t color){
	float px,py;
	px = graph->mx*(float)x+graph->bx;
	py = graph->my*(float)y+graph->by;
	TM_LCD_DrawPixel((uint16_t)px,(uint16_t)py,color);
}

void graphLine(GRAPH_t *graph, int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint32_t color){
	float px0,py0,px1,py1;
	px0 = graph->mx*(float)x0+graph->bx;
	py0 = graph->my*(float)y0+graph->by;
	px1 = graph->mx*(float)x1+graph->bx;
	py1 = graph->my*(float)y1+graph->by;
	TM_LCD_DrawLine((uint16_t)px0, (uint16_t)py0, (uint16_t)px1, (uint16_t)py1, color);
}