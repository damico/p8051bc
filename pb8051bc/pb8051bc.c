/*
 * pb8051bc.c
 *
 *		Programmable 8051 Binary Calculator
 *  	Created on: Apr 26, 2012
 *      Author: Jose Damico <jd.comment@gmail.com>
 */
#include <mcs51/at89c51ed2.h>

unsigned char byteCount = 0;
unsigned char byteLock = 0;
unsigned char tempVal = 0;
unsigned char db = 0;
unsigned char byte_A[8];
unsigned char byte_B[8];
unsigned char byte_C[8];
unsigned char currentKey = -1;
unsigned char bitVal = 0x00;


void resetCount(){

	P0_0 = 0x00;
	P0_1 = 0x00;
	P0_2 = 0x00;
	P0_3 = 0x00;
	P0_4 = 0x00;
	P0_5 = 0x00;
	P0_6 = 0x00;
	P0_7 = 0x00;

	byteCount = 0;
}

void setBitLedValue(){

	bitVal = tempVal == 0 ? 0x00 : 0xff;

	switch (byteCount) {

	case 1:
		P0_2 = bitVal; //2
		break;
	case 2:
		P0_3 = bitVal; //3
		break;
	case 3:
		P0_0 = bitVal; //0
		break;
	case 4:
		P0_1 = bitVal; //1
		break;
	case 5:
		P0_6 = bitVal; //8
		break;
	case 6:
		P0_5 = bitVal; //6
		break;
	case 7:
		P0_4 = bitVal; //5
		break;
	case 8:
		P0_7 = bitVal; //4
		break;
	default:
		break;
	}
}

void setByteArray(){
	switch (byteLock) {
	case 0:
		byte_A[byteCount] = tempVal;
		break;
	case 1:
		byte_B[byteCount] = tempVal;
		break;
	case 2:
		byte_C[byteCount] = tempVal;
		break;
	default:
		break;
	}
}

void inputByte(){
	P1_1=0xff; // busy status on

	if(byteCount <= 8 && db == 0){

		byteCount++;
		tempVal = currentKey == 0 ? 0 : 1;
		setBitLedValue();
		setByteArray();
		db = 1;

	}
}

void main(){

	//Control LEDs

	P1_5=0x00;
	P1_4=0x00;
	P1_3=0x00;
	P1_2=0x00;
	P1_1=0x00;
	P1_0=0xff; // Ready Status

	//Byte LEDs


	P0_0 = 0x00;
	P0_1 = 0x00;
	P0_2 = 0x00;
	P0_3 = 0x00;
	P0_4 = 0x00;
	P0_5 = 0x00;
	P0_6 = 0x00;
	P0_7 = 0x00;




	while(1){

		if(P3_3 == 0x00 && byteLock <= 2){
			currentKey = 1;
			inputByte();

		}else if(P3_4 == 0x00 && byteLock <= 2){
			currentKey = 0;
			inputByte();

		}else if(P3_6 == 0x00 && byteCount >= 8){
			byteCount = 10; //Enter
			P1_1=0xff; // busy status on
			if(P1_2==0x00){
				P1_2=0xff; // byte A ready
				byteLock = 1;
				resetCount();

			}else if(byteLock==1){
				P1_3=0xff; // byte B ready
				byteLock = 2;
				resetCount();

			}else if(byteLock==2){
				P1_4=0xff; // byte C ready
				resetCount();
			}

		}else if(P3_5 == 0x00 && byteCount == 0){

			inputByte();

		}else{
			P1_1=0x00; // busy status off
			db = 0;
		}



	}
}

