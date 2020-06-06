#include <stdio.h>

#include <stdint.h>
#include "tm4c123gh6pm.h"


void uartinit(void){

SYSCTL_RCGCUART_R |=0x01 ; //ENABLE CLK UART0
	SYSCTL_RCGCGPIO_R |=0x01 ; //ENABLE CLK PORTA
	while((SYSCTL_PRGPIO_R & 0x01)==0){}
	UART0_CTL_R =0; //DISABLE UART0 
	UART0_CC_R=0x05;
	UART0_IBRD_R = 104; // BAUD RATE =9600
	UART0_FBRD_R= 11;
	UART0_LCRH_R = 0x070; //8-bit,NO PARITY ,enable fifo
	UART0_CTL_R = 0x301; //activate UART0
	GPIO_PORTA_AMSEL_R &= ~ 0x03;
	GPIO_PORTA_AFSEL_R |= 0x03;//SET alternative FUNCTION
	GPIO_PORTA_PCTL_R = ((GPIO_PORTA_PCTL_R & 0xFF00) + 0x0011);
	GPIO_PORTA_DEN_R |= 0x03;


}
void SystemInit(){}
void UART_Init(void){
  SYSCTL_RCGCUART_R |= 0x01;            // activate UART0
  SYSCTL_RCGCGPIO_R |= 0x01;            // activate port A
  while((SYSCTL_PRGPIO_R&0x01) == 0){};
  UART0_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  UART0_IBRD_R = 104;                    // IBRD = int(50,000,000 / (16 * 115,200)) = int(27.1267)
  UART0_FBRD_R = 11;                     // FBRD = int(0.1267 * 64 + 0.5) = 8
 // UART0_CC_R=0x05;
		UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART0_CTL_R |= 0x301;                 // enable UART
  GPIO_PORTA_AFSEL_R |= 0x03;           // enable alt funct on PA1-0
  GPIO_PORTA_DEN_R |= 0x03;             // enable digital I/O on PA1-0
                                        // configure PA1-0 as UART
  GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
  GPIO_PORTA_AMSEL_R &= ~0x03;          // disable analog functionality on PA
}
void UART_OutINT(int data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}
void UART_Outchar(char data){
  while((UART0_FR_R&UART_FR_TXFF) != 0);
  UART0_DR_R = data;
}
char UART_InChar(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return((char)(UART0_DR_R&0xFF));
}

int  UART_InInt(void){
  while((UART0_FR_R&UART_FR_RXFE) != 0);
  return((int)(UART0_DR_R&0xFF));
}
struct room{
int roomno;
char pass[4];
int status;	
	int doorstatus;
};
void printstring(char c[],int lenght){
int i=0;
	for(i=0;i<lenght;i++){
	UART_Outchar(c[i]);
	}
}
void printroom(struct room room1){
char d[5]={'d','o','o','r','_'};
char x[7]={'l','o','c','k','e','d',' '};
  char o[]={'o','p','e','n',' '};
	char str[5];
  sprintf(str, "%d", room1.roomno);
printstring(str,3);
	UART_Outchar(' ');
	
if(room1.status==1){
	
char c[]={'c','h','e','c','k','e','d',' ','i','n',' '};
 
	printstring(c,11);
if(room1.doorstatus==1){
	printstring(d,5);
	printstring(o,5);
	
}else if(room1.doorstatus==0){
 printstring(d,5);
 
	printstring(x,7);
}
}
else if(room1.status==0){
	printstring(x,7);

 printstring(d,5);
	printstring(x,7);
	
}
else if(room1.status==2){
char c[]={'s','e','r','v','i','c','e',' '};
	printstring(c,6);
 printstring(d,5);
	printstring(o,5);
 
 
}

	UART_Outchar(' ');
printstring(room1.pass,4);
	UART_Outchar(' ');
}
struct room change_status(struct room room1){

//if i want to command the room1
			if(room1.status==0){// room is locked then transform status to checked in and get pass word
				room1.status=1;
				room1.pass[0]=UART_InChar();
				room1.pass[1]=UART_InChar();
				room1.pass[2]=UART_InChar();
				room1.pass[3]=UART_InChar();
		}
		else if(room1.status==1){room1.status=UART_InInt();//send 0 to lock it or send 2 to cleaning service	
//room is checked in either i get it to be cleaned in or lock it depend on status i send		
		}		
		else if(room1.status==2)room1.status=1;//from sevice to check in 
		

return room1;

}
struct room checkpass(struct room room1){
char pass[4];
	pass[0]=UART_InChar();
	pass[1]=UART_InChar();
	pass[2]=UART_InChar();
	pass[3]=UART_InChar();
room1.doorstatus=1;
	if(room1.pass[0]!=pass[0])if(room1.status==1)room1.doorstatus=0;
	if(room1.pass[1]!=pass[1])if(room1.status==1)room1.doorstatus=0;
	if(room1.pass[2]!=pass[2])if(room1.status==1)room1.doorstatus=0;
	if(room1.pass[3]!=pass[3])if(room1.status==1)room1.doorstatus=0;
	
if(room1.status==0)room1.doorstatus=0;
	if(room1.status==2)room1.doorstatus=1;



return room1;
}



struct room room1={1,"0000",0,0};//initalize the room to no 1
	struct room room2={1,"0000",0,0};//be locked and have password
	struct room room3={1,"0000",0,0};//equals 0000
	struct room room4={1,"0000",0,0};//status
	struct room room5={1,"0000",0,0};
int main(void){ 
uartinit();	
room1.roomno=UART_InInt();

	room2.roomno=UART_InInt();

	room3.roomno=UART_InInt();

	room4.roomno=UART_InInt();

	room5.roomno=UART_InInt();
		while(1){
	int16_t room_no=UART_InInt();
		if(room_no==room1.roomno)room1=change_status(room1);
		else if(room_no==room2.roomno)room2=change_status(room2);	
		else if(room_no==room3.roomno)room3=change_status(room3);		
		else if(room_no==room4.roomno)room4=change_status(room4);	
		else if(room_no==room5.roomno)room5=change_status(room5);	
			
			
			if(room_no==0){

			printroom(room1);
			printroom(room2);	
			printroom(room3);	
			printroom(room4);	
			printroom(room5);	

		}
		
	 
if(room_no==255){	//used to send password through uart if it is checked in then it will open if password match
	
uint16_t roomno=UART_InInt();//room no
	
if(roomno==room1.roomno)room1=checkpass(room1);
if(roomno==room2.roomno)room2=checkpass(room2);
if(roomno==room3.roomno)room3=checkpass(room3);
if(roomno==room4.roomno)room3=checkpass(room4);
if(roomno==room5.roomno)room5=checkpass(room5);
}

}
}
	




	
	
