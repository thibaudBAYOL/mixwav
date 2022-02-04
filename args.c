#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "args.h"


typedef struct option_s {
  const char* keyword;
  enum { OptVoid,OptInt, OptShort, OptString, OptFloat } spec;
  union {
    void (*opt_void)();
    void (*opt_int)(int);
    void (*opt_short)(int16_t);
    void (*opt_str)(const char*);
    void (*opt_float)(float);
  } fct;
  struct option_s* next;
} option_t;


option_t* opt_void(option_t* l, const char* kw, void (*f)()){

struct option_s* s =malloc(sizeof(option_t));
(*s).keyword=kw;
(*s).spec= OptVoid;
(*s).fct.opt_void=f;
(*s).next=NULL;


option_t* op=l;
option_t* oo=NULL;
while(op!=NULL){
oo=op;
op=(*op).next;
}
(*oo).next=s;

return l;

}


option_t* opt_int(option_t* l, const char* kw, void (*f)(int)){


struct option_s* s =malloc(sizeof(option_t));
(*s).keyword=kw;
(*s).spec= OptInt;
(*s).fct.opt_int=f;
(*s).next=NULL;

option_t* op=l;
option_t* oo=NULL;
while(op!=NULL){
oo=op;
op=(*op).next;
}
(*oo).next=s;

return l;
}

option_t* opt_short(option_t* l, const char* kw, void (*f)(int16_t)){


struct option_s* s =malloc(sizeof(option_t));
(*s).keyword=kw;
(*s).spec= OptShort;
(*s).fct.opt_short=f;
(*s).next=NULL;

option_t* op=l;
option_t* oo=NULL;
while(op!=NULL){
oo=op;
op=(*op).next;
}
(*oo).next=s;

return l;
}

option_t* opt_float(option_t* l, const char* kw, void (*f)(float)){


struct option_s* s=malloc(sizeof(option_t));
(*s).keyword=kw;
(*s).spec= OptFloat ;
(*s).fct.opt_float=f;
(*s).next=NULL;

option_t* op=l;
option_t* oo=NULL;
while(op!=NULL){
oo=op;
op=(*op).next;
}
(*oo).next=s;

return l;
}


option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*)){


struct option_s* s=malloc(sizeof(option_t));
(*s).keyword=kw;
(*s).spec= OptString;
(*s).fct.opt_str=f;
(*s).next=NULL;


if (l==NULL){
l=s;
}else{

option_t* op=l;
while(op->next!=NULL){
op=(*op).next;
}
(*op).next=s;

}

return l;
}

void opt_delete(option_t* l){
	if(l!=NULL)return;
	opt_delete(l->next);
	free(l);
}


//Hungry->

void process_arguments(option_t* l, int argc, char* *argv){
option_t* pt;
int i;

for(i = 1; i < argc; i++){
 pt=l;
while( 0!=strcmp(pt->keyword, argv[i] ) && (pt!=NULL) ){

pt=pt->next;
}


if(pt->spec== OptVoid){
pt->fct.opt_void();
}else if(pt->spec== OptInt){
i++;
pt->fct.opt_int(strtol(argv[i],NULL,10));
}else if(pt->spec== OptShort){
i++;
pt->fct.opt_short(strtol(argv[i],NULL,10));
}else if(pt->spec== OptString){
i++;
pt->fct.opt_str(argv[i]);
}else if(pt->spec== OptFloat){
i++;
pt->fct.opt_float(strtod(argv[i],NULL));
}

}
return;
}

