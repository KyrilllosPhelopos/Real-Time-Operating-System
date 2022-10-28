/*===========================================
 *				includes
 *===========================================*/
#include "bsp.h"

/*=============================================================================================
 * 						  FUNCTIONS Definitions
==========================================================================================*/




//
//******************************************************************************
// * \Name            : uart0_init
// * \Description     : Initialize the uart0 module and configure its baudrate
// * \Parameters (in) : - clk: value of the system clk
//						- baud_rate: wanted baudrate
// * \Parameters (out): None
// * \Return value:   : None
// *******************************************************************************/

void uart0_init(char clk, int baud_rate) {
//1. Enable the UART module using the RCGCUART register (see page 344).

	SYSCTL_RCGCUART_R |= (1 << 0);
	
//2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 340).
//To find out which GPIO port to enable, refer to Table 23-5 on page 1351.

	SYSCTL_RCGCGPIO_R |= (1 << 0);

//3. Set the GPIO AFSEL bits for the appropriate pins (see page 671). To determine which GPIOs to
//configure, see Table 23-4 on page 1344.
	GPIO_PORTA_AFSEL_R |= (1 << 1) | (1 << 0);
//set PA0 , PA1 To be TX , RX for the UART0
	GPIO_PORTA_PCTL_R |= (1 << 0) | (1 << 4);
//set PA0 , PA1 To be digital
GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);

//================================================================================
//==========Configuration of the UART module======================================
//================================================================================

//1. Disable the UART by clearing the UARTEN bit in the UARTCTL register.
    UART0_CTL_R &=~(1<<0);

//2. calculate the integer portion & fraction portion of the BRD to the UARTIBRD, UARTFBRD registers.

	float ibrd = (clk * 1e6) / (16 * baud_rate);
	float fbrd = (int) ((ibrd - (int) ibrd) * 64 + 0.5);
	UART0_IBRD_R = (int) ibrd;
	UART0_FBRD_R = fbrd;

//3. Write the desired serial parameters to the UARTLCRH register (in this case, a value of

	UART0_LCRH_R = (0x3 << 5);// 8-bit data
	
//4. configure the type of the system clk
	UART0_CC_R = 0x0;

//5. Enable the UART by setting the UARTEN bit in the UARTCTL registe
	UART0_CTL_R = (1 << 0) | (1 << 8) | (1 << 9); // 1-stop bit , no parity 
}


//
//******************************************************************************
// * \Name            : uart0_putchar
// * \Description     : send character using uart0
// * \Parameters (in) : - c: char we want to send
// * \Parameters (out): None
// * \Return value:   : None
// *******************************************************************************/
void uart0_putchar(char c) 
{
	//wait if the UART Transmit FIFO Full
	while ((UART0_FR_R & (1 << 5)) != 0);
	
	UART0_DR_R = c;
}



//
//******************************************************************************
// * \Name            : print
// * \Description     : send a string ofcharacters using uart0
// * \Parameters (in) : - str: string we want to send
// * \Parameters (out): None
// * \Return value:   : None
// *******************************************************************************/
void print(const char *str)
{
	while (*str) 
	{
		uart0_putchar(*(str++));
	}
}

//
//******************************************************************************
// * \Name            : print_ibin
// * \Description     : print the binary formate of a number 
// * \Parameters (in) : - n: number we want to print in binary
// * \Parameters (out): None
// * \Return value:   : size of the binary number 
// *******************************************************************************/
int print_ibin(unsigned n) {

	print("0b");
	for (int i = 31; i >= 0; i--) {
		int mask = (1 << i);
		if (n & mask)
			print("1");
		else
			print("0");
	}

	return 34;
}



//
//******************************************************************************
// * \Name            : print_ihex
// * \Description     : print the hex formate of a number 
// * \Parameters (in) : - n: number we want to print in hex
// * \Parameters (out): None
// * \Return value:   : size of the hex number 
// *******************************************************************************/
int print_ihex(unsigned n)
{
    // char array to store hexadecimal number
    char hexaDeciNum[8];
 
    // counter for hexadecimal number array
    int i = 0;
    while (n != 0) {
        // temporary variable to store remainder
        int temp = 0;
 
        // storing remainder in temp variable.
        temp = n % 16;
 
        // check if temp < 10
        if (temp < 10) {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else {
            hexaDeciNum[i] = temp + 55;
            i++;
        }
 
        n = n / 16;
    }
 
    // printing hexadecimal number array in reverse order
	print("0x");
    for (int j = i - 1; j >= 0; j--)
		uart0_putchar( hexaDeciNum[j]);
	return 10;
}


//
//******************************************************************************
// * \Name            : print_idec
// * \Description     : print the decimal formate of a number and it sign
// * \Parameters (in) : - n: number we want to print in signed decimal formate
// * \Parameters (out): None
// * \Return value:   : size of the number 
// *******************************************************************************/
int print_idec(int n) {
	int count = 0;
	//check if it was a negative number 
	if (n < 0) {
		uart0_putchar('-');
		count++;
		n *= -1;
	} 
	//check if it was 0
	else if (n == 0) 
	{
		uart0_putchar('0');
		return 1;
	}
	// make a buffer to store the ascci of each digit in the number 
	char buff[16];
	//calculate the length of the number and send it to get the ascii of each digit
	count += tostring(buff, n);
	//print the values of the ascii of the number 
	print(buff);
	return count;
}


//
//******************************************************************************
// * \Name            : tostring
// * \Description     : convert a decimal number into a string 
// * \Parameters (in) : - num: number we want to convert into string
// * \Parameters (out): str - buffer to store the ascii of the number
// * \Return value:   : length of the number 
// *******************************************************************************/
int tostring(char str[], int num) {
	int i, rem, len = 0, n;
	n = num;
	//to compute the length of the number 
	while (n != 0) {
		len++;
		//divide by 10 to skip one digit that we count 
		n /= 10;
	}
	//loop throuth the digits of the num and add the ascii of of '0' to it and store it in the array
	for (i = 0; i < len; i++) {
		//get the reminder to get the last digit 
		rem = num % 10;
		//skip the digit we git from the num
		num = num / 10;
		// add ascii of '0' to the digit we get
		str[len - (i + 1)] = rem + '0';
	}
	
	str[len] = '\0';
	return len;
}



