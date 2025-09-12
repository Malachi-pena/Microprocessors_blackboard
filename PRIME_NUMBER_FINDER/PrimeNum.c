
// Malachi Pena
// data: 3/4/2025
// Prime number finder
// The following code was made through the usage of refence material such as: Realdigital,
// Mr. Welker's UART configuration example code in UART.pptx.
#include <stdint.h>
#include <stdio.h>



#define UART1_CR       *((uint32_t *) 0xE0001000) // UART1 control register
#define UART1_MR       *((uint32_t *) 0xE0001004) // UART1 Mode register
#define UART1_BAUDGEN  *((uint32_t *) 0xE0001018)
#define UART1_BAUDDIV  *((uint32_t *) 0xE0001034)
#define UART1_SR       *((uint32_t *) 0xE000102C) // UART1 status reg
#define UART1_DATA     *((uint32_t *) 0xE0001030) // UART1 TX/RX FIFO DATA

// Values for 115200 baud on some platforms
#define BaudGen115200  0x7C
#define BaudDiv115200  6

// Checks if transmit FIFO is full
int uart1_tx_full()
{
    return (UART1_SR & 16) != 0;
}

// Sends a single character over UART1
void uart1_put_char(char c)
{
    while(uart1_tx_full()) { /* wait */ }
    UART1_DATA = c;
}

// Sends a null-terminated string over UART1
void uart1_putstr(const char *str)
{
    while(*str)
    {
        uart1_put_char(*str);
        str++;
    }
}

// Checks if receive FIFO is empty
int UART1_RXEmpty()
{
    return (UART1_SR & 2) != 0;
}

// Blocks until a character is available, then returns it
char uart1_get_char()
{
    while(UART1_RXEmpty()) { /* wait */ }
    return (char)(UART1_DATA & 0xFF);
}

// Reads a decimal integer from UART1 until Enter is pressed
int GetInteger()
{
    char buffer[32];
    int i = 0;
    int number = 0;

    uart1_putstr("Enter an integer: ");

    while(1)
    {
        char c = uart1_get_char();

        if(c == '\r' || c == '\n')
        {
            uart1_put_char('\r');
            uart1_put_char('\n');
            buffer[i] = '\0';
            break;
        }
        if(c >= '0' && c <= '9' && i < 31)
        {
            buffer[i++] = c;
            uart1_put_char(c);
        }
    }

    for(int j = 0; j < i; j++)
    {
        number = number * 10 + (buffer[j] - '0');
    }

    return number;
}

void ResetUART1()
{
    UART1_CR = 3;
    while(UART1_CR & 0x2) { /* wait */ }
    while(UART1_CR & 0x1) { /* wait */ }
}

void Configure_UART1()
{
    UART1_MR = 0x20;
    uint32_t val = UART1_CR;
    val |= 0x14;
    UART1_CR = val;
}

void SetBaudrate1()
{
    UART1_BAUDGEN = BaudGen115200;
    UART1_BAUDDIV = BaudDiv115200;
}

void initUart1()
{
    ResetUART1();
    Configure_UART1();
    SetBaudrate1();
}

int isPrime(int num)
{
    if(num < 2) return 0;
    for(int i = 2; i <= num / 2; i++)
    {
        if((num % i) == 0)
            return 0;
    }
    return 1;
}

int main()
{
    initUart1();

    while(1)
    {
        int NumberIn;
        do {
            NumberIn = GetInteger();
            if(NumberIn < 2)
            {
                uart1_putstr("Invalid. Please enter a number >= 2.\r\n");
            }
        } while(NumberIn =< 2);

        while(1)
        {
            char tmp[32];
            uart1_putstr("The test is made and ");
            sprintf(tmp, "%d", NumberIn);
            uart1_putstr(tmp);

            if(isPrime(NumberIn))
            {
                
                uart1_putstr("The prime number found is ");
                sprintf(tmp, "%d\r\n", NumberIn);
                uart1_putstr(tmp);
                break;
            }
            else
            {
                uart1_putstr(" is found to be not prime.\r\n");
                sprintf(tmp, "%d", NumberIn);
                uart1_putstr(tmp);
                uart1_putstr(" is decremented to ");
                sprintf(tmp, "%d\r\n", NumberIn - 1);
                uart1_putstr(tmp);
                NumberIn--;
            }
        }
    }
}
