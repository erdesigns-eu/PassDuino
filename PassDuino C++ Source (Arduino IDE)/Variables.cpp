/*

	PassDuino Variables
	Erdesigns.eu - Ernst Reidinga

	Version 1.0 (August 2020)

	Description:
	- Global variables used in Configurator and PassDuino.

	Note:
	- Change the STRCONOFFSET if you want to store more int values,
	  after this the rest space is for saving Strings (passwords).
	- Change the STRSTART and STRSTOP to your liking if you like to
	  have another message you can insert them there.

*/

#include "Arduino.h"
#include "Variables.h"

const extern String CNLN = "\r>";
const extern String CRLF = "\r\n";

const extern char CR = '\r';
const extern char LF = '\n';

const extern char DIVIDER = '|';

const extern String STROK = "OK";
const extern String STRER = "ERROR";

const extern String STRSTART = "Configuration mode started.";
const extern String STRSTOP  = "Configuration mode stopped.";

const extern String DEFAULTPASS  = "Empty Password";

const extern String STRCONFSTART = "STARTCONFIG";
const extern String STRCONFSTOP	 = "STOPCONFIG";
const extern String STRCONFSET 	 = "SET";
const extern String STRCONFGET 	 = "GET";
const extern String STRCONFSTR 	 = "STR";
const extern String STRCONFINT 	 = "INT";
const extern String STRCONFEXEC	 = "EXE";
const extern String STRCMDTYPE	 = "TYPE";
const extern String STRCMDDUMP	 = "DUMP";
const extern String STRCMDCLEAR	 = "CLEAR";

const extern int STRCONFOFFSET = 5;
const extern int KEYSMALLDELAY = 500;
const extern int KEYLONGDELAY  = 1000;
const extern int FIRSTRUNKEY   = 85;