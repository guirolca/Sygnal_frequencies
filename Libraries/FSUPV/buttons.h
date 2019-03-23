#include "stm32fxxx_hal.h"
#include "tm_stm32_touch.h"
#include "tm_stm32_lcd.h"
#define MAX_BUTTONS 20

#define GRAY25 0x39E7
#define GRAY50 0x7BEF
#define GRAY75 0xBDF7


typedef enum {
	STATE_NOTHING=0,
	STATE_OVER,
}ButtonSTATE;
typedef struct {
	uint16_t x0;
	uint16_t y0;
	uint16_t width;
	uint16_t height;
	uint32_t back_color;
	uint32_t text_color;
	uint32_t line_color;
	uint32_t over_color;
	ButtonSTATE state;
	uint8_t active;
	void (* release)(void *but);
	void (* over)(void *but);
	void (* notover)(void *but);
	char str[100];
	uint16_t id;
}Button;

typedef struct {
	uint16_t x;
	uint16_t y;
}sizeString;

typedef enum {
	NOTHING=0,
	OVER,
	RELEASE,
}TouchMode;

void checkButtons(uint16_t X, uint16_t Y, TouchMode mode);

void checkTouch(void);

uint8_t createButton(Button *but, uint8_t active);
void ButtonsInit(void);
Button createDefaultButton (uint16_t x, uint16_t y, const char *str);	
void deactivateButton(Button *button);
void activateButton(Button *button);
