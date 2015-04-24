#include <hidef.h>      /* common defines and macros */
#include <mc9s12dt256.h>     /* derivative information */

//#include "pbs12dslk.h"
#include "lcd.h"

#pragma LINK_INFO DERIVATIVE "mc9s12dt256"



#define MASK_ON 0x01
#define MASK_OFF 0x02
#define MASK_BIT0_POS1 0x01
#define MASK_BIT1_POS2 0x02
#define MASK_BIT2_POS3 0x04
#define MASK_BIT3_POS4 0x08

void wait_local(unsigned int n);
void wait1000_local(void);

int atoi_local(char* s);
void itoa_local(int i, char* buffer);
void string_reverse_local(char* string);

void lcd_displaystring(char s[]);


#define TRUE 1
#define FALSE 0
#define KEYPAD_ON 1
#define KEYPAD_OFF 0
#define MAXBUFFER 50

void keypad_getstring(void);
char keypad_get(void);
char keypad_scan(void);
void keypad_wait(void);

char keypad_status;
char keypad_buffer[MAXBUFFER];
//char keypad_buffer_status;


//enum KeypadStatus {
//	KEYPAD_OFF,
//	KEYPAD_ON
//	}
//
//KeypadStatus keypad_status = KEYPAD_OFF;
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

//unsigned char result_array[4][4];



void main(void)
{

int count;
char buffer[MAXBUFFER];

DDRP = 0x00;
PERP = 0x01; // 0001
PPSP = 0x01; // 0001
PIEP = 0x01; // 0001

// DDRT = 0x0F;
// PERT = 0xF0;
// PPST = 0x00;

EnableInterrupts;

LCDInit();
LCDClearDisplay();

keypad_status = KEYPAD_OFF;
count = 200;
while (1) {
	if (keypad_status == KEYPAD_ON) {
		LCDClearDisplay();
		keypad_getstring();

		if (keypad_buffer[0] == 'A') {
			count = atoi_local(keypad_buffer + 1);
			}
		}

	itoa_local(count, buffer);
	LCDClearDisplay();
	lcd_displaystring(buffer);

	wait1000_local();

	count--;
	if (count == 0) {
		LCDClearDisplay();
		lcd_displaystring("done");
		while (1) {}
		}
	}
}




void interrupt 56 func_P(void)
{
if (PIFP & MASK_BIT0_POS1) {

	if (keypad_status == KEYPAD_OFF) {
		keypad_status = KEYPAD_ON;
		}
	else {
		keypad_status = KEYPAD_OFF;
		}


//	if (keypad_status == KEYPAD_OFF) {
//		keypad_status = KEYPAD_ON;
//		PPSP = 0x00;
//		}
//	else {
//		keypad_status = KEYPAD_OFF;
//		PPSP = 0x01;
//		}

	PIFP = MASK_BIT0_POS1;
	}
}




void keypad_getstring(void)
{
char key;
unsigned char x;

// DDRP = 0x00;
// PERP = 0x01; // 0001
// PPSP = 0x01; // 0001
// PIEP = 0x01; // 0001

DDRT = 0x0F;
PERT = 0xF0;
PPST = 0x00;

keypad_buffer[0] = 0;
// keypad_buffer_status = BUFFER_EMPTY;

x = 0;
	// indicates current character
while (1) {
	wait_local(100);
	key = keypad_get();

	// check if read status has been changed
	if (keypad_status == KEYPAD_OFF)
		break;
			// only way to exit loop

	if (key != 0) {

//		if (key == ?)
			// could be added

		keypad_buffer[x] = key;
		LCDPutChar(key);
		x++;
		if (x >= MAXBUFFER) {
			x = 0;
			// could be MAXBUFFER if previous chars want
			// to be saved
			keypad_status = KEYPAD_OFF;
			break;
			}
		}

	wait1000_local();
	keypad_wait();
	}

keypad_buffer[x] = '\0';
// keypad_buffer_status = BUFFER_FULL;
	// buffer gets set to full whether filled with chars or not
return;
}




char keypad_get(void)
{
char key;

key = 0;
// runs a scan for a key and then checks flags
// if key is zero then there was no key press or multiple key presses
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
		// initialize the current row
	wait_local(10);
		// wait a few seconds to let the port take on the new values
	while (column < 4) {
		if (PTT == matrix_array[row][column]) {
			key = key_array[row][column];
			// check if there was already a key press
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






void wait_local(unsigned int n)
{
for (; n > 0; n--) {}
return;
}


void wait1000_local(void)
{
int i, j;
for (i = 0; i < 500; i++) {
	for (j = 0; j < 500; j++) {}
	}
return;
}




void lcd_displaystring(char s[])
{
int x;

x = 0;
while (s[x] != '\0') {
	LCDPutChar(s[x]);
	x++;
	}

return;
}




int atoi_local(char* s)
{
int i;
unsigned char x;

i = 0;
x = 0;
while (s[x] != '\0') {
	i = i*10 + (s[x] - '0');
	}

return i;
}




void itoa_local(int i, char* buffer)
{
unsigned char x;
int remainder;

x = 0;
while (i != 0) {
	remainder = i % 10;
	buffer[x] = remainder + '0';
	i = i/10;
	x++;
	}
buffer[x] = '\0';
string_reverse_local(buffer);
return;
}


void string_reverse_local(char* string)
{
char* pe;
char* ps;

ps = (char*) &(string[0]);
pe = ps;
while (*(pe + 1) != '\0') {
	pe = pe + 1;
	}

while (ps < pe) {
	*ps = *ps ^ *pe;
	*pe = *ps ^ *pe;
	*ps = *ps ^ *pe;
	ps = ps + 1;
	pe = pe - 1;
	}
return;
}




