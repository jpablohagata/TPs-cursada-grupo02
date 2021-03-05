#include "sapi.h"

#define BAUD_RATE 115200
#define MAX_LENGTH 250

void wifi_parse(char * str, char * ssid, char * pass);
bool_t my_strcmp(char * s1, char * s2);
void cwjap(char * out, char * ssid, char * pass);
void cip(uint16_t temp, char * send, char * get);
uint16_t ajuste_analogico(uint16_t in);
void itoa(uint16_t val, char * str);
uint16_t my_strlen(char * str);
