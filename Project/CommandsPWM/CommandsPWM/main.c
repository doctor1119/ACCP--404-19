
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define PWM_PIN PD5
#define TICK_DURATION 130   // Duration of a tick in ms
#define ARRAY_SIZE 1

uint16_t data[] = {251};
uint16_t dataIndex = 0;
uint16_t tickCount = 0;
uint8_t pwmState = 1;

void setup() {
	// Set PWM_PIN as output
	DDRD |= (1 << PWM_PIN);
	// Set Timer1 for PWM
	TCCR0A |= (1 << COM0B1) | (1 << WGM00);
	TCCR0B |= (1 << CS01);
	OCR0B = 0;
}

void delay(uint16_t ms) {
	for(uint16_t i=0; i<ms; i++) {
		_delay_ms(1);
	}
}

void loop() {
	if(dataIndex >= ARRAY_SIZE) {
		dataIndex = 0;
	}

	if(pwmState) {
		OCR0B = 255; // High duty cycle
		} else {
		OCR0B = 0; // Low duty cycle
	}

	if(tickCount >= data[dataIndex]) {
		tickCount = 0;
		dataIndex++;
		} else {
		tickCount++;
		pwmState = !pwmState;
	}

	delay(TICK_DURATION);
}

int main() {
	setup();
	sei();

	while(1) {
		loop();
	}
}

