#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <stdint.h>

#include "wave.h"


// : int8_t, int16_t, int32_t, uint8_t, uint16_t et uint32_t


//		Construction, destruction, édition de base

struct wave_t{

int8_t Riff[5];	//RIFF
uint32_t S;   	// Taille S du fichier en octets moins 8 
int8_t Wave[5];	//WAVE
int8_t fmt[5];	//'fmt '
int32_t nombre;	//16
int16_t PCM;	//1
uint16_t c;	//Nombre de canaux c, de 1 à 6
uint32_t f;	//Fréquence f d'échantillonnage en Hertz
uint32_t r;	//Nombre d'octets par seconde r
uint16_t b;	//Nombre d'octets par bloc b
uint16_t p;	//Précision p des échantillons en bits (8,16,24)
int8_t Data[5];
uint32_t D;	//Taille D des données en octets

int64_t* BLOC;

};


//		Construction, destruction, édition de base

wave_t* wave_new(uint32_t f, uint16_t p, uint16_t c, uint32_t B){
	wave_t* w =(wave_t*)malloc(sizeof(wave_t));
	strcpy(w->Riff,"RIFF\0");
	strcpy(w->Wave,"WAVE\0");
	strcpy(w->fmt, "fmt \0");
	strcpy(w->Data,"data\0");


	w->nombre = 16;
	w->PCM = 1;
	w->f=f;
	w->p=p;
	w->c=c;
	w->b=(c*(p/8));
	w->r=(f* (w->b) );
	w->D=(B* (w->b) );
	w->S=(w->D+44-8);

	w->BLOC=(int64_t*)malloc( (w->D/(w->p/8) )*sizeof(int64_t));
    	int32_t i;
    	for (i=0; i<(w->D/(w->p/8) ) ;i++){
    	w->BLOC[i]=0;
 	}
	return w;
}


void wave_delete(wave_t* w){//		DELETE
	free(w->BLOC);
	free(w);
}


int64_t wave_get(wave_t* w, uint32_t i, uint16_t j){
//	LIRE ieme bloc jeme canal
	int64_t v=0;
	 v=(  w->BLOC[(i*w->c)+j ] );
	return v;
}

int64_t fact(int64_t p){
	int64_t a=1;
	int64_t i;
	for(i=0;i<p;i++){a=a*2;}
	return a;
}


void wave_set(wave_t* w, uint32_t i, uint16_t j, int64_t a){
//  ECRIRE a dans ieme bloc jeme canal
	int64_t b=(fact(w->p-1)-1);			
	int64_t c=( (-1)*(fact(w->p-1)) );
	if(a>=b){a=b;}else if (a<=c){a=c;}
 	w->BLOC[(i*w->c)+j]=a;
}


void add_signal(wave_t* w, double (*s)(double)){
//Ecrire les resultat d'un fonction s dans le 1er cannal des blocs???
	uint32_t i;
	int64_t a;
	double cpt=0;
	for(i=0; i<( (w->D)/(w->b) );i++){			
	if(s(cpt)<0){
	a= s(cpt)*(fact(w->p-1));
	}else{
	a= s(cpt)*(fact(w->p-1)-1);
	}
	cpt++;
	wave_set(w,i,(w->c)-1,a);
	}
}



//		Entrée/sortie

wave_t* wave_load(const char* fname){

	wave_t* wave=malloc(sizeof(wave_t));

	FILE* fichier=NULL;
	fichier = fopen(fname,"r");
	if (fichier==NULL){fclose(fichier);return NULL;
	}///////////////////////////verif fopen

	
	char c[5]={0};
	fgets(c,5,fichier);//"RIFF"
	if ( 0!=strcmp("RIFF",c) ){fclose(fichier);return NULL;
	}///////////////////////////verif RIFF
								strcpy(wave->Riff,"RIFF\0");

	fread(&(wave->S),sizeof(int32_t),1,fichier);//w.S

	fgets(c,5,fichier);//"WAVE"
	if ( 0!=strcmp("WAVE",c) ){fclose(fichier);return NULL;
	}///////////////////////////verif WAVE
								strcpy(wave->Wave,"WAVE\0");
	fgets(c,5,fichier);//"fmt "
	if ( 0!=strcmp("fmt ",c) ){fclose(fichier);return NULL;
	}///////////////////////////verif 'fmt '
								strcpy(wave->fmt,"fmt \0");
	int a=0;
	fread(&a,sizeof(int32_t),1,fichier);//16
	if ( a!=16 ){fclose(fichier);return NULL;
	}///////////////////////////16
								wave->nombre = 16;
	short b=0;
	fread(&b,sizeof(int16_t),1,fichier);
	if (b!=1){fclose(fichier);return NULL;
	}///////////////////////////1
								wave->PCM = 1;
	fread(&(wave->c),sizeof(int16_t),1,fichier);
	fread(&(wave->f),sizeof(int32_t),1,fichier);
	fread(&(wave->r),sizeof(int32_t),1,fichier);
	fread(&(wave->b),sizeof(int16_t),1,fichier);
	fread(&(wave->p),sizeof(int16_t),1,fichier);
	fgets(c,5,fichier);				
	if ( 0!=strcmp("data",c) ){fclose(fichier);return NULL;
	}///////"data"
									strcpy(wave->Data,"data\0");
	fread(&(wave->D),sizeof(int32_t),1,fichier);
	wave->BLOC= malloc((wave->D/(wave->p/8) ) *sizeof(int64_t) );
	int64_t i;
	for(i=0; i<(wave->D/(wave->p/8) );i++){
	wave->BLOC[i]=0;
        fread( &(wave->BLOC[i]),(wave->p/8),1,fichier);
	if( (wave->BLOC[i])>(fact(wave->p-1)-1) ){wave->BLOC[i]= (wave->BLOC[i]|~0<<wave->p);}
	}////								 attention
	fclose(fichier);
	return wave;
}




bool wave_save(const char* fname, wave_t* w){


	FILE* fichier=NULL;
	fichier = fopen(fname,"w");
	if (fichier==NULL ){
	fclose(fichier);
	return false;
	}///verif fopen

	size_t t;int q;
	t=fwrite(w->Riff,sizeof(int8_t),4,fichier);//"RIFF"
	if(t!=4 ){fclose(fichier);return false;}

	t=fwrite(&(w->S),sizeof(int32_t),1,fichier);
	if(t!=1 ){fclose(fichier);return false;}

	t=fwrite(w->Wave,sizeof(int8_t),4,fichier);//"WAVE"
	if(t!=4 ){fclose(fichier);return false;}
	
	t=fwrite(w->fmt ,sizeof(int8_t),4,fichier);//"fmt "
	if(t!=4 ){fclose(fichier);return false;}

	int a=16;
	t=fwrite(&(w->nombre),sizeof(int32_t),1,fichier);
	if(t!=1 ){fclose(fichier);return false;}
	

	short b=1;
	t=fwrite(&(w->PCM),sizeof(int16_t),1,fichier);
	if(t!=1 ){fclose(fichier);return false;}

	t=fwrite(&(w->c),sizeof(int16_t),1,fichier);
	if(t!=1 ){fclose(fichier);return false;}
	t=fwrite(&(w->f),sizeof(int32_t),1,fichier);
	if(t!=1 ){fclose(fichier);return false;}
	t=fwrite(&(w->r),sizeof(int32_t),1,fichier);
	if(t!=1 ){fclose(fichier);return false;}
	t=fwrite(&(w->b),sizeof(int16_t),1,fichier);
	if(t!=1 ){fclose(fichier);return false;}
	t=fwrite(&(w->p),sizeof(int16_t),1,fichier);
	if(t!=1 ){fclose(fichier);return false;}

	t=fwrite(w->Data,sizeof(int8_t),4,fichier);
	if(t!=4 ){fclose(fichier);return false;}

	t=fwrite(&(w->D),sizeof(int32_t),1,fichier);
	if(t!=1 ){fclose(fichier);return false;}

	int32_t i;
	for(i=0;i<( w->D/(w->p/8) );i++){
        t=fwrite(&(w->BLOC[i]),(w->p/8),1,fichier);
        if(t!=1 ){fclose(fichier);return false;}
	}	
	fclose(fichier);
	return true;
}




//						Transformations de WAVE



void wave_canal(wave_t* w, uint16_t c){	

	wave_t* w1=wave_new(w->f,w->p,c,w->D/w->b); 

	uint32_t i;uint16_t j;
	if(c==(w->c)){
	wave_delete(w1);
	return;
	}else if(c< w->c){

	int cpt;
	for (i=0;i<( w->D/w->b);i++){
	cpt=0;

	for (j=c;j<w->c;j++){   cpt=cpt+wave_get(w,i,j);}
	for(j=0;j<c;j++){ wave_set(w1,i,j, (wave_get(w,i,j)+cpt)/( (w->c)-(c-1) ) );}
	}
	}else{

	for (i=0;i<(w->D/w->b);i++){
	for(j=0; j<(w->c) ; j++){
        wave_set(w1,i,j, wave_get(w,i,j));
     	}
	}
	for (i=0;i<( w->D/w->b );i++){
	for (j=(w->c);j<c;j++){
	wave_set(w1,i,j,wave_get( w1,i, j-(w->c) ) );
     	}
	}

	}
	free(w->BLOC);
	(*w)=(*w1);
	free(w1);
return;
}

//////////////////////////////////////////////////////////////////////////////////////////////


void wave_reverse(wave_t* w){
	uint32_t B = (w->D)/(w->b);
	wave_t* temp =wave_new(w->f, w->p, w->c,B);
	int i = 0;
	int j = 0;
	int k = 0;
		for(i = ((temp->D)/(temp->b))-1 ; i >= 0 ; i--){
		for(j = 0 ; j < w->c ; j++){//					ex c=2 ; 0,1 
			temp->BLOC[k*temp->c + j] = w->BLOC[i*w->c + j];
	}	
	k++;
	}
	free(w->BLOC);
	(*w) = (*temp); ///!!tester avec une copie du fichier d'origine!!///
	free(temp);
return;
}



wave_t** wave_split(wave_t* w, int* pc){
	wave_t** wtab = (wave_t**)malloc(w->c*sizeof(wave_t*));
	uint16_t k;
	uint32_t i;
	wave_t* cop;
	uint32_t B = (w->D)/(w->b);
	for(k = 0 ; k < (w->c) ; k++){
	cop= wave_new(w->f,w->p,1,B);
	for(i = 0 ; i < B ; i++){
	cop->BLOC[i*cop->c + 0] = w->BLOC[i*w->c + k];
	}
	wtab[k]=cop;
	}
	if(pc!=NULL)(*pc)=w->c;
	wave_delete(w);
return wtab;
}


wave_t* wave_merge(wave_t** w, int c){
	uint32_t B = (w[0]->D)/(w[0]->b);		
	wave_t* wav = (wave_t*)wave_new(w[0]->f,w[0]->p,c,B);
	uint32_t i= 0;	uint32_t j= 0;
	for(i = 0; i < B; i++){
	for(j = 0; j < c; j++){
	wav->BLOC[i*wav->c + j] = w[j]->BLOC[i*w[j]->c + 0];
	}}
	for(j=0;j< c;j++){wave_delete(w[j]);}///free  ?????
	free(w);
return wav;
}



wave_t* wave_crop(wave_t* w, uint32_t start, uint32_t length){
	int B = length-start;
	if(B < 0 && (length+start)>(w->D/w->r) ) return NULL;
	wave_t* wav =wave_new(w->f,w->p,w->c,B);
	int i = 0;int j = 0;int k = start;
	for(i = 0 ; i < B ; i++){
	for(j = 0; j < w->c ; j++){
	wav->BLOC[i*w->c + j] = w->BLOC[k*w->c + j];
	}k++;
	}
return wav;
}



wave_t* wave_scale(wave_t* w, double s){ // à vérifier
	if(w == NULL){ fprintf(stderr,"impossible de lancer la fonction wave_scale"); return NULL;
	}else if(s <= 0){return w;}
	uint32_t B = (w->D/w->b); 
	wave_t* wav = (wave_t*)wave_new(w->f*s,w->p,w->c,B);
	int i = 0;int j = 0;
    	for(i = 0 ; i < (w->D)/(w->b); i++){  
        for(j = 0 ; j <w->c ; j++){
        wav->BLOC[i*wav->c + j] = w->BLOC[i*w->c + j];
        }}
return wav;
}


//////////////////////////////////////// SUPLEMENTAIRE


void wave_concat(wave_t* w1, wave_t* w2){ // le fichier WAVE w2 est ajouté à la suite du fichier WAVE w1
	if (w1->r != w2->r){ fprintf(stderr,"Ces deux WAVE ne peuvent pas être concaténer"); return;}
	int B = (w1->D)/(w1->b) + (w2->D)/(w2->b); // B1 + B2
	wave_t* temp = wave_new(w1->f,w1->p,w1->c,B);
	int i = 0;int j = 0;
	for(i = 0 ; i < (w1->D/w1->b) ; i++){ // B1
	    for(j = 0 ; j < temp->c ; j++){
	        temp->BLOC[i*temp->c + j] = w1->BLOC[i*w1->c + j];
	    }    
	}
	int k = 0;
	for(i = (w1->D/w1->b) ; i < B ; i++){ // B1; B2
	    for(j = 0 ; j < temp->c ; j++){
	        temp->BLOC[i*temp->c + j] = w2->BLOC[k*w2->c + j];
	    }
	k++;
	}
	(*w1)=(*temp);
	wave_delete(temp);
return;
}





wave_t* wave_cut(wave_t* w, int32_t start, int32_t length){ // fonction inverse de la fonction wave_crop: renvoi le WAVE sans la partie entre start et length
	int test = length-start;
	if(test < 0 || test > (w->D/w->b)) return NULL; // B
	int B = (w->D/w->b)-(length-start);
	wave_t* temp = wave_new(w->f,w->p,w->c,B);
	int i = 0;int j = 0;
	    for(i = 0 ; i < start ; i++){
	        for(j = 0; j < w->c ; j++){
	            temp->BLOC[i*temp->c + j] = w->BLOC[i*w->c + j];
	        }
	    }
	int k = i;
	    for(i = length ; i < (w->D/w->b) ; i++){
	        for(j = 0; j < w->c ; j++){
	            temp->BLOC[k*temp->c + j] = w->BLOC[i*w->c + j];
	        }
	    k++;
	    }
return temp;
}




//wave_t* wave_scale_bis(wave_t* w, int32_t start, int32_t length, double s)
//void wave_deleteC(wave_t* w, int c);
void wave_deleteC(wave_t* w, int c){ // supprime le c'ième canal dans w
	if(w == NULL || w->c < c || w->c == 1 || c <= 0) return ;
	uint32_t B = (w->D/w->b); 
	wave_t* temp = (wave_t*)wave_new(w->f,w->p,w->c-1,B);
	int i = 0;
	int j = 0;
	int k = 0;
	for(i = 0 ; i < (w->D)/(w->b); i++){
	for(j = 0 ; j < w->c-1 ; j++){
	if(k == c-1){ k++;}
	temp->BLOC[i*temp->c + j] = w->BLOC[i*w->c + k];
	k++;
	}}
	free(w->BLOC);
	(*w)=(*temp);
	free(temp);
}


////////////////////////////////////////////////////////////////////////////////////////////////+++++++++++++++++++++++++++++





void wave_modif( uint16_t p, uint16_t c,uint32_t f, wave_t*w){
	wave_t* wA=wave_new(f,p,c,w->D/w->b);
	wave_canal(w,c);
	uint32_t i;uint16_t j;
	int64_t a;
	double cpt=0;
	for(i=0; i<( (w->D)/(w->b) );i++){
	for(j=0; j<w->c;j++){		
	if(wave_get(w,i,j)<0){
	wave_set(wA,i,j,( wave_get(w,i,j)/(fact(w->p-1)    )*fact(wA->p-1) )  );
	}else{
	wave_set(wA,i,j,( wave_get(w,i,j)/(fact(w->p-1)-1) )*(fact(wA->p-1)-1)  );
	}
	}}
	free(w->BLOC);
	(*w)=(*wA);
	free(wA);
return;
}

void type(wave_t* w){
	printf("Fichier de type WAVE, précision %hi bites, %hi canaux, fréquence de %d Hertz.\n",w->p,w->c,w->f);
	int32_t s=(w->D/w->r);
	int32_t h=s/3600;	s=s-(h*3600);
	int32_t m=s/60;		s=s-(m*60);
	printf("Durée approximative en (hh:mm:ss) %.2d:%.2d:%.2d \n",h,m,s);
}



void convertsb(uint32_t start, uint32_t length, wave_t* w){
	start=(start*(w->r))/w->b;
	length=(length*(w->r))/w->b;
return;
}





















