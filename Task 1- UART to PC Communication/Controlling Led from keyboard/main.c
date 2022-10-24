typedef unsigned int uint32_t;



#define RCGCUART			*(volatile uint32_t *)(0x400FE000 + 0x618)
#define RCGCGPIO			*(volatile uint32_t *)(0x400FE000 + 0x608)
#define GPIOPCTL			*(volatile uint32_t *)(0x40004000 + 0x52C)
#define GPIOAFSEL			*(volatile uint32_t *)(0x40004000 + 0x420)
#define GPIODEN				*(volatile uint32_t *)(0x40004000 + 0x51C)

#define GPIOCR				*(volatile uint32_t *)(0x40004000 + 0x524)
#define GPIODR2R			*(volatile uint32_t *)(0x40004000 + 0x500)

#define UART0_BASE 			0x4000C000
#define UARTCTL				*(volatile uint32_t *)(UART0_BASE + 0x030)
#define UARTIBRD			*(volatile uint32_t *)(UART0_BASE + 0x024)
#define UARTFBRD			*(volatile uint32_t *)(UART0_BASE + 0x028)
#define UARTLCRH			*(volatile uint32_t *)(UART0_BASE + 0x02C)
#define UARTFR			        *(volatile uint32_t *)(UART0_BASE + 0x18)
#define UARTDR			        *(volatile uint32_t *)(UART0_BASE + 0x0)
 
#define GPIOF_DATA                      *(volatile uint32_t *)(0x40025000 + 0x3FC) 
#define GPIOF_DEN                       *(volatile uint32_t *)(0x40025000 + 0x51C) 
#define GPIOF_DIR                       *(volatile uint32_t *)(0x40025000 + 0x400) 



#define RCGC2                          (*((volatile unsigned long *)0x400FE108))



char read_char()
{
	char c;
	while( (UARTFR & (1<<4)) !=0); //The receiver is not empty.
	c  = UARTDR ; 
	return c;
	
}

void print_char(char c)
{
	
	while( (UARTFR & (1<<5) ) !=0); //UART Transmit FIFO Full
	UARTDR = c;
	
}
void print_string(char* str)
{
	while(*str)
	{
		print_char(*(str++));
	}
}



int main()
{

    
    
//1. Enable the UART module using the RCGCUART register (see page 344).
RCGCUART |=(1<<0);
//2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 340).
//To find out which GPIO port to enable, refer to Table 23-5 on page 1351.
RCGCGPIO |=(1<<0);
//3. Set the GPIO AFSEL bits for the appropriate pins (see page 671). To determine which GPIOs to
//configure, see Table 23-4 on page 1344.
GPIOAFSEL = (1<<0) | (1<<1);
GPIOPCTL  = (1<<0)|(1<<4);
GPIODEN   = (1<<0)|(1<<4);

GPIOCR  |= (1<<0);


//================================================================================
//1. Disable the UART by clearing the UARTEN bit in the UARTCTL register.
UARTCTL &=~(1<<0);
//2. Write the integer portion of the BRD to the UARTIBRD register.

//int IBRD = (clk)/(8 * baudrate);
UARTIBRD = 104;

//3. Write the fractional portion of the BRD to the UARTFBRD register.

UARTFBRD = 11;
//4. Write the desired serial parameters to the UARTLCRH register (in this case, a value of
//0x0000.0060).

UARTLCRH |= (0x3<<5); // 8-bit data
//UARTLCRH |= (1<<4); // FIFO enable



//5. Enable the UART by setting the UARTEN bit in the UARTCTL registe
UARTCTL =(1<<0) |(1<<8) | (1<<9);


// configure led 


RCGC2 |= 0x00000020; /*enable clock to port F*/
GPIOF_DIR = (1<<1) |(1<<2) |(1<<3);
GPIOF_DEN = (1<<1) |(1<<2) |(1<<3);
GPIOF_DATA &=~( (1<<1) |(1<<2) |(1<<3) );
char c;
while(1)
{
        print_char('a');
	print_string("Enter R or G or B \n");
	c  = read_char();
	print_char(c);
	print_char('\n');
	
	switch(c)
	{
		case 'r':
		GPIOF_DATA = (1<<1);
		break;
		case 'g':
		GPIOF_DATA = (1<<2);
		break;
		case 'b':
		GPIOF_DATA = (1<<3);
		break;
	}
}
}