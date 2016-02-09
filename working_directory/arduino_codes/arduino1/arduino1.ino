//NOTE: All flags are binary ( 0 or 1)

//FLAGS -- Mainloop
<<<<<<< HEAD
int flag_ldr;
=======
int flag_check_ldr =  0;
>>>>>>> refs/remotes/origin/develop
int flag_check_5v_brownout = 1;
int flag_check_12v_brownout = 0;

// FLAGS -- Status
int flag_5v_brownout_detected = 0;
int flag_12v_brownout_detected = 0;
<<<<<<< HEAD
=======
int flag_dynamixel1_disconnected = 0;
int flag_dynamixel2_disconnected = 0;
>>>>>>> refs/remotes/origin/develop

//THRESHOLDS
int threshold_12v = 512; //#CHANGE
int threshold_5v = 512; //#CHANGE
<<<<<<< HEAD
=======
int threshold_ldr = 512;
>>>>>>> refs/remotes/origin/develop

//Pin Definitions
char pin_5v_brownout = A3;
char pin_12v_brownout = A4;
char pin_ldr_1 = A1;
char pin_ldr_2 = A2;
<<<<<<< HEAD
=======
int pin_relay_5v = 6;
int pin_relay_12v = 7;
>>>>>>> refs/remotes/origin/develop

//Other Definitions
int input_5v;
int input_12v;
<<<<<<< HEAD
char serial_command;
=======
int input_ldr1;
int input_ldr2;
char serial_command;
char data_packet;
int debug_pin = 13;
>>>>>>> refs/remotes/origin/develop

void setup() {
	Serial.begin(57600);

	//Pin Initializations
	pinMode(pin_5v_brownout,INPUT);
	pinMode(pin_12v_brownout,INPUT);
	pinMode(pin_ldr_1,INPUT);
	pinMode(pin_ldr_2,INPUT);
<<<<<<< HEAD

	// Safety Initializations
	turn_off_dynamixel();

=======
    pinMode(debug_pin,OUTPUT);
        
	// Safety Initializations
	turn_off_dynamixel();
	turn_off_backup_battery();

    // Other Initializations
    digitalWrite(13,HIGH);
>>>>>>> refs/remotes/origin/develop
}

void loop() {
	// checks for serial communication
	if(Serial.available()>0){
		serial_command = Serial.read();
		service_serial_command(serial_command);
	}

	// checks for 12 Volt brown out
	if(flag_check_12v_brownout == 1){
		check_for_12v_brownout();
	}

	// checks for 5 Volt brown out
	if(flag_check_5v_brownout == 1){
		check_for_12v_brownout();
	}
<<<<<<< HEAD
}

void service_serial_command(char serial_command){
	// This function is used for calling various functions as per the serial command. 
	
=======

	// checks for LDR status
	if(flag_check_ldr == 1){
		check_both_ldr();
	}
}

// Functions to be run continuously
void service_serial_command(char serial_command){
	// This function is used for calling various functions as per the serial command. 
	if(serial_command == 'I'){ //* Initialize arduino
		initialize_to_default();
	}
    if(serial_command == 'D'){ //* Dynamixel supply on
    	initialize_dynamixel();
    }
    if(serial_command == 'R'){ //* Repeat last sent data packet
    	repeat_last_sent_data_packet();
    }
>>>>>>> refs/remotes/origin/develop
	//#CHANGE
}

void check_for_12v_brownout(){
<<<<<<< HEAD
=======
	// This function is used to check for stable 12 Volts
>>>>>>> refs/remotes/origin/develop
	input_12v = analogRead(pin_12v_brownout);
	// CHANGE LATER (to ensure actual brownout and not a minor fluctuation)
	if(input_12v<threshold_12v){
		turn_off_dynamixel();
		flag_12v_brownout_detected = 1;
		flag_check_12v_brownout = 0;
	}
}

void check_for_5v_brownout(){
<<<<<<< HEAD
=======
	// This function is used to check for stable 5 Volts
>>>>>>> refs/remotes/origin/develop
	input_5v = analogRead(pin_5v_brownout);
	if(flag_5v_brownout_detected == 1){
		// CHANGE LATER (to ensure actual brownout and not a minor fluctuation)
		if(input_5v<threshold_5v){
			turn_on_backup_battery();
			flag_5v_brownout_detected = 1;
		}
	}
	else{
		if(input_5v>threshold_5v){
			turn_off_backup_battery();
			flag_5v_brownout_detected = 0;
		}
<<<<<<< HEAD
	}
	
}

void turn_off_dynamixel(){
	// This function will turn off the dynamixel

	//#CHANGE
}

void turn_on_backup_battery(){
	// This function will turn on backup battery

	//#CHANGE
}

void turn_off_backup_battery(){
	// This function will turn off backup battery

	//#CHANGE
=======
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
	data_packet = 'i'; //* initialization acknowledgement
	Serial.write(data_packet);
	digitalWrite(debug_pin,LOW);
	//#CHANGE
}

void initialize_dynamixel(){
	turn_on_dynamixel();
	data_packet = 'd'; //* Acknowledgement that supply is on
	Serial.write(data_packet); 
	while(Serial.available()!=1){}
	serial_command = Serial.read();
	if(serial_command == 'Y'){ //* Dynamixel acknowledgement
		flag_check_ldr = 1;
		flag_check_12v_brownout = 1;
	}
	else{
		data_packet = 'x'; //* Wrong Command 
		Serial.write(data_packet);
	}
}

void repeat_last_sent_data_packet(){
	Serial.write(data_packet);
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
void blink_debug_led(){
	digitalWrite(debug_pin,LOW);
	delay(500);
	digitalWrite(debug_pin,HIGH);
>>>>>>> refs/remotes/origin/develop
}
