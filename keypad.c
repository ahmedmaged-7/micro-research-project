
#include "tm4c123gh6pm.h"

#include <stdint.h>

void keypad_init(){
SYSCTL_RCGCGPIO_R |= 0x04; /* enable clock to GPIOC */
SYSCTL_RCGCGPIO_R |= 0x10; /* enable clock to GPIOE */
while((SYSCTL_PRGPIO_R & 0x014)==0){}	// do nothhng till PortC,E is activated
/*PORTE ARE ROWS , PORTC ARE COLUMNS*/
	
  GPIO_PORTE_AMSEL_R = 0x00;        //  disable analog 
  GPIO_PORTE_DIR_R |= 0x0000000F;          //  PE3-0 out
  GPIO_PORTE_AFSEL_R = 0x00;        //  disable alt funct 
  GPIO_PORTE_DEN_R |= 0x0F;          //  enable digital PE
  GPIO_PORTE_ODR_R |= 0x0F;       //FOR SAFETY
	
	GPIO_PORTC_AMSEL_R &= ~0xF0;        //  disable analog 
  GPIO_PORTC_DIR_R &= ~0xF0;          //  PC7-4 INPUT
  GPIO_PORTC_AFSEL_R &= ~0xF0;        //  disable alt funct 
  GPIO_PORTC_DEN_R |= 0xF0;          //  enable digital PE
	GPIO_PORTC_PUR_R |= 0xF0;          //  enable PULL UP RES PE

	
}


void delay(int z){
uint32_t s=z*100;
uint32_t i=0; 	
	while(i<s){
	
	i++;
	
	}

}


char keypad(void){
const char KEYPAD[4][3] = {
{ '1', '2', '3'},
{ '4', '5', '6'},
{ '7', '8', '9'},
{ '*', '0', '#'},
};
int row, col;
GPIO_PORTE_DATA_R = 0; /* enable all rows */
col = GPIO_PORTC_DATA_R & 0xF0; /* read all columns */
if (col == 0xF0) {return 0;}/* no key pressed */

while (1)
{
row = 0;
GPIO_PORTE_DATA_R = 0x0E; /* enable row 0 */
delay(1); 
col = GPIO_PORTC_DATA_R & 0xF0;
if (col != 0xF0) {break;}
row = 1;
GPIO_PORTE_DATA_R = 0x0D; /* enable row 1 */
delay(1); 
col = GPIO_PORTC_DATA_R & 0xF0;
if (col != 0xF0) {break;}
row = 2;
GPIO_PORTE_DATA_R = 0x0B; /* enable row 2 */
delay(2);
col = GPIO_PORTC_DATA_R & 0xF0;
if (col != 0xF0) {break;}
row = 3;
GPIO_PORTE_DATA_R = 0x07; /* enable row 3 */
delay(1); 
col = GPIO_PORTC_DATA_R & 0xF0;
if (col != 0xF0) {break;}
return 0; 
}
if (col == 0xE0) return KEYPAD[row][0];
if (col == 0xD0) return KEYPAD[row][1]; 
if (col == 0xB0) return KEYPAD[row][2];
return 0; 
}








char keypad_get_char(void){
char key;
do{
while(keypad() != 0);
delay(25);
}while(keypad() !=0	);

do{
key = keypad();
delay(25);
}while(keypad() !=key	);
return key;
}


