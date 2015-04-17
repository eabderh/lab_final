#include <hidef.h>      /* common defines and macros */
#include <mc9s12dt256.h>     /* derivative information */

//#include "pbs12dslk.h"
//#include "lcd.h"

#pragma LINK_INFO DERIVATIVE "mc9s12dt256"

int count = 0;


#define MASK_ON 0x01
#define MASK_OFF 0x02
#define MASK_BIT0_POS1 0x01
#define MASK_BIT1_POS2 0x02
#define MASK_BIT2_POS3 0x04

void wait_local(unsigned int n);



#define KEYPAD_MAXBUFFER 50

void keypad_process(void);
char keypad_get(void);
char keypad_scan(void);
void keypad_wait(void);


#define KEYPAD_ON 1
#define KEYPAD_OFF 0
#define TRUE 1
#define FALSE 0
//enum KeypadStatus {
//	KEYPAD_OFF,
//	KEYPAD_ON
//	}
//
//KeypadStatus keypad_status = KEYPAD_OFF;
char keypad_status;
char keypad_buffer[KEYPAD_MAXBUFFER];


//unsigned char column_array[4] = {
//	0xE
//
//unsigned char row_array[4] = {
//	0x0E, 0x0D, 0x0B, 0x07
//	};

unsigned char matrix_array[4][4] = {
	{0xEE, 0xDE, 0xBE, 0x7E},
	{0xED, 0xDD, 0xBD, 0x7D},
	{0xEB, 0xDB, 0xBB, 0x7B},
	{0xE7, 0xD7, 0xB7, 0x77},
	};

char key_array[4][4] = {
	{'A', '1', '2', '3'},
	{'B', '4', '5', '6'},
	{'C', '7', '8', '9'},
	{'D', '*', '0', '#'}
	};

unsigned char result_array[4][4];


void wait1000_local(void);

void main(void)
{

DDRP = 0x00;
PERP = 0x01; // 0001
PPSP = 0x01; // 0001
PIEP = 0x01; // 0001

DDRT = 0x0F;
PERT = 0xF0;
PPST = 0x00;

EnableInterrupts;

//LCDInit(void);
//LCDClearDisplay(void);


keypad_status = KEYPAD_OFF;

while (1) {
	if (keypad_status == KEYPAD_ON) {
		keypad_process();
		keypad_status = KEYPAD_OFF;
		}

	// do stuff

	}
}




void interrupt 56 func_P(void)
{

if (PIFP & MASK_ON) {
	keypad_status = KEYPAD_ON;
	PIFP = MASK_ON;
	}
}




void keypad_process(void)
{
char key;
unsigned char x;

DDRP = 0x00;
PERP = 0x01; // 0001
PPSP = 0x01; // 0001
PIEP = 0x01; // 0001

keypad_buffer[0] = 0;
x = 0;
while (1) {
	wait_local(100);
	key = keypad_get();
	if (keypad_status == KEYPAD_OFF) {
		keypad_buffer[x] = 0;
		break;
		}
	if (key != 0) {

//		if (keypad_mode == ?) 	could be added

		keypad_buffer[x] = key;
		x++;
		if (x >= KEYPAD_MAXBUFFER) {
			keypad_buffer[0] = 0; // could be KEYPAD_MAXBUFFER
			keypad_status = KEYPAD_OFF;
			break;
			}
		}
	wait_local(100);
	keypad_wait();
	}

return;
}




char keypad_get(void)
{
char key;

key = 0;
while ((key == 0) && (keypad_status == KEYPAD_ON)) {
	key = keypad_scan();
	}
return key;
}




char keypad_scan(void)
{
unsigned char row, column;
unsigned char hit;
char key;

key = 0;
hit = FALSE;
row = 0;
while (row < 4) {
	column = 0;
	PTT = matrix_array[row][0];
	wait_local(10);
	while (column < 4) {
		if (PTT == matrix_array[row][column]) {
			key = key_array[row][column];
			if (hit == TRUE) {
				return 0;
				}
			hit = TRUE;
			}
		column++;
		}
	row++;
	}

return key;
}




void keypad_wait(void)
{
PTT = 0x00;
while (PTT != 0x0F) {}
return;
}






void wait1000_local(void) {
int i, j;
for (i = 0; i < 500; i++) {
	for (j = 0; j < 500; j++) {}
	}
return;
}


void wait_local(unsigned int n)
{
for (; n > 0; n--) {}
return;
}


