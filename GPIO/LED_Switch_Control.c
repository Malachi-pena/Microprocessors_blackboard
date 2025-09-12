// Malachi Pena
// used Chatgbt as refences along with RealDigital,
// a little C primer.pdf, and Geeksforgeeks
// this task is to generate lights from the PWM using the switches.
//  add the PWM control to teh switch LED control in task 1

#include <stdint.h>
#include "sleep.h"


#define LED_DATA_ADDR	0x41210000 // LED are the lower 10 bits
#define SW_DATA         0x41220000  // switches are the lower 12 bits
#define Button_Data     0x41200000 // buttons are the lower 4 bits

#define PWM0_En      	0x43C00000 // LED 10 Blue Enable
#define PWM0_Period  	0x43C00004 // LED 10 Blue Period
#define PWM0_Width   	0x43C00008  // LED 10 Blue Width
#define PWM1_En      	0x43C00010 // LED 10 Green Enable
#define PWM1_Period  	0x43C00014 // LED 10 Green Period
#define PWM1_Width   	0x43C00018  // LED 10 Green Width
#define PWM2_En      	0x43C00020 // LED 10 Red Enable
#define PWM2_Period  	0x43C00024 // LED 10 Red Period
#define PWM2_Width   	0x43C00028  // LED 10 Red Width

#define PWM3_En      	0x43C00030 // LED 11 Blue Enable
#define PWM3_Period  	0x43C00034 // LED 11 Blue Period
#define PWM3_Width   	0x43C00038  // LED 11 Blue Width
#define PWM4_En      	0x43C00040 // LED 11 Green Enable
#define PWM4_Period  	0x43C00044 // LED 11 Green Period
#define PWM4_Width   	0x43C00048  // LED 11 Green Width
#define PWM5_En      	0x43C00050 // LED 11 Red Enable
#define PWM5_Period  	0x43C00054 // LED 11 Red Period
#define PWM5_Width   	0x43C00058  // LED 11 Red Width
// global defines to allow me to change values without hard coding values
#define Period 1024 // The width cannot be greater than the period.
#define Width  512  // Start in the middle
#define Width_Delta 50

#define SleepDelay 50000 // This is needed because you cannot update the PWM to fast.

int main (void) {

		uint32_t *datap, *dataSW, *dataBut;
		uint32_t valSW,valBut;
		uint32_t *LED_10_B_EN,*LED_10_G_EN,*LED_10_R_EN;
		uint32_t *LED_10_B_Period,*LED_10_G_Period,*LED_10_R_Period;
		uint32_t *LED_10_B_Width,*LED_10_G_Width,*LED_10_R_Width;
		uint32_t *LED_11_B_EN,*LED_11_G_EN,*LED_11_R_EN;
		uint32_t *LED_11_B_Period,*LED_11_G_Period,*LED_11_R_Period;
		uint32_t *LED_11_B_Width,*LED_11_G_Width,*LED_11_R_Width;

// set up the addresses for the physical parts
		datap = (uint32_t *)LED_DATA_ADDR;	//set the address of datap to led control
		dataSW = (uint32_t *)SW_DATA;	//set the address of dataSW to Switches
		dataBut = (uint32_t *)Button_Data;	//set the address of dataSW to Buttons
// setup the pointers to the RGB LEDS
		LED_10_B_EN = (uint32_t *)PWM0_En;
		LED_10_B_Period = (uint32_t *)PWM0_Period;
		LED_10_B_Width = (uint32_t *)PWM0_Width;
		LED_10_G_EN = (uint32_t *)PWM1_En;
		LED_10_G_Period = (uint32_t *)PWM1_Period;
		LED_10_G_Width = (uint32_t *)PWM1_Width;
		LED_10_R_EN = (uint32_t *)PWM2_En;
		LED_10_R_Period = (uint32_t *)PWM2_Period;
		LED_10_R_Width = (uint32_t *)PWM2_Width;

		LED_11_G_EN = (uint32_t *)PWM4_En;
		LED_11_G_Period = (uint32_t *)PWM4_Period;
		LED_11_G_Width = (uint32_t *)PWM4_Width;

/////////////////////////////////////////////
		/// Turn on the RGB LED with PWM
		*LED_10_B_EN = 1; // turn it on
		*LED_10_B_Period = Period ; // set it to 1024 to start
		*LED_10_B_Width = Width;  // half of period
		*LED_10_R_EN = 1; // turn it on
		*LED_10_R_Period = Period ; // set it to 1024 to start
		*LED_10_R_Width = Width;  // half of period
		*LED_10_G_EN = 1; // turn it on
		*LED_10_G_Period = Period ; // set it to 1024 to start
		*LED_10_G_Width = Width;  // half of period

		*LED_11_G_EN = 1; // turn it on
		*LED_11_G_Period = Period ; // set it to 1024 to start
		*LED_11_G_Width = Width;  // half of period


		while(1){

		valSW = *dataSW; // read the switches
		valBut = *dataBut; // read the Buttons
		*datap = 0b0000;	//write '0' to the LED's
		*datap = valSW;	//write the switch value to the register used for sw and button

		if ( valBut == 1){ // looking for switch 10
			if (valSW & 0x400){
			*LED_10_B_Width += Width_Delta ;
			if (*LED_10_B_Width > Period){ // saturate it at period
				*LED_10_B_Width =Period;
			}
			}
			else {
			*LED_10_B_Width -= Width_Delta ;
			if (*LED_10_B_Width > Period){ // this is an unsigned int it will go larger than period never less than 0
				*LED_10_B_Width =0;
			}
			}
		}

		if ( valBut == 2){ // looking for switch 10
			if (valSW & 0x400){
			*LED_10_G_Width += Width_Delta ;
			if (*LED_10_G_Width > Period){ // saturate it at period
				*LED_10_G_Width =Period;
			}
			}
			else {
			*LED_10_G_Width -= Width_Delta ;
			if (*LED_10_G_Width > Period){ // set break at 0
				*LED_10_G_Width =0;
			}
			}
		}
		if ( valBut == 4){ // looking for switch 10
			if (valSW & 0x400){
			*LED_10_R_Width += Width_Delta ;
			if (*LED_10_R_Width > Period){ // saturate it at period
				*LED_10_R_Width =Period;
			}
			}
			else {
			*LED_10_R_Width -= Width_Delta ;
			if (*LED_10_R_Width > Period){ // set break at 0
				*LED_10_R_Width =0;
			}
			}
		}

		usleep(SleepDelay); // updating PWM to fast to see

		} // from while loop
    return 1;
}
