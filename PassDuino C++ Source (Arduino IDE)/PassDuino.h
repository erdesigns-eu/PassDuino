/*

	PassDuino
	Erdesigns.eu - Ernst Reidinga

	Version 1.0 (August 2020)

	Description:
	- Type passwords as a simulated keyboard

*/

#ifndef PassDuino_h
#define PassDuino_h

#include <Arduino.h>

class PassDuino {
	private:
		int eepromSize;
		int eepromOffset;
		int stringLength;
		int typeDelay;

		int default_password_index;
		bool return_after_typing;

		String get_password(const int &index);
		int get_configuration(const int &config);
	public:
		PassDuino(const int &eepromSize, const int &eepromOffset);

		int max_passwords();

		void init();

		void type_default_password();
		void type_string(const String &str);
		void type_password(const int &index);
};

#endif