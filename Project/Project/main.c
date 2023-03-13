#define F_CPU 16000000UL
#define Baud  9600
#define MyBaud F_CPU/16/Baud-1

#include <xc.h>
#include <avr/interrupt.h> 
#include <avr/io.h>


int main(void)
{
	temp_sensor_init();
	uart_init(MyBaud);
    while(1)
	{
		
	}
    
}





