/*

	PassDuino Configurator
	Erdesigns.eu - Ernst Reidinga

	Version 1.0 (August 2020)

	Description:
	- Read String and Int from EEPROM
	- Write String and Int to EEPROM

*/

#include "Arduino.h"
#include "avr/wdt.h"
#include "Configurator.h"
#include "Variables.h"

// Reading or writing to build-in EEPROM 
#include <EEPROM.h>

Configurator::Configurator(const int &eepromSize, const int &eepromOffset, const String &password) {
	this->eepromSize   = eepromSize;
	this->eepromOffset = eepromOffset;
	this->password 	   = password;

	// Test if this is the first time this program is run on the device
	// and if it is, we want to set some default values.
	if (this->get_configuration_int(4) != FIRSTRUNKEY) {

		// String (Password) length
		this->set_configuration_int(0, 20);
		// Default password index
		this->set_configuration_int(1, 0);
		// Press return after typing
		this->set_configuration_int(2, 0);
		// Type delay - time before starting to type
		this->set_configuration_int(3, 0);
		// Set the first run key
		this->set_configuration_int(4, FIRSTRUNKEY);
		// Set "default" password at index 0
		this->set_configuration_string(0 + STRCONFOFFSET, DEFAULTPASS);

	}

	this->stringLength = this->get_configuration_int(0);
}

int Configurator::write_string_eeprom(const int &offset, const String &str) {
	byte len = str.length();
	EEPROM.update(this->eepromOffset + offset, len);
	for (int i = 0; i < len; i++) {
		EEPROM.update(this->eepromOffset + offset + 1 + i, str[i]);
	}
	return offset + 1 + len;
}

int Configurator::write_integer_eeprom(const int &offset, const int &val) {
	byte lowByte = ((val >> 0) & 0xFF);
	byte highByte = ((val >> 8) & 0xFF);
	EEPROM.update(this->eepromOffset + offset, lowByte);
	EEPROM.update(this->eepromOffset + offset + 1, highByte);
	return offset + 2;
}

String Configurator::get_configuration_string(const int &config) {
	if (config - STRCONFOFFSET < 0) {
		return "";
	}
	int offset;
	offset = this->eepromOffset + (STRCONFOFFSET * 2) + ((config - STRCONFOFFSET) * (this->stringLength +1));
	int newStrLen = EEPROM.read(offset);
	char data[newStrLen + 1];
	for (int i = 0; i < newStrLen; i++) {
		data[i] = EEPROM.read(offset + 1 + i);
	}
	data[newStrLen] = '\0';
	return String(data);
}

bool Configurator::set_configuration_string(const int &config, const String &str) {
	if (config - STRCONFOFFSET < 0) {
		return false;
	}
	int offset;
	offset = this->eepromOffset + (STRCONFOFFSET * 2) + ((config - STRCONFOFFSET) * (this->stringLength +1));
	return this->write_string_eeprom(offset, str) > offset -1;
}

int Configurator::get_configuration_int(const int &config){
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

bool Configurator::set_configuration_int(const int &config, const int &val){
	if (config > STRCONFOFFSET) {
		return false;
	}
	int offset;
	offset = this->eepromOffset + config * 2;
	return this->write_integer_eeprom(offset, val) > offset -1;
}

void Configurator::write_configurator_output_string(const String &str) {
	String output = str;
	output.concat(CNLN);
	Serial.print(output);
}

void Configurator::write_configurator_output_hex(const int &val) {
	Serial.print(val, HEX);
	Serial.print(CNLN);
}

void Configurator::write_configurator_output_dec(const int &val) {
	Serial.print(val, DEC);
	Serial.print(CNLN);
}

int Configurator::max_passwords() {
	return floor((this->eepromSize - (this->eepromOffset + (STRCONFOFFSET * 2))) / this->stringLength);
}

bool Configurator::start(const String &password) {
	//EEPROM.begin(this->eepromSize);
	this->stringLength = this->get_configuration_int(0);
	if (password == this->password) {
		this->active = true;
		this->write_configurator_output_string(STRSTART);
	} else {
		this->active = false;
		this->error();
	}
	return this->active;
}

void Configurator::stop() {
	//EEPROM.end();
	this->active = false;
	this->write_configurator_output_string(STRSTOP);
}

void Configurator::clear() {
	for (int i = 0; i < this->max_passwords(); ++i) {
      this->write_configuration(i + STRCONFOFFSET, "", -1);
    }
    this->write_configurator_output_string(STROK);
}

bool Configurator::is_active() {
  return this->active;
}

void Configurator::ok() {
	this->write_configurator_output_string(STROK);
}

void Configurator::error() {
	this->write_configurator_output_string(STRER);
}

void Configurator::read_configuration(const int &config) {
	if (config == -1) {
		this->write_configurator_output_dec(this->max_passwords());
	} else
	if (config < STRCONFOFFSET && config > -1) {
		this->write_configurator_output_dec(this->get_configuration_int(config));
	} else
	if (config >= STRCONFOFFSET && config < this->max_passwords()) {
		this->write_configurator_output_string(this->get_configuration_string(config));
	} else 
	if (config >= this->max_passwords() || config < -1) {
		this->error();
	}
}

void Configurator::write_configuration(const int &config, const String &str, const int &val) {
	if (config < STRCONFOFFSET && config > -1) {
		if (this->set_configuration_int(config, val)) {
			this->ok();
		} else {
			this->error();
		}
	} else
	if (config >= STRCONFOFFSET && config < this->max_passwords()) {
		if (this->set_configuration_string(config, str)) {
			this->ok();
		} else {
			this->error();
		}
	} else 
	if (config >= this->max_passwords() || config < -1) {
		this->error();
	}
}

int Configurator::string_part_count(const String &str) {
	int c = 0;
	for (int i = 0; str[i] != '\0'; i++) {
	    if(str[i] == DIVIDER)
	        ++c;
	}
	return c;
}

String Configurator::string_part(const String &str, const int &index) {
	int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = str.length() - 1;
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (str.charAt(i) == DIVIDER || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? str.substring(strIndex[0], strIndex[1]) : "";
}
