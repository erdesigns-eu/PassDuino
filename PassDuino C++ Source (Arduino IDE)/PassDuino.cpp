/*

	PassDuino
	Erdesigns.eu - Ernst Reidinga

	Version 1.0 (August 2020)

	Description:
	- Type passwords as a simulated keyboard

*/

#include "Arduino.h"
#include "PassDuino.h"
#include "Variables.h"

// Reading or writing to build-in EEPROM 
#include <EEPROM.h>
// Needed for emulating a keyboard.
#include <Keyboard.h>


PassDuino::PassDuino(const int &eepromSize, const int &eepromOffset) {
	this->eepromSize   = eepromSize;
	this->eepromOffset = eepromOffset;
	this->init();
}

String PassDuino::get_password(const int &index) {
	if (index < 0 || index > this->max_passwords()) {
		return "";
	}
	int offset;
	offset = this->eepromOffset + (STRCONFOFFSET * 2) + (index * (this->stringLength +1));
	int newStrLen = EEPROM.read(offset);
	char data[newStrLen + 1];
	for (int i = 0; i < newStrLen; i++) {
		data[i] = EEPROM.read(offset + 1 + i);
	}
	data[newStrLen] = '\0';
	return String(data);
}

int PassDuino::get_configuration(const int &config) {
	if (config > STRCONFOFFSET) {
		return -1;
	}
	int output;
	int offset;
	offset = this->eepromOffset + config * 2;
	byte lowByte = EEPROM.read(offset);
	byte highByte = EEPROM.read(offset + 1);
	output = ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
	return output;
}

int PassDuino::max_passwords() {
	return floor((this->eepromSize - (this->eepromOffset + (STRCONFOFFSET * 2))) / this->stringLength);
}

void PassDuino::init() {
	this->stringLength 			 = this->get_configuration(0);
	this->default_password_index = this->get_configuration(1);
	this->return_after_typing	 = this->get_configuration(2) == 1;
	this->typeDelay				 = this->get_configuration(3);
}

void PassDuino::type_default_password() {
  this->type_string(this->get_password(this->default_password_index));
}

void PassDuino::type_string(const String &str) {
	Keyboard.print(str);
	if (this->return_after_typing) {
		Keyboard.press(KEY_RETURN);
		delay(KEYSMALLDELAY);
		Keyboard.release(KEY_RETURN);
	}
}

void PassDuino::type_password(const int &index) {
	if (this->typeDelay > 0) {
		delay(this->typeDelay * 1000);
	}
	this->type_string(this->get_password(index));
}
