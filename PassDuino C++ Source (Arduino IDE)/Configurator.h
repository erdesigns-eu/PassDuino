/*

	PassDuino Configurator
	Erdesigns.eu - Ernst Reidinga

	Version 1.0 (August 2020)

	Description:
	- Read String and Int from EEPROM
	- Write String and Int to EEPROM

*/

#ifndef Configurator_h
#define Configurator_h

#include <Arduino.h>
#include <avr/wdt.h>
#include "Variables.h"

class Configurator {
	private:
		int eepromSize;
		int eepromOffset;
		int stringLength;
		bool active;
		String password;

		int write_string_eeprom(const int &offset, const String &str);
		int write_integer_eeprom(const int &offset, const int &val);
		
		String get_configuration_string(const int &config);
		bool set_configuration_string(const int &config, const String &str);
		
		int get_configuration_int(const int &config);
		bool set_configuration_int(const int &config, const int &val);
		
		void write_configurator_output_string(const String &str);
		void write_configurator_output_hex(const int &val);
		void write_configurator_output_dec(const int &val);
	public:
		Configurator(const int &eepromSize, const int &eepromOffset, const String &password);

		int max_passwords();

		bool start(const String &password);
		void stop();
		void clear();
		
		bool is_active();
		
		void ok();
		void error();

		void read_configuration(const int &config);
		void write_configuration(const int &config, const String &str, const int &val);

		int string_part_count(const String &str);
		String string_part(const String &str, const int &index);
};

#endif