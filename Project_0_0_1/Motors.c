/*
 * Motors.c
 *
 * Created: 23/03/2017 1:19:37 PM
 *  Author: s4357594
 */ 

 uint8_t steps;


 void init_step(void){
	steps = 0;
 }

 uint8_t current_step(void){
	uint8_t returnValue = steps;

	return returnValue;
 }

 void step(int8_t speed){
	// clockwise defined positive for speed
	// will work out how fast to turn motor based on speed (up for testing)
	// time on is the amount of time the stepper has to turn one rotation
	uint8_t time_on = 0;
	if (speed >= 0){
		if (speed < 30){
			 time_on = 15;
		} else if (speed < 60) {
			time_on = 10;
		} else if (speed < 90){
			time_on = 5;
		} else {
			time_on = 2;
		}
		step_clockwise(time_on);
	} else {
		if (speed > -30){
			time_on = 15;
		} else if (speed > -60) {
			time_on = 10;
		} else if (speed > -90){
			time_on = 5;
		} else {
			time_on = 2;
		}
		step_anticlockwise(time_on);
	}
 }

 void increment_step(void){
	++steps;
 }

 void decrement_step(void){
	--steps;
 }

 void step_clockwise(uint8_t time_on){
	// turns stepper motor clockwise


	 DDRD = 0xFF;
	 uint32_t current_time;
	 //uint8_t time_on = 100;

	 if(phase = 0){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTD = (1<<PORTD2)|(0<<PORTD3)|(0<<PORTD4)|(0<<PORTD5);
		 }
		 ++phase;
	 }
	 else if(phase = 1){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTD = (0<<PORTD2)|(1<<PORTD3)|(0<<PORTD4)|(0<<PORTD5);
		 }
		 ++phase;
	 }
	 else if(phase = 2){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTD = (0<<PORTD2)|(0<<PORTD3)|(1<<PORTD4)|(0<<PORTD5);
		 }
		 ++phase;
	 }
	 else if(phase = 3){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTD = (0<<PORTD2)|(0<<PORTD3)|(0<<PORTD4)|(1<<PORTD5);
		 }
		 phase = 0;
	 }
 }


 void step_anticlockwise(uint8_t time_on){
	// turns stepper motor clockwise
	DDRD = 0xFF;
	 uint32_t current_time;
	 //uint8_t time_on = 100;

	 if(phase = 0){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTD = (0<<PORTD2)|(0<<PORTD3)|(0<<PORTD4)|(1<<PORTD5);
		 }
		 ++phase;
	 }
	 else if(phase = 1){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTD = (0<<PORTD2)|(0<<PORTD3)|(1<<PORTD4)|(0<<PORTD5);
		 }
		 ++phase;
	 }
	 else if(phase = 2){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTD = (0<<PORTD2)|(1<<PORTD3)|(1<<PORTD4)|(0<<PORTD5);
		 }
		 ++phase;
	 }
	 else if(phase = 3){
		 current_time = get_tcnt0_ticks();
		 while((current_time + time_on) > get_tcnt0_ticks()){
			 PORTD = (1<<PORTD2)|(0<<PORTD3)|(0<<PORTD4)|(1<<PORTD5);
		 }
		 phase = 0;
	 }
 }