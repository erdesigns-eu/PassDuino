/*

	PassDuino Variables
	Erdesigns.eu - Ernst Reidinga

	Version 1.0 (August 2020)

	Description:
	- Global variables used in Configurator and PassDuino.

*/

#ifndef Variables_h
#define Variables_h
#include <Arduino.h>

const extern String CNLN;
const extern String CRLF;

const extern char CR;
const extern char LF;

const extern char DIVIDER;

const extern String STROK;
const extern String STRER;

const extern String STRSTART;
const extern String STRSTOP;

const extern String DEFAULTPASS;

const extern String STRCONFSTART;
const extern String STRCONFSTOP;
const extern String STRCONFSET;
const extern String STRCONFGET;
const extern String STRCONFSTR;
const extern String STRCONFINT;
const extern String STRCONFEXEC;
const extern String STRCMDTYPE;
const extern String STRCMDDUMP;
const extern String STRCMDCLEAR;

const extern int STRCONFOFFSET;
const extern int KEYSMALLDELAY;
const extern int KEYLONGDELAY;
const extern int FIRSTRUNKEY;

#endif