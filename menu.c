#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"


//		action_t
struct action{
char* description;//[100];
void (* fonc)();///    	fonction
};


union type{//Contenu des items
menu_t* sousMenu;
action_t* action;
};


enum vu{//   Les 2 typr d'items
sousMenu,
action
};

//		items_t
 struct items{
 union type u ;//
 enum vu e;//
};


//		menu_t
struct menu{
 menu_t* menuParent;
char* description1;//[100];
int compteur;
items_t items[9];
};




menu_t* createMenu(const char* text){	// CREAT MENU
//menu_t menu
menu_t* menu = (menu_t*)malloc(sizeof(menu_t));// malloc menu
(*menu).menuParent=NULL;
(*menu).compteur=0;

char* copy=(char*)malloc(100*sizeof(char));// copy creation malloc
strcpy(copy,text);
(*menu).description1=copy;
return menu;
}



void addSubMenu(menu_t* m, menu_t* sm){		// ADD SOUS MENU
int i=(*m).compteur;
if (i>=9 || (*sm).menuParent!=NULL){printf("\npas de place\n");
}else{
 (*sm).menuParent=m;
(*m).items[i].u.sousMenu=sm;(*m).items[i].e=0;
(*m).compteur++;

}
}


void addMenuAction(menu_t* m, const char* text, void(*f)()){	// ADD MENU ACTION
int i=(*m).compteur;
if (i>=9){printf("\npas de place\n");}else{

action_t* act=(action_t*)malloc(sizeof(action_t));//	malloc action

char* copy=(char*)malloc(100*sizeof(char));// copy creation malloc
strcpy(copy,text);
(*act).description=copy;
(*act).fonc=f;

(*m).items[i].u.action=act;(*m).items[i].e=1;
(*m).compteur++;

}
}


void deleteMenu(menu_t* m){//			delate menu
int i=0;
while( ( (*m).items[i].u.sousMenu!=NULL || (*m).items[i].u.action!=NULL ) && (i<9)){
	if ((*m).items[i].e==0){
		deleteMenu((*m).items[i].u.sousMenu);
  }else if ((*m).items[i].e==1){
		free( (*(*m).items[i].u.action).description);
		free((*m).items[i].u.action);
	}
i++;
}
free((*m).description1);
free(m);
}






void launchMenu(menu_t* m){//			Lunche menu

printf("\n %s\n\n",(*m).description1 );//main menu
int i=0;
do{
	if ((*m).items[i].e==1){
	printf("   %d - %s",i+1,( (*(*m).items[i].u.action).description  ));// action
  } else if ((*m).items[i].e==0){
	printf("   %d - %s",i+1,( (*(*m).items[i].u.sousMenu).description1) );//sous menu
}
printf("\n\n");
i++;
}while(i<(*m).compteur);


char c=0;i=0;

while( !(c=='q' && (*m).menuParent != NULL) && ( i<1 || i>(*m).compteur) ){

printf("choix?");
	scanf(" %c",&c);

printf("\n\n");i=c-48;

}

if (c=='q'){ launchMenu((*m).menuParent);//  menu parent
}else{
	i=i-1;
 if ((*m).items[i].e==1){ //action
	 ((*(*m).items[i].u.action).fonc)();// action fonction erreur

 }else if ((*m).items[i].e==0){ //sousMenu
  launchMenu((*m).items[i].u.sousMenu);// sous menu
 }

}
//fin lunch
}

