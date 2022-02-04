
#ifndef Thibaud_8_args_h
#define Thibaud_8_args_h



#include <stdlib.h>
#include <stdio.h>
#include <string.h>

		#define NOOPTION ((option_t*)NULL)


typedef struct option_s option_t;

option_t* opt_void(option_t* l, const char* kw, void (*f)());

option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*));

option_t* opt_int(option_t* l, const char* kw, void (*f)(int));

option_t* opt_float(option_t* l, const char* kw, void (*f)(float));

void opt_delete(option_t* l);

//Hungry->

void process_arguments(option_t* l, int argc, char* *argv);


#endif

