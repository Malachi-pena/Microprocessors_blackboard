// Malachi Pena
// Lab2.c
// used RealDigital, a little C primer.pdf, and Geeksforgeeks as
// reference material 

#include <stdint.h>
#include <unistd.h> // For usleep()

// 7-segment display registers (memory-mapped I/O)
#define SEG_CTL  (*((volatile uint32_t *) 0x43c10000)) // pointer is seg_CTL with address 0x43c10000 
#define SEG_DATA (*((volatile uint32_t *) 0x43C10004))// same thing for all of these
#define SEG_DP   (*((volatile uint32_t *) 0x43C10014))

// Button Registers (Assuming first 3 buttons are BT0, BT1, BT2)
#define BTN_DATA (*((volatile uint32_t *) 0x41200000))

// Stopwatch State Definitions
#define STOPPED 0
#define RUNNING 1

volatile uint8_t stopwatch_state = STOPPED;  // Stopwatch starts in STOPPED mode
volatile uint16_t counter_value = 0;         // BCD counter starts at 0

/**
 * Displays a 4-digit BCD number on the 7-segment display.
 */
void display_BCD(uint16_t n)
{
    uint32_t temp = 0;
    int i;

    // Enable display in standard mode (BCD)
    SEG_CTL = 1;

    for (i = 0; i < 4; i++)
    {
        temp |= (n % 10) << (i * 8);  // Extract each decimal digit in bits not bytes, 0-7,8-15,16-23        
        n /= 10;                      // Shift to next digit 
    }

    // Disable all decimal points
    temp |= 0x80808080;

    // Write to segment data register
    SEG_DATA = temp;
}

/**
 * Handles button presses and updates stopwatch state.
 */
void handle_buttons()
{
    uint8_t current_state = BTN_DATA & 0x7; // Read first 3 buttons

    if (current_state & 0x1) // BT0 (Stop)
    {
        stopwatch_state = STOPPED;
    }
    else if (current_state & 0x2) // BT1 (Start)
    {
        stopwatch_state = RUNNING;
    }
    else if ((current_state & 0x4) && stopwatch_state == STOPPED) // BT2 (Reset)
    {
        counter_value = 0;
        display_BCD(counter_value); // Force update immediately
        usleep(200000); // Small delay to prevent accidental multiple resets
    }
}

/**
 * Stopwatch mode - Counts in BCD when running.
 */
void stopwatch_mode()
{
    handle_buttons();

    if (stopwatch_state == RUNNING)
    {
        display_BCD(counter_value);
        counter_value++;
    }

    usleep(100000); // Delay of 100ms
}

/**
 * Main function
 */
int main(void)
{
    while (1)
    {
        stopwatch_mode();
    }

    return 0;
}
