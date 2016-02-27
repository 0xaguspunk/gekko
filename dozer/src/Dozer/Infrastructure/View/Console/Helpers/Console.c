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
* @Name: Console_promt
* @Def: Prints the promt of the app
* @Arg: user: User
*
***********************************/
void Console_promt(char * message) {

    char text[150];

    sprintf(text, "%s> ", message);
    Console_print(text);
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

/***********************************
*
* @Name: Console_read
* @Def: Reads the option selected
* @Ret: char*
*
***********************************/
char* Console_read() {

    char* text;
    char aux;
    int i;

    i = 0;
    read(STDIN_FILENO, &aux, sizeof(char));
    text = malloc(sizeof(char));

    while(aux != '\n') {
        text[i] = aux;
        i++;
        text = realloc(text, sizeof(char)*(i+1));
        read(STDIN_FILENO, &aux, sizeof(char));
    }

    text[i] = '\0';

    for (i = 0 ; text[i]; i++) {
        text[i] = tolower(text[i]);
    }

    return text;
}
