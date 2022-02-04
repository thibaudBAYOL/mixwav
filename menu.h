
#ifndef thibaud_0_menu_h
#define thibaud_0_menu_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct menu menu_t;

typedef struct action action_t;

typedef struct items items_t;


menu_t* createMenu(const char* text);

void addSubMenu(menu_t* m, menu_t* sm);

void addMenuAction(menu_t* m, const char* text, void(*f)());

void deleteMenu(menu_t* m);

void launchMenu(menu_t* m);


#endif
