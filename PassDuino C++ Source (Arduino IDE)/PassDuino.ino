/*

  PassDuino.
  Erdesigns.eu - Ernst Reidinga

  Version 1.0 (August 2020) 

  First Release, for private use at this current moment :)

  *** Lock your PC (Windows/Linux) when the COM port closes (USB Removal),
  and enter the password on insertion. ***

  You can use the configurator to switch passwords, so you can use it
  to store more passwords. The settings are password protected, you can
  set the password for the settings in the constructor.

  When using the configuration tool (Windows/Linux) you can use it to
  quickly enter the password you select in the configuration tool.
  This allows you to take your passwords with you in a safe manner,
  and be able to use very long or difficult passwords.

  If you set the password length to 20 characters, you should be able to
  save up to 50 passwords in the EEPROM. Should be more then enough for 
  most people :)

  ********************************************************************************
  
  Note 1:
  You can change the global variables in the Variables.h and Variables.cpp files,
  i stored them in there so i dont have to redeclare the variables, and i have
  all String's in one place.

  Note 2:
  The configuration integers are used for the settings, i personally use index
  0 to 3 for the settings.
  - 0: String Length (default 20 characters)
  - 1: Default Password index (This password will auto type on USB insert)
  - 2: Press Return after typing (0 = false, 1 = true)
  - 3: Type delay in seconds, set to 0 to start typing directly. I use this
       so i have the time to select another window when i use the configuration
       software to enter a selected password.
  - 4: This is set to FIRSTRUNKEY, this is a check to see if this is the "first" run,
       if this is the first run (with a empty EEPROM) the configurator will set default
       values for INT config 0-4 and a default password at index 0.

  ********************************************************************************

  ** The definition for the EEPROM Offset (bytes), EEPROM Size (bytes) and the
  Password used for configurating are set here for convenience. **

  I use this on the ATmega32U4 chip, with a CJMCU-32 USB stick. This chip has:
  - 32KB Program Memory (Flash)
  - 1KB EEPROM Memory
  - 2.5KB SRAM Memory
  - 16 MHZ Clock speed

  ** https://www.microchip.com/wwwproducts/en/ATmega32u4 **
  ** https://www.banggood.com/CJMCU-32-Virtual-Keyboard-Badusb-For-Leonardo-USB-ATMEGA32U4-p-1098876.html **

  Other Keyboard Layouts - create a backup of the Keyboard.cpp file and update with the layout found here:
  - https://github.com/Dukweeno/LocaleKeyboard.js/tree/master/locales

  ** Keyboard.cpp can be found here: arduino-x.x.x/libraries/Keyboard/src **

*/

#include "Keyboard.h"
#include "Variables.h"
#include "Configurator.h"
#include "PassDuino.h"

#define EEPROMOFFSET 0
#define EEPROMSIZE 1024

#define PASSWORD "YOURPASSWORDHERE"

Configurator configurator(EEPROMSIZE, EEPROMOFFSET, PASSWORD);
PassDuino passduino(EEPROMSIZE, EEPROMOFFSET);

char character;
String command;

void setup() {
  Keyboard.begin();
  delay(KEYLONGDELAY);  

  // On plugging the USB we want to type the default password.
  passduino.type_default_password();
  
  // Then we want to wait for a connection on the COM Port
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

}

void loop() {
  
  // Here we will have a connection on the COM Port, and we'll read
  // the commands send over the COM Port. This allows using the
  // configuration tool to type passwords, but also allows us
  // to add/delete/change passwords and settings with the configuration
  // software (Windows / Linux).
  while (Serial.available() > 0) {

    // Read the input from the Serial Port
    character = Serial.read();

    // If the character is a newline or carriagereturn we want 
    // to process the command.
    if (character == CR || character == LF) {

      // Get the CMD from the command string and convert to uppercase.
      String cmd = configurator.string_part(command, 0);
      cmd.toUpperCase();
      
      // Flush the Serial Port
      Serial.flush();
     
      // Start Configurator
      if (cmd == STRCONFSTART && !configurator.is_active()) {
        if (configurator.start(configurator.string_part(command, 1))) {
          passduino.init();
        }
      } else

      // Stop Configurator
      if (cmd == STRCONFSTOP && configurator.is_active()) {
        configurator.stop();
      } else

      // If the configurator is active, we want to respond to
      // commands send from the Configuration software.
      if (configurator.is_active()) {

        // Set configuration - write to EEPROM
        if (cmd == STRCONFSET) {

          // Set CMD
          cmd = configurator.string_part(command, 1);
          cmd.toUpperCase();

          // Write Int
          if (cmd == STRCONFINT) {
            configurator.write_configuration(configurator.string_part(command, 2).toInt(), "", configurator.string_part(command, 3).toInt());
            // When settings are written like string length, we need to reinitialize the passduino because it still has the "old" string length
            // and/or press return key / type delay settings. So when we use the configurator and update the string length and rewrite the
            // passwords - the passduino must know what the new stringlength is because else it wont be able to read valid strings.
            passduino.init();
          } else

          // Write String (password)
          if (cmd == STRCONFSTR) {
            configurator.write_configuration(configurator.string_part(command, 2).toInt() + STRCONFOFFSET, configurator.string_part(command, 3), -1);
          } else

          // All other commands are invalid.
          {
            configurator.error();
          }

        } else 

        // Get configuration - read from EEPROM
        if (cmd ==  STRCONFGET) {

          // Set CMD
          cmd = configurator.string_part(command, 1);
          cmd.toUpperCase();

          // Read Int
          if (cmd == STRCONFINT) {
            configurator.read_configuration(configurator.string_part(command, 2).toInt());
          } else

          // Read String (password)
          if (cmd == STRCONFSTR) {
            configurator.read_configuration(configurator.string_part(command, 2).toInt() + STRCONFOFFSET);
          } else

          // All other commands are invalid.
          {
            configurator.error();
          }

        } else

        // Execute action, used to type the password set by
        // the configuration software, but you can add more
        // commands if you like to extend the functionality.
        if (cmd == STRCONFEXEC) {

          // Set CMD
          cmd = configurator.string_part(command, 1);
          cmd.toUpperCase();

          // Type the password at index
          if (cmd == STRCMDTYPE) {
            passduino.type_password(configurator.string_part(command, 2).toInt());
          }

          // Dump all passwords. This can come handy if you want all your stored passwords
          // in a txt file for quick searching.
          if (cmd == STRCMDDUMP) {
            for (int i = 0; i < passduino.max_passwords(); ++i) {
              passduino.type_password(i);
            }
          }

          // Clear all passwords
          if (cmd == STRCMDCLEAR) {
            configurator.clear();
          }

        } else

        // All other commands are invalid.
        {
          configurator.error();
        }

      } else 

      // If the configurator is not active, and we receive commands other
      // than starting the configuration or request for a reboot -
      // we send an error.
      {
        configurator.error();
      }

      // Clear the command, so we can read the next command.
      command = "";

    } else 
    // If the character is not the end of a command, we want to add 
    // the character to the command.
    {
      command.concat(character);
      delay(10);
    }

  }

}
