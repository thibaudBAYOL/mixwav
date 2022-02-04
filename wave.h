
#ifndef Thibaud_4_wave_h
#define Thibaud_4_wave_h


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include <stdint.h>

// : int8_t, int16_t, int32_t, uint8_t, uint16_t et uint32_t




//		Construction, destruction, édition de base


typedef struct wave_t wave_t;


wave_t* wave_new(uint32_t f, uint16_t p, uint16_t c, uint32_t B);

void wave_delete(wave_t* w);


int64_t wave_get(wave_t* w, uint32_t i, uint16_t j);

void wave_set(wave_t* w, uint32_t i, uint16_t j, int64_t a);


void add_signal(wave_t* w, double (*s)(double));


//		Entrée/sortie

wave_t* wave_load(const char* fname);


bool wave_save(const char* fname, wave_t* w);



//		Transformations de WAVE

void wave_canal(wave_t* w, uint16_t c);


void wave_reverse(wave_t* w);

wave_t** wave_split(wave_t* w, int* pc);//

wave_t* wave_merge(wave_t** w, int c);//

wave_t* wave_crop(wave_t* w, uint32_t start, uint32_t length);

wave_t* wave_scale(wave_t* w, double s);

///////////////////suplement

void wave_concat(wave_t* w1, wave_t* w2);

void wave_inverse(wave_t* w);	//??????

wave_t* wave_cut(wave_t* w, int32_t start, int32_t length);

void wave_deleteC(wave_t* w, int c);

///////////////      PLUS

void wave_modif( uint16_t p, uint16_t c,uint32_t f, wave_t*w);

void type(wave_t* w);

void convertsb(uint32_t start, uint32_t length, wave_t* w);

#endif



