// Malachi Pena
// GPIO.c
// used refence RealDigital,Dr.Mark Welker, and
// a little C primer.pdf.

#include <stdint.h>
#include <sleep.h>

#define LED_Data       *((uint32_t *)0x41210000) // LED are the lower 10 bits
#define Switch_Data    *((uint32_t *)0x41220000) // switches are the lower 12 bits
#define Button_Data    *((uint32_t *)0x41200000) // buttons are the lower 4 bits

// define the addresses for the GPIO inputs directly
#define Bank0_Input    *((uint32_t *)0xE000A060) // bank0 inputs
#define Bank1_Input    *((uint32_t *)0xE000A064) // bank1 inputs
#define Bank2_Input    *((uint32_t *)0xE000A068) // bank2 inputs
#define Bank3_Input    *((uiWorking nt32_t *)0xE000A06C) // bank3 inputs

// the GPIO BANK output data
#define Bank0_Output   *((uint32_t *)0xE000A040) // bank0 outputs
#define Bank1_Output   *((uint32_t *)0xE000A044) // bank1 outputs
#define Bank2_Output   *((uint32_t *)0xE000A048) // bank2 outputs
#define Bank3_Output   *((uint32_t *)0xE000A04C) // bank3 outputs

// the GPIO BANK Direction data
#define Bank0_Dir      *((uint32_t *)0xE000A204) // bank0 inputs
#define Bank1_Dir      *((uint32_t *)0xE000A244) // bank1 inputs
#define Bank2_Dir      *((uint32_t *)0xE000A284) // bank2 inputs
#define Bank3_Dir      *((uint32_t *)0xE000A2C4) // bank3 inputs

// the GPIO BANK Enable data
#define Bank0_Enable   *((uint32_t *)0xE000A208) // bank0 inputs
#define Bank1_Enable   *((uint32_t *)0xE000A248) // bank1 inputs
#define Bank2_Enable   *((uint32_t *)0xE000A288) // bank2 inputs
#define Bank3_Enable   *((uint32_t *)0xE000A2C8) // bank3 inputs


int main(void) {
    uint32_t InTemp0, InTemp2;
    uint32_t DirTemp0, DirTemp2;
    uint32_t EnTemp0, EnTemp2;

    while (1) {
        // Clear the LED's then set to the switch value ld0
        LED_Data = 0b0000;
        LED_Data = Switch_Data;

        // Just reading the inputs for now
        InTemp0 = Bank0_Input;
        InTemp2 = Bank2_Input;
        DirTemp0 = Bank0_Dir;
        DirTemp2 = Bank2_Dir;
        EnTemp0  = Bank0_Enable;
        EnTemp2  = Bank2_Enable;

        // Stop red signal on Bank2
        Bank2_Dir    &= 0x0000;
        Bank2_Enable &= 0x0000;
        Bank2_Output &= 0x0000;

        if (Switch_Data & 0b1) { // if true, board in N/S SENDS VALUES
            // Stop red signal on Bank2
            Bank2_Dir    = 0x0000;
            Bank2_Enable = 0x0000;
            Bank2_Output = 0x0000;

            sleep(1);

            // Send red signal on Bank2
            Bank2_Dir    = 0x8000;
            Bank2_Enable = 0x8000;
            Bank2_Output = 0x8000;

            // Turn on LED 12 red component as output
            Bank0_Dir    |= 0x20000;
            Bank0_Enable |= 0x20000;
            Bank0_Output |= 0x20000;

            sleep(2); // delay 2 seconds

            // Clear LED 12 components
            Bank0_Output &= ~(0x20000 | 0x40000 | 0x80000);

            // Set and enable green component
            Bank0_Dir    |= 0x40000;
            Bank0_Enable |= 0x40000;
            Bank0_Output |= 0x40000;

            sleep(2);

            // Clear LED then set both red and green components (yellow)
            Bank0_Output &= ~(0x20000 | 0x40000 | 0x80000);
            Bank0_Dir    |= 0x60000;
            Bank0_Enable |= 0x60000;
            Bank0_Output |= 0x60000;

            sleep(1);

            // Clear LED then turn back on red component
            Bank0_Output &= ~(0x20000 | 0x40000 | 0x80000);
            Bank0_Dir    |= 0x20000;
            Bank0_Enable |= 0x20000;
            Bank0_Output |= 0x20000;

            sleep(2);

            // Stop red signal on Bank2
            Bank2_Dir    = 0x8000;
            Bank2_Enable = 0x8000;
            Bank2_Output = 0x8000;

            // Send green signal on Bank2
            Bank2_Dir    = 0x10000;
            Bank2_Enable = 0x10000;
            Bank2_Output = 0x00000;

            sleep(2);

            // Stop green signal on Bank2
            Bank2_Dir    = 0x10000;
            Bank2_Enable = 0x10000;
            Bank2_Output = 0x10000;

            // Send yellow signal on Bank2
            Bank2_Dir    = 0x20000;
            Bank2_Enable = 0x20000;
            Bank2_Output = 0x00000;

            sleep(1);

            // Stop yellow signal on Bank2
            Bank2_Dir    = 0x20000;
            Bank2_Enable = 0x20000;
            Bank2_Output = 0x20000;

            // Finally, send red signal again on Bank2
            Bank2_Dir    = 0x8000;
            Bank2_Enable = 0x8000;
            Bank2_Output = 0x0000;
        } else {
            int red = 0;
            int green = 0;
            int yellow = 0;

            // Set up Bank2 for general output signals
            Bank2_Dir    = 0x78000;
            Bank2_Enable = 0x78000;
            Bank2_Output = 0x78000;

            // Turn on LED 12 red component as output
            Bank0_Dir    |= 0x20000;
            Bank0_Enable |= 0x20000;
            Bank0_Output |= 0x20000;

            InTemp2 = Bank2_Input;

            if (InTemp2 == 0x00f7ffff && red == 0) {
                Bank0_Output &= ~(0x20000 | 0x40000 | 0x80000);
                Bank0_Dir    |= 0x20000; // Set LED 12 red component as output
                Bank0_Enable |= 0x20000; // Enable LED 12 red component
                Bank0_Output |= 0x20000;
                red = 1;
                green = 0;
                yellow = 0;
            }
            if (InTemp2 == 0x00efffff && green == 0) {
                Bank0_Output &= ~(0x20000 | 0x40000 | 0x80000);
                Bank0_Dir    |= 0x40000; // Set green component as output
                Bank0_Enable |= 0x40000; // Enable green component
                Bank0_Output |= 0x40000;
                green = 1;
                red = 0;
                yellow = 0;
            }
            if (InTemp2 == 0x00dfffff && yellow == 0) {
                Bank0_Output &= ~(0x20000 | 0x40000 | 0x80000);
                Bank0_Dir    |= 0x60000; // Set both red and green components as output
                Bank0_Enable |= 0x60000; // Enable both red and green components
                Bank0_Output |= 0x60000;
                yellow = 1;
                red = 0;
                green = 0;
            }
        }

        // These are the hex numbers that relate to the specific pins
        // PMODC pin 1 0x8000
        // PMODC pin 2 0x10000
        // PMODC pin 3 0x20000
        // PMODC pin 4 0x40000
        // PMODC pin 7 0x80000
        // PMODC pin 8 0x100000
        // PMODC pin 9 0x200000
        // PMODC pin 10 0x400000
        // bank 2 is where all the PMODC pins live.
        // If we set bank2 direction and enable to 0x8000 we should be able
        // to write 0x8000 to output to drive it to a one.

        // JC1 should be a 1 right now

        // Bank2_Output = 0x0000;
        // turn on LED 12 to be white. This lives at bank 0 bits 16, 17, 18
        // B 1, R 2, G 4,
        //
        // Bank0_Dir = 0x20000; // This will turn on the red LED
        // Bank0_Enable = 0x20000;
        // Bank0_Output = 0x20000;
    }
    return 1;
}

