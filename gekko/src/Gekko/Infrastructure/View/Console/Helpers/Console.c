#include "Console.h"

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/***********************************
*
* @Name: Console_print
* @Def: Prints message for the app
* @Arg: message: char*
*
***********************************/
void Console_print(char * message) {

    char text[150];

    sprintf(text, "[%s] %s", APP_NAME, message);
    write(STDOUT_FILENO, text, strlen(text));
}

/***********************************
*
* @Name: Console_printn
* @Def: Prints message for the app with break
* @Arg: message: char*
*
***********************************/
void Console_printn(char * message) {
    Console_print(message);
    write(STDOUT_FILENO, "\n", 1);
}

/***********************************
*
* @Name: Console_printNext
* @Def: Prints next message for the app with break
* @Arg: message: char*
*
***********************************/
void Console_printNext(char * message) {
    char text[150];

    sprintf(text, "%s", message);
    write(STDOUT_FILENO, text, strlen(text));
}

/***********************************
*
* @Name: Console_printDouble
* @Def: Prints double for the app
* @Arg: number: double
*
***********************************/
void Console_printDouble(long long int number) {

    char text[150];

    sprintf(text, "%lld", number);
    Console_printNext(text);
}

/***********************************
*
* @Name: Console_printFloat
* @Def: Prints float for the app
* @Arg: number: float
*
***********************************/
void Console_printFloat(float number) {
    char text[150];

    sprintf(text, "%.2f", number);
    Console_printNext(text);
}

/***********************************
*
* @Name: Console_error
* @Def: Prints error
* @Arg: error: char*
*
***********************************/
void Console_error(char * error) {

    char text[150];

    sprintf(text, "Error: %s", error);
    Console_printn(text);
}

