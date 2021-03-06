//NOTE: All flags are binary ( 0 or 1)

//FLAGS -- Mainloop
int flag_check_ldr =  0;
int flag_check_12v_brownout = 1;

// FLAGS -- Status
int flag_5v_brownout_detected = 0;
int flag_12v_brownout_detected = 0;
int flag_dynamixel1_disconnected = 0;
int flag_dynamixel2_disconnected = 0;

//THRESHOLDS
int threshold_12v = 700; //#CHANGE
int threshold_5v = 700; //#CHANGE
int threshold_ldr = 700;

//Pin Definitions
char pin_5v_brownout = A2;
char pin_12v_brownout = A3;
char pin_ldr_1 = A5;
char pin_ldr_2 = A4;
int pin_relay_5v = 6;
int pin_relay_12v = 7;

//Other Definitions
int input_5v;
int input_12v;
int input_ldr1;
int input_ldr2;
char serial_data_received;
char last_sent_data_packet;
// char next_expected_data_packet = ' ';
int debug_pin = 13;

void setup() {
	Serial.begin(57600);

	//Pin Initializations
	pinMode(pin_5v_brownout,INPUT);
	pinMode(pin_12v_brownout,INPUT);
	pinMode(pin_ldr_1,INPUT);
	pinMode(pin_ldr_2,INPUT);
    pinMode(debug_pin,OUTPUT);
    pinMode(pin_relay_5v,OUTPUT);
    pinMode(pin_relay_12v,OUTPUT);

    // Safety Initializations
    turn_off_dynamixel();
    turn_off_backup_battery();

    // Other Initializations
    digitalWrite(13,HIGH);
}

void loop() {
	// checks for serial communication
	if(Serial.available()>0){
		serial_data_received = Serial.read();
		service_serial_data_received(serial_data_received);
	}

	// checks for 12 Volt brown out
	if(flag_check_12v_brownout == 1){
		check_for_12v_brownout();		
	}

	// checks for 5 Volt brown out
	check_for_5v_brownout();

	// checks for LDR status
	if(flag_check_ldr == 1){
		check_both_ldr();
	}
}

// Functions to be run continuously
void service_serial_data_received(char serial_data_received){
	// This function is used for calling various functions as per the serial command. 
	if(serial_data_received == 'I'){ //* Initialize arduino
		initialize_to_default();
	}
    else if(serial_data_received == 'D'){ //* Dynamixel supply on
    	initialize_dynamixel();
    }
    else if(serial_data_received == 'R'){ //* Repeat last sent data packet
    	repeat_last_sent_data_packet();
    }
    else if(serial_data_received == 'L'){ //* Start checking LDR values
    	start_checking_ldr();
    }
    else if(serial_data_received == 'S'){ //* Ask for status
    	send_status();
    }
    else if(serial_data_received == 'h'){ //* Handshaking initialized
    	send_handshaking_value();
    }
    else{
    	send('x');
    }
	//#CHANGE
}

void check_for_12v_brownout(){
	// This function is used to check for stable 12 Volts
	input_12v = analogRead(pin_12v_brownout);
	// CHANGE LATER (to ensure actual brownout and not a minor fluctuation) 900 500
	// Serial.println(input_12v);

	if(input_12v<threshold_12v){
		turn_off_dynamixel();
		flag_12v_brownout_detected = 1;
	}
	else{
		// turn_on_dynamixel(); //COMMENT LATER
		flag_12v_brownout_detected = 0;
	}
}

void check_for_5v_brownout(){
	// This function is used to check for stable 5 Volts
	input_5v = analogRead(pin_5v_brownout);
	// 	// CHANGE LATER (to ensure actual brownout and not a minor fluctuation)

	if(input_5v<threshold_5v){
		turn_on_backup_battery();
		flag_5v_brownout_detected = 1;
	}
	else{
		turn_off_backup_battery();
		flag_5v_brownout_detected = 0;
	}
}

void check_both_ldr(){
	// This function is used to check if both Dynamixel lights are still on
	input_ldr1 = analogRead(pin_ldr_1);
	input_ldr2 = analogRead(pin_ldr_2);
	if(input_ldr1<threshold_ldr){
		turn_off_dynamixel();
		flag_dynamixel1_disconnected = 1;
	}
	else if(input_ldr2<threshold_ldr){
		turn_off_dynamixel();
		flag_dynamixel2_disconnected = 1;
	}
	else{
		flag_dynamixel1_disconnected = 0;
		flag_dynamixel2_disconnected = 0;
	}
}

// Functions called in cases for serial commands
void initialize_to_default(){
	// This function gives default initializations
	turn_off_dynamixel();
	turn_off_backup_battery();
	send('i'); //* initialization acknowledgement
	digitalWrite(debug_pin,LOW);
	//#CHANGE
}

void initialize_dynamixel(){
	check_for_12v_brownout();
	if(flag_12v_brownout_detected == 1){
		send('B'); //* 12V brown_out
	}
	else{
		flag_check_12v_brownout = 0;
		turn_on_dynamixel();
		send('d'); //* Acknowledgement that supply is on
	}
}

void start_checking_ldr(){
	flag_check_12v_brownout = 1;
	flag_check_ldr = 1;
	send('l'); //* Acknowledgement that LDR checking has started
}

void send(char character){
	last_sent_data_packet = character;
	Serial.write(last_sent_data_packet);
}


void repeat_last_sent_data_packet(){
	Serial.write(last_sent_data_packet);
}

void send_status(){
	if(flag_12v_brownout_detected == 1){
		send('B'); //* 12V brown_out
	}
	if(flag_5v_brownout_detected == 1){
		send('b'); //*  5V brown_out
	}
	if(flag_dynamixel1_disconnected == 1){
		send('1'); //* Dynamixel 1 & 2 Disconnected
	}
	if(flag_dynamixel2_disconnected == 1){
		send('2'); //* Dynamixel 2 Disconnected
	}
	if(flag_12v_brownout_detected+flag_5v_brownout_detected+flag_dynamixel1_disconnected+flag_dynamixel2_disconnected == 0){
		send('o'); //* All OK
	}
}

void send_handshaking_value(){
	send('0'); //* Indicates Arduino number == 1
}
// Functions for ease of access
void turn_on_backup_battery(){
	// This function will turn on backup battery
	digitalWrite(pin_relay_5v,HIGH); }

void turn_off_backup_battery(){
	// This function will turn off backup battery
	digitalWrite(pin_relay_5v,LOW); }

void turn_off_dynamixel(){
	// This function will turn off the dynamixel
	digitalWrite(pin_relay_12v,LOW);
	flag_check_ldr = 0;}

void turn_on_dynamixel(){
	// This function will turn on the dynamixel
	digitalWrite(pin_relay_12v,HIGH);}


// Debug Funciton
// void blink_debug_led(){
// 	digitalWrite(debug_pin,LOW);
// 	delay(500);
// 	digitalWrite(debug_pin,HIGH);
// }
