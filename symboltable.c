/**
 * File name: symboltable.c
 * Author Name: Navid Nafiuzzaman <mxn4459@cs.rit.edu>
 * This file implements the functionality of symbol table.
 * It takes file as an input and converts the contents of the 
 * file into a symbol table with name, type and value,
 * mapADT.h - A hash map structure is reused from Homework 6
 * for the data structure of the symbol table. 
 * Credit for mapADT.h interface: ckd, wrc
 **/

#define MAX_NAME_LEN    7:
#include "mapADT.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symboltable.h"
#include <fenv.h>
#include <math.h>

/**
 * This is the equals method used by the mapADT to create
 * the table
 * @param const void* a : casted as symbol
 * @param const void *b: two void pointers
 * which are casted as symbol
 * @return boolean indicating whether the two pointers are 
 * equal or not
 *
 **/
bool equals(const void *a, const void *b){
    Symbol* a1 = (Symbol*) a;
    Symbol* b1 = (Symbol*) b;
    if (strcmp(a1->name,b1->name)== 0) 
        return true;
    return false;
}

/**
 * An opener method which take char pointers
 * as input and opens the file, reads the file
 * line by line and makes a symbol table using the content of the file
 * @param char*f: whch is the name of the file
 * @return the symbol table created from the given file.
 **/
symtab opener(char *f){ // memory management
	FILE* cf = fopen(f,"r"); //cf = current file
	if (cf ==NULL){
		fprintf(stderr,"Error reading the file %s\n", f);
		exit(EXIT_FAILURE);

	}
	char type[80];
	char name[80];
	float value;
	symtab rt = map_create(&equals);
	while (fscanf(cf, "%s %s %f", type, name, &value)==3){ //change it to strt
		Symbol* sym = malloc(sizeof(Symbol));
		strncpy(sym->name, name, 7);
		if (strcmp(type, "integer")==0){
			sym->type = Integer;
		}
                if (strcmp(type, "real")==0){
                        sym->type = Float;
                }
                switch (sym->type){
                        case Integer:
				sym->value.iVal = value;
                                break;
                   	case Float:
                           	sym->value.fVal =value;
                           	break;
		   	case Unknown:
				exit(EXIT_FAILURE);
                }
 	      map_put(rt, &sym->name, sym);
	}
	fclose(cf);
	return rt;
}

/**
 * This methos takes a symbol table, name and type as an
 * input and makes a entry in the table.
 * @param symtab tab: the symbol table
 * @param char* name: the name of the entry
 * @param type: the type of the entry
 * @return void
 **/
void create_tab(symtab tab, char* name, Type type){
	Symbol* sym = malloc(sizeof(Symbol));
	strncpy(sym->name, name, MAX_NAME_LEN+1);
	sym->type = type;
	
	switch (sym->type){
                   case Integer:
                           sym->value.iVal = 0;
                           break;
                   case Float:
                           sym->value.fVal = 0;
                           break;
		   case Unknown:
			exit(EXIT_FAILURE);

	map_put(tab, &sym->name, sym);

	}
}


/**
 * update_tab the methods which updates the existing 
 * content in the table.
 * @param symtab tab: the created symbol table
 * @param char* name: The name of the entry which is to be updated
 * @param float value: the float value of the entry which is the 
 * updated value
 * @return void 
 **/
void update_tab(symtab tab, char* name, float value){
	Symbol* rt  = map_get(tab,name);
	switch (rt->type){
                   case Integer:
                           rt->value.iVal = value;
                           break;
                   case Float:
                           rt->value.fVal = value;
                           break;
	           case Unknown:
			exit(EXIT_FAILURE);
              }

}	


/**
 * This method prints the table in an organized way
 * It prints the name, type and the value of the entry. 
 * @param symtab tab: the symbol table which is to be printed.
 **/
void printSymbolTable(symtab tab){
	printf("\n");
	printf("Symbol\tTable\tContents\t\n");
	printf("Name\tType\tValue\n"); 
	printf("=====================\n");
	Symbol** value = (Symbol**) mapValue(tab);
	for (int i=0; i < mapSize(tab); i++){
		switch (value[i]->type){
			case Integer:	
				printf("%s\t%s\t%d\n", value[i]->name, "integer",value[i]->value.iVal);
				break;
                   	case Float:
				printf("%s\t%s\t%f\n", value[i]->name, "real",value[i]->value.fVal);
				break;
			case Unknown:
				exit(EXIT_FAILURE);
		}	
	}
}


/**
 * This method gets the speicied name entry
 * in the table
 * @param symtab tab: the symbol table
 * @ param char* name: The character pointer of the name 
 * which we want to get the symbol for.
 * @return Symbol*: it returns a symbol pointer for the passed name 
 **/
Symbol* get_tab(symtab tab, char* name){
	return map_get(tab, name);
}
