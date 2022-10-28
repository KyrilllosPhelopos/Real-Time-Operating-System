#include "bsp.h"

int main(void){

	//intialize UART0 with 16MHz clock & 9600 baud rate 
	uart0_init(16, 9600);
	
	/* transmit a string containing personal data */
        print("\r\n Personal Data");
		print("\r\n Name: kyrillos Phelopos Sawiris ");
        print("\r\n ID: 1804628");


        print("\r\n =================================================================");
         
	//printing clk freq
	print("\r\n clock frequency: 16 MHZ");
        
        print("\r\n =================================================================");

	//printing boud rate
	print("\r\n boud rate in decimal: ");
	print_idec(9600);
	
	print("\r\n boud rate in binary: ");
	print_ibin(9600);
	
	print("\r\n boud rate in hex: ");
	print_ihex(9600);

	
	
	while(1);
	
}