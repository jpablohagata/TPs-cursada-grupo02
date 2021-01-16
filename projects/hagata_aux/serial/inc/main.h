#include "sapi.h"

#define BAUD_RATE 115200
#define MAX_LENGTH 200

bool_t my_strcmp(char *, char *);
void my_strcpy(char * s1, char * s2);
void json_parse(char * json, char * ssid, char * pass);
