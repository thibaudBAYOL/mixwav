

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


#include "menu.h"
#include "wave.h"
#include "args.h"
void E5();


wave_t* w=NULL;wave_t* w1=NULL; wave_t** tw=NULL;
char fname[50]="\0";
int pc;


void F(){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	printf("En cour de construction\n");
}




////////////////////////////////////  OUVRIR;
void F1(){char yn='n';
	if(w!=NULL){
	printf("Une source audio courant est deja charger!\n Voulez-vous la sauvgarder?(y=yes ou n=no):");
	scanf(" %c",&yn);
	}
	if(yn=='y'){
	E5();
	}else if(yn=='n'){
	if(w!=NULL){wave_delete(w);w=NULL;}
	printf("Entre le nom du fichier audio a charger:");scanf("%s",fname);
	w=wave_load(fname);
	if(w==NULL){fprintf(stderr,"erreure dans l'ouverture du fichier %s.\n",fname);return;}
	printf("Le fichier audio %s est charger.\n",fname);
	}
return;
}


/////////////////////////////////////////////ENREGISTRER;
void E0( uint16_t p, uint16_t c,uint32_t f){
	char yn='n';int i;
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	printf("Voulez-vous sauvgarder a partir du nom du fichier d'origine %s:?(y=yes,n=no):",fname);
	scanf(" %c",&yn);
	if(yn=='y'){
	return;
	}else if(yn=='n'){int i;
	for(i=0;i<50;i++)fname[i]='\0';
	printf("Entre le nom du fichier où il doit etre enregister(moin de 50 caracteres):");scanf("%s",fname);
	}
	if(f!=0 && p!=0 && c!=0 ){wave_modif(p,c,f,w);}
	bool y;
	y=wave_save(fname,w);if(y!=true){fprintf(stderr,"erreure dans l'enregistrement du fichier %s.\n",fname);return;}
	printf("Le fichier audio est enregistre dans le fichier %s\n",fname);
	wave_delete(w);w=NULL;
	for(i=0;i<50;i++)fname[i]='\0';
return;
}

void E1(){	E0(8,1,11025);return;}

void E2(){	E0(16,2,44100);return;}

void E3(){	E0(24,6,192000);return;}

void E4(){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	uint16_t p; uint16_t c; uint32_t f;
	do{
	printf("Entrer la precision (p) des echantillons(8,16 ou 24):");scanf("%hi",&p);
	}while(p!=8 && p!=16 && p!=24);
	do{
	printf("Entrer le nombre de cannaux (c) de 1 a 6:");scanf("%hi",&c);
	}while(c<1 || c>6);
	do{
	printf("Entrer la fréquence (f) des echantillons en Hertz:");scanf("%d",&f);
	}while(f<=0 || f>4294967294);
	E0(p,c,f);
return;
}

void E5(){	E0(0,0,0);return;}


/////////////////////////////////////////// INFORMATION
void I(){
if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
if(fname[0]!='\0')printf("Nom du fichier d'origine: %s\n",fname);
type(w);
}

//////////////////////////////////////////// QUITTER
bool cont = true;

void Q() {
	char yn='n';
	if(w1!=NULL)wave_delete(w1);
	if(w!=NULL){
	printf("Une source audio courant a été charger!\n Voulez-vous la sauvgarder?(y=yes,n=no):");
	scanf(" %c",&yn);
	if(yn=='y'){	E5();
	}else{
	wave_delete(w);w=NULL;
	}
	}
	cont = false;
}


void inverser(){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	wave_reverse(w);
	printf("Fichier %s a été inverser.\n",fname);
}

void decouper(){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	if(w1!=NULL){wave_delete(w1);w1==NULL;}
	uint32_t start; uint32_t length;
	printf("Entrer la date de depart (en seconds):");scanf("%d",&start);
	printf("Entrer la durée (en seconds):");scanf("%d",&length);
	convertsb(start,length,w);
	w1=wave_crop(w,start,length);
	if(w1!=NULL){
	printf("Découpe éffectuer.\n");
	}else{
	printf("Erreur de paramétre.\n");
	}
}

void cuter(){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	if(w1!=NULL){wave_delete(w1);w1==NULL;}
	uint32_t start; uint32_t length;
	printf("Entrer la date de depart (en seconds):");scanf("%d",&start);
	printf("Entrer la durée (en seconds):");scanf("%d",&length);
	convertsb(start,length,w);
	w1=wave_cut(w,start,length);
	if(w1!=NULL){
	printf("Découpe éffectuer.\n");
	}else{
	printf("Erreur de paramétre.\n");
	}
}

void redim(){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	double s;
	do{
	printf("Entre le ratio de redimentionnement (0.5= 2 fois moin vite 2= 2 fois plus vite):");scanf("%lf",&s);
	}while(s<=0);
	w=wave_scale(w,s); printf("Redimentionement éffectuer.\n");
}



void adS(){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	int32_t a=1;printf("Choisie un signal(1 sinus ou 2 cosinus):");scanf("%d",&a);
	//if(a==1)add_signal(w,sin);// math.h
	//if(a==2)add_signal(w,cos);// math.h
	printf("signale ajouter au dernier canal du fichier.\n");
}

void split(){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	tw=wave_split(w,&pc);
	printf("wave_split executer\n");
}
void merge(){
	if (tw==NULL){printf("Aucune tableaux de wave n'est charger!(wave_split)\n");return;}
	w=wave_merge(tw,pc);
	printf("wave_merge executer\n"); pc=0;
}

void mC(){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	int16_t c;
	do{
	printf("Conbiens de canal voulez-vous (1 a 6):");scanf("%hi",&c);
	}while(c<1 || c>6);
	wave_canal(w,c);
	printf("Modification du nombre de canaux effectuer\n");
}

void supC(){
	int c;
	printf("Qu'elle est le numero du canal a suprimer(0 a c):");scanf("%d",&c);
	wave_deleteC(w,c);
	printf("Supression du canal %d effectuer\n",c);
}



void wave_ajout(){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	if (w1==NULL){printf("Aucune source wave suplementaire n'est charger!\n");return;}
	wave_concat(w,w1);
}


/////////////////////////////////// args.c

void lire(const char* str) {
	if(w!=NULL){wave_delete(w);w=NULL;}
	strcpy(fname,str);
  	w=wave_load(str);
	if(w==NULL){fprintf(stderr,"erreure dans l'ouverture du fichier %s.\n",fname);return;}
	printf("Le fichier audio %s est charger.\n",fname);
}
void rein() {printf("rein\n");}
void ecrire(const char* str) {
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	bool y;
  	y=wave_save(str,w);if(y!=true){fprintf(stderr,"erreure dans l'enregistrement du fichier %s.\n",str);return;}
	printf("Le fichier audio est enregistre dans le fichier %s\n",str);
	wave_delete(w);w=NULL;
	int i; for(i=0;i<50;i++)fname[i]='\0';
}

void Mc(int16_t c){
	if (w==NULL){printf("Aucune source courante n'est charger!\n");return;}
	if(c<1 || c>6)return;
	wave_canal(w,c);
	printf("Modification du nombre de canaux effectuer\n");
}

int main(int argc, char** argv) { // teste arg[]
	option_t* opt = NOOPTION;
	opt = opt_string(opt,"-i", lire);	//ouvrire
	opt = opt_void(opt,"-CI", rein);	// rein
	opt = opt_string(opt,"-o", ecrire);	//enregistrer
//E...
	opt = opt_void(opt,"-I", I);		// info
	opt = opt_void(opt,"-Q", Q);		// quitter
	opt = opt_void(opt,"-i", inverser);	//inversion(debut/fin)
//decouper 2s
//redim double
//ad signal int
	opt = opt_void(opt,"-s", split);	// division
	opt = opt_void(opt,"-m", merge);	// regoupement
//modif canal c
	opt = opt_short(opt,"-l",Mc);

//suprim canal c

//?Hauteur
	opt = opt_void(opt,"-a", wave_ajout);	// ajout w+w1

	process_arguments(opt, argc, argv);
	opt_delete(opt);
////////////////////TESTE MENU

menu_t* m0;
menu_t* m;
menu_t* sm;
//	//	//	//	//Initialisation MENU //

	m0=createMenu("MainMenu");//	 0-
	       m=createMenu("Fichier");//0-1
	addSubMenu(m0,m);
addMenuAction(m,"Ouvrir",F1 );//	 	 0-1-a
sm=createMenu("Enregistrer");//		 0-1-b
	addSubMenu(m,sm);
//	        			     b-I
	addMenuAction(sm,"WAVE 8bits Mono 11.025kHz",E1);
//					     b-II
	addMenuAction(sm,"WAVE 16bits Stereo 44,1kHz",E2);
//					     b-III
	addMenuAction(sm,"WAVE 24bits 5.1 192kHz",E3);
//					     b-IV
	addMenuAction(sm,"WAVE customisé",E4);

	addMenuAction(sm,"WAVE d'origine",E5);
//					     b-(..)
//

addMenuAction(m,"Information",I);//	 0-1-c
addMenuAction(m,"Quitter",Q);//	 	 0-1-d

m=createMenu("Durée et tempo");//	 0-2
addSubMenu(m0,m);
addMenuAction(m,"Inverser",inverser);//	 0-2-a
addMenuAction(m,"Découper",decouper);//	 0-2-b	// création d'un w1 +
addMenuAction(m,"Découper (autour)",cuter);
addMenuAction(m,"Redimensionner",redim);//   0-2-c

m=createMenu("Canaux");//		 0-3
addSubMenu(m0,m);
//
addMenuAction(m,"add_signal",adS);//	0-3-a....

addMenuAction(m,"wave_split",split);//	0-3-b....

addMenuAction(m,"wave_merge",merge);//	0-3-c....

addMenuAction(m,"modif canal",mC);//		....

addMenuAction(m,"suprimer canal c",supC);//		....
//
m=createMenu("Hauteur");//		 0-4
addSubMenu(m0,m);
//
addMenuAction(m,"...",F);//		....
//
m=createMenu("Autre");//		 0-4
addSubMenu(m0,m);
addMenuAction(m,"Ajouter w1 dans w",wave_ajout);//		....

//	//	//	//	// FIN INIT MENU


while(cont) launchMenu(m0);

  deleteMenu(m);

  return EXIT_SUCCESS;
}

