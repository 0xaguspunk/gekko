#ifndef _Console_h_
#define _Console_h_

#define APP_NAME "DOZER"

void Console_print(char * message);
void Console_printn(char * message);
void Console_printNext(char * message);
void Console_printDouble(long long number);
void Console_printFloat(float number);
void Console_promt(char * message);
void Console_error(char * error);
char* Console_read();


#endif
