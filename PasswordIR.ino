//Libraries in use
#include <IRremote.h>
#include <IRremoteInt.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

bool oldButtonState;

int counters = 0;
//Digital Pins
const int RECV_PIN = 5;
const int greenLed = 10;
const int redLed = 8;
const int buzzer = 1;
const int armButton = 7;
const int speakerPin = 6;
const int sensor = 0;
//Alarm counter
int counter = 0;

//Password Variables
String password;
int passwordEEPROM;
//int checkPassword;
int chances = 3;
bool afterChances = true;
bool shouldBeep = true;
bool unlockSound = true;

bool locked = true;
String cPass;
int chance = 0;

//Remote Button Code
int btn1 = 0x9716be3f;
int btn2 = 0x3d9ae3f7;
int btn3 = 0x6182021b;
int btn4 = 0x8c22657b;
int btn5 = 0x488f3cbb;
int btn6 = 0x449e79f;
int btn7 = 0x32c6fdf7;
int btn8 = 0x1bc0157b;
int btn9 = 0x3ec3fc1b;
int btn0 = 0xc101e57b;
int btnReset = 0xd7e84b1b;
int btnRESET = 0xe318261b;

IRrecv irrecv(RECV_PIN);
decode_results results;

//LCD Pins
LiquidCrystal lcd(2, 3, 9, 10, 11, 12); // rs, en, d4,d5,d6,d7


void setup()
{
	Serial.begin(9600);
	readData();
	setupPassword(); //Called only once
	lcd.begin(16, 2);
	lcd.print("Alarm Disabled");
	lcd.setCursor(0, 1);
	lcd.print("Press OK to arm");

	pinMode(sensor, INPUT);
	pinMode(redLed, OUTPUT);//Diagnostic
	pinMode(greenLed, OUTPUT);//Diagnostic
	pinMode(armButton, INPUT_PULLUP);

	digitalWrite(redLed, HIGH); //Diagnostic
	digitalWrite(greenLed, LOW);	// Diagnostic

	irrecv.enableIRIn(); //Starts the IR Reciever
}

void setupPassword()
{
	Serial.print("Enter Password: ");
	while (Serial.available() == 0)
	{

	}
	passwordEEPROM = Serial.parseInt();
	saveData();


	print();
	
	//Serial.print("Your password is: " + password);
}

void saveData() {
	//saves data to EEPROM
	int address = 0;
	address = write(address, &passwordEEPROM, 2);
}

int write(int address, void *var, int num) {
	byte* ptr = (byte*)var;
	for (int i = 0; i < num; i++) {
		EEPROM.write(address + i, *ptr++);
	}
	return address + num;
}

void readData() {
	int address = 0;

	address = read(address, &passwordEEPROM, 2);
	print();
}

int read(int address, void *var, int num) {
	byte* ptr = (byte*)var;
	for (int i = 0; i < num; i++) {
		*ptr = EEPROM.read(address + i);
		ptr++;
	}
	return address + num;

}


void print() {
	Serial.print("Password EEPROM: ");
	Serial.print(passwordEEPROM);
	password = String(passwordEEPROM);

}

void loop()
{
	activationButton();
	sensorActivation();
	enterPassword();
}

bool sensorActivation()
{
	if (analogRead(sensor) > 700)
	{
		return true;
	}
}

bool activationButton()
{
	bool armButtonState = digitalRead(armButton);
	if (armButtonState != oldButtonState)
	{
		if (armButtonState)
		{
			alarmCounter();
			if (locked == false)
			{
				alarmActivated();
			}
		}
	}
	oldButtonState = armButtonState;
}

void enterPassword()
{
	if (irrecv.decode(&results))
	{
		lcd.blink();
		if (int(results.value) == btn1)
		{
			Serial.print("1");
			lcd.print("1");
			updatePass("1");
			if (locked == false)
			{
				reset("1");
			}
		}
		else if (int(results.value) == btn2)
		{
			Serial.print("2");
			lcd.print("2");
			updatePass("2");
			if (locked == false)
			{
				reset("2");
			}
		}
		else if (int(results.value) == btn3)
		{
			Serial.print("3");
			lcd.print("3");
			updatePass("3");
			if (locked == false)
			{
				reset("3");
			}
		}
		else if (int(results.value) == btn4)
		{
			Serial.print("4");
			lcd.print("4");
			updatePass("4");
			if (locked == false)
			{
				reset("4");
			}
		}
		else if (int(results.value) == btn5)
		{
			Serial.print("5");
			lcd.print("5");
			updatePass("5");
			if (locked == false)
			{
				reset("5");
			}
		}
		else if (int(results.value) == btn6)
		{
			Serial.print("6");
			lcd.print("6");
			updatePass("6");
		}
		else if (int(results.value) == btn7)
		{
			Serial.print("7");
			lcd.print("7");
			updatePass("7");
			if (locked == false)
			{
				reset("7");
			}
		}
		else if (int(results.value) == btn8)
		{
			Serial.print("8");
			lcd.print("8");
			updatePass("8");
			if (locked == false)
			{
				reset("8");
			}
		}
		else if (int(results.value) == btn9)
		{
			Serial.print("9");
			lcd.print("9");
			updatePass("9");
			if (locked == false)
			{
				reset("9");
			}
		}
		else if (int(results.value) == btn0)
		{
			Serial.print("0");
			lcd.print("0");
			updatePass("0");
			if (locked == false)
			{
				reset("0");
			}
		}
		else if (int(results.value) == btnRESET)
		{
			
			lcd.clear();
			lcd.print("Enter Password");
			lcd.setCursor(0, 1);
		}
		else if (int(results.value) == btnReset)
		{
			Serial.println("\nReset");
			lcd.clear();
			lcd.print("Enter Password");
			lcd.setCursor(0, 1);
			resetPass();
		}
		irrecv.resume();
	}
	if (chance > chances)
	{
		//digitalWrite(buzzer, HIGH);
	}
	if (locked)
	{
		digitalWrite(redLed, HIGH);
		digitalWrite(greenLed, LOW);
	}
	else
	{
		digitalWrite(redLed, LOW);
		digitalWrite(greenLed, HIGH);
	}
}

bool updatePass(String ch)
{
	if (!locked)
	{
		return false;
	}
	beep();
	if (updateable())
	{
		int i;
		
		cPass += ch;
		if (cPass.length() < password.length())
	//	if(cPass < 4)
		{

		}
		else
		{
			if (password == cPass)
			{
				alarmCounter();
				locked = false;
				digitalWrite(greenLed, HIGH);
				digitalWrite(redLed, LOW);
				chance = 0;
				Serial.println("\nUnlocked");
				lcd.clear();
				lcd.print("Alarm Activated");
				if (locked == false)
				{
					alarmActivated();
				}
			}
			else
			{
				//Serial.print("Incorrect");
				//Serial.print(cPass);
				//serial.print(password);
				//cPass = NULL;
				cPass = "";
				chance += 1;
				Serial.println("\nWrong Password");
				lcd.print("Denied");
			}
		}
	}
}

bool updateable()
{
	if (chance <= chances)
	{
		return true;
	}
	return afterChances;
}

void reset(String ch)
{
	Serial.print("reset method");
	cPass += ch;
	if (cPass.length() < password.length())
	{

	}
	else
	{
		if (password == cPass)
		{
			alarmDeactivated();
		}
		else
		{
			cPass = "";
			Serial.println("\nWrong Password");

		}
	}
}

void resetPass()
{
	String(cPass )= "";
	locked = true;
}

void beep()
{
	if (shouldBeep)
	{
	}
}

void alarmCounter()
{
	while (counter < 5)
	{
		int freq = 2000;
		int length = 100;
		tone(speakerPin, freq, length);
		delay(length + 1000);
		counter++;
		locked = false;
	}

}

void alarmActivated()
{
	Serial.print(analogRead(sensor));
	while (locked == false)
	{
		Serial.print(analogRead(sensor));
		if (analogRead(sensor) > 700)
		{
			int freq = 100;
			int length = 100;
			tone(speakerPin, freq, length);
			delay(length + 100);
			enterPassword();
		}
		else
		{
			enterPassword();
		}
	}
}

void alarmDeactivated()
{
	lcd.clear();
	lcd.print("Alarm Deactivated");
	Serial.println("Alarm Deactivated");
	resetPass();
	lcd.setCursor(0, 1);
	lcd.println("Press ok to arm");
	enterPassword();
}
