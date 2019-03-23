#include "buttons.h"
Button *buttonList[MAX_BUTTONS];
uint16_t buttoncount=0;
TM_TOUCH_t TS;
uint16_t lastX;
uint16_t lastY;
uint8_t touched=0;

void overButton(void *but);
void redrawButton(void *but);

uint16_t getIndex(void);

uint16_t getIndex(void){
	uint16_t i=0;
	for (i=0;i<MAX_BUTTONS;i++){
		if (buttonList[i]==NULL){
			return i;
		}
	}
	return MAX_BUTTONS;
}



void checkTouch(void){
	TM_TOUCH_Read(&TS);
		
		/* Check if pressed */
	if (TS.NumPresses) {
		checkButtons(TS.X[0],TS.Y[0],OVER);
		touched=1;
		lastX = TS.X[0];
		lastY = TS.Y[0];
	}else{
		if (touched){
			checkButtons(lastX,lastY,RELEASE);
			touched = 0;
		}
	}
}
void ButtonsInit(void){
	TS.Orientation = TM_LCD_GetOrientation();
	TM_TOUCH_Init(NULL, &TS);
}
uint8_t createButton(Button *but,uint8_t active){
//	uint16_t lenstr;
//	uint16_t xstr;
//	uint16_t xc,yc;
	uint16_t index;
	if (buttoncount==MAX_BUTTONS){
		return 0;
	}
	index = getIndex();
	if (index == MAX_BUTTONS){
		return 0;
	}
	buttonList[index]=but;
	buttoncount++;
	but->over = overButton;
	but->notover = redrawButton;
	but->state = STATE_NOTHING;
	but->active=active;
	redrawButton((Button *) but);
	
	return 1;
}

Button createDefaultButton (uint16_t x, uint16_t y, const char *str){
	Button button;
	button.x0=x;
	button.y0=y;
	button.width=100;
	button.height=32;
	button.back_color=LCD_COLOR_BLUE2;
	button.line_color=LCD_COLOR_BLACK;
	button.over_color=LCD_COLOR_WHITE;
	button.text_color=LCD_COLOR_BLACK;
	sprintf(button.str,"%s",str);
	return button;
}

void overButton(void *button){
	
	uint16_t lenstr;
	uint16_t xstr;
	uint16_t xc,yc;
	Button *but = (Button *) button;
	xc = (but->x0) + ((but->width)>>1);
	yc = (but->y0) + ((but->height)>>1);
	lenstr=strlen(but->str);
	xstr = (7*lenstr)>>1;
	TM_LCD_DrawFilledRectangle(but->x0,but->y0,but->width,but->height,but->over_color);
	TM_LCD_DrawRectangle(but->x0,but->y0,but->width,but->height,but->line_color);
	TM_LCD_SetXY(xc-xstr,yc-5);
	TM_LCD_SetFont(&TM_Font_7x10);
	TM_LCD_SetColors(but->text_color,but->over_color);
	TM_LCD_Puts(but->str);
}

void redrawButton(void *button){
	uint16_t lenstr;
	uint16_t xstr;
	uint16_t xc,yc;
	Button *but = (Button *) button;
	xc = (but->x0) + ((but->width)>>1);
	yc = (but->y0) + ((but->height)>>1);
	lenstr=strlen(but->str);
	xstr = (7*lenstr)>>1;
	if (but->active){
		TM_LCD_DrawFilledRectangle(but->x0,but->y0,but->width,but->height,but->back_color);
		TM_LCD_DrawRectangle(but->x0,but->y0,but->width,but->height,but->line_color);
		TM_LCD_SetXY(xc-xstr,yc-5);
		TM_LCD_SetFont(&TM_Font_7x10);
		TM_LCD_SetColors(but->text_color,but->back_color);
		TM_LCD_Puts(but->str);
	
	}else{
		TM_LCD_DrawFilledRectangle(but->x0,but->y0,but->width,but->height,GRAY50);
		TM_LCD_DrawRectangle(but->x0,but->y0,but->width,but->height,GRAY75);
		TM_LCD_SetXY(xc-xstr,yc-5);
		TM_LCD_SetFont(&TM_Font_7x10);
		TM_LCD_SetColors(GRAY25,GRAY50);
		TM_LCD_Puts(but->str);
	}
	
}

uint8_t checkButtonPressed(uint16_t X, uint16_t Y, Button *but){
	if (X>=but->x0 && X<=(but->x0+but->width) && Y>=but->y0 && Y<=(but->y0+but->height)){
		return 1;
	}
	return 0;
}
void checkButtons(uint16_t X, uint16_t Y, TouchMode mode){
	int i=0;
	for(i=0;i<buttoncount;i++){
		if (buttonList[i]->active){
			if (checkButtonPressed(X,Y,buttonList[i])){
				switch (mode){
					case OVER:
						if (buttonList[i]->state != STATE_OVER){
							buttonList[i]->over(buttonList[i]);
							buttonList[i]->state = STATE_OVER;
						}
					break;
					case RELEASE:
						buttonList[i]->notover(buttonList[i]);
						buttonList[i]->release(buttonList[i]);
						buttonList[i]->state = STATE_NOTHING;
					default:
						break;
				}
			}else{
				if (buttonList[i]->state != STATE_NOTHING){
					buttonList[i]->notover(buttonList[i]);
					buttonList[i]->state = STATE_NOTHING;
				}
			}
					
				
		}
	
	}
}
void deactivateButton(Button *button){
	button->active=0;
	redrawButton(button);
}
void activateButton(Button *button){
	button->active=1;
	redrawButton(button);
}

