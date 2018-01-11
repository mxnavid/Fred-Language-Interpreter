/**
*Author: Navid Nafiuzzaman <mxn4459@rit.edu>
*File Name: mapADT.c
*Interface Author (mapADT.h): ckd, wrc
*This program creates map functionality in C 
*for any data type using Dunamic Memory Allocation. 
**/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/**
*Entry structure which will works as a linked list with the
*key, value, next and previous pointer. 
**/

typedef struct enter {
        void* key;
        void* value;
        struct enter* next;
        struct enter* prev;
} Entry;

/**
* Map structur contaiing the first  pointer and the 
* end pointer to the map. It also contains the function pointer
* for the equals.
**/
struct map {
	 bool (*equals)(const void *a,const void *b);
	 Entry* first;
	 Entry* end;
	 int size;
};

typedef struct map* MapADT;

#define _MAP_IMPL_
#include "mapADT.h"
/**
Create a MapADT which uses the supplied function as a comparison
routine.
 The comparison function takes two void* parameters, and returns
a Boolean result which indicates if the two things are identical.
@param equals the address of the equals function to be used for
determining if two keys are equal
 @return a pointer to a MapADT, or NULL if the allocation fails
 rt = the return pointer
**/
MapADT map_create( bool (*equals)(const void *a,const void *b) ){
	MapADT rt = malloc(sizeof(struct map));
	if (rt !=NULL){
		rt->equals = equals;
		rt->first = NULL;
		rt->end = NULL;
		rt->size = 0;
	}
	return rt;
}

/**
Tear down and deallocate the supplied MapADT.
 @param map - the MapADT to be manipulated
**/
void map_destroy( MapADT map ){
	map_clear(map);
	free(map);
}


/**
 *Remove all contents from the supplied MapADT, but do not
*deallocate the MapADT itself.
*@param map - the MapADT to be manipuated
**/

void map_clear( MapADT map ){
	Entry* n = map->first;
	while (n != NULL){
		Entry* next= n->next;
		free(n->key);
		free(n->value);
		free(n);
    		n = next;	
	}
map->size = 0;
map->first= NULL;
map->end = NULL;
}


/**
Check if the specified entry exists in the map
Uses the map's equals function to determine if an entry with
the same key already exists in the map. If so, then return
true.
@param key the key to lookup in the map
@return true if the entry exists, else false
*n  = pointer entry to go the next one
**/
bool map_contains(MapADT map, void *key){
	Entry* n = map->first;
	while (n != NULL){
		if (map->equals(n->key, key)){
			return true;
		}
		n = n->next;
	}
	return false;
}


/**
Initialize the map by allocating memory.
@param key: key for the entry
@param value : value for the key
@return the entry
**/
static Entry* initializer(void *key, void *value){
    Entry *rt = malloc(sizeof(Entry));
    if (rt !=NULL){
        rt->key = key;
        rt->value = value;
    	rt->next = NULL;
	rt->prev = NULL;
	}
    return rt;
}

/**
 * This function returns the size of the map. 
 * @param Map Structure
 * @return integer value of the size of the map. 
 **/
int mapSize(MapADT map){
        return map->size;
}

/**
 Put the specified entry into the Map
* Uses the map's equals function to determine if an entry with
* the same key already exists in the map. If so, then replace it 
* and return a pointer to the old value.
* @param map the MapADT into which the value is to be inserted
* @param key the key to be inserted
* @param value the value to be inserted
* @exception If the value cannot be inserted into the map for
* whatever reason (usually implementation-dependent), the program
* should terminate with an error message.  This can be done by
* printing an appropriate message to the standard error output and
* then exiting with EXIT_FAILURE, or by having an assert() fail.
**/
void* map_put( MapADT map, void *key, void *value){
	//checke map_contains
	//if contains = true; update the value of the desired key
	//if false; add it to the end of the list, update next and prev
	//for the first thing in the in the lost, the first andn end is going to same thing
	if(map_empty(map)){
		
        	Entry* rt = initializer(key, value);        
        	map->first = rt;
        	map->end = rt;
		map->size +=1;
        	return NULL;
	}
	if (!map_contains(map, key)){	
        		Entry* rt = initializer(key, value);        
        		Entry* tempr = map->end;
        		map->end = rt;
        		tempr->next = rt;
        		rt->prev = tempr;
			map->size +=1;
        		return NULL;
	}
    Entry* begin  = map->first;
    while(begin!=NULL){
        if (map->equals(key, begin->key)){
            void* oldValue = begin->value;
            begin->value = value;
            return oldValue;
        }
        begin = begin->next;

    }
    exit(EXIT_FAILURE);
    return NULL;
}



/**
Find the entry specified by the key and return a pointer to the value
param map the MapADT to be manipulated
return the value that corresponds to the key
exception If the map is empty, the program should terminate
with an error message.  This can be done by printing an
appropriate message to the standard error output and then
exiting with EXIT_FAILURE, or by having an assert() fail.
**/
const void *map_get( MapADT map, void *key ){
	//
	Entry* n = map->first;

	while (n != NULL){
		if (map->equals(n->key, key)){
			return n->value;
		}
		n = n->next;
	}
return NULL;
}

/**
Find the entry specified by the key and remove it from the MapADT,
returning the value pointer. 
@param map the MapADT to be manipulated
@return the value that was removed from the map
@exception If the map is empty, the program should terminate
with an error message.  This can be done by printing an
appropriate message to the standard error output and then
exiting with EXIT_FAILURE, or by having an assert() fail.
**/
void *map_delete( MapADT map, void *key ){
    //if map_contains = true; 
    if (map_empty(map)){
        exit(EXIT_FAILURE);
	return NULL;
    }
    if(!map_contains(map, key)){
        fprintf(stderr, "Key is not found in map");
        exit(EXIT_FAILURE);
	return NULL;
    }
    Entry* begin = map->first;
    while(begin!=NULL){
        if (map->equals(key, begin->key)){
            
            void* oldValue = begin->value;
            Entry* prevPointer = begin->prev;
            Entry* nextPointer = begin->next;
            
	    if(nextPointer == NULL && prevPointer == NULL){
		map->first = NULL;
		map->end = NULL;
  
	    }

	    if (nextPointer ==NULL){
                map->end = prevPointer;
                prevPointer->next = NULL;
            }else{
               nextPointer->prev = prevPointer;            }
            if (prevPointer ==NULL){
                map->first = nextPointer;
                nextPointer->prev = NULL;
                
            }
            else{
		prevPointer->next = nextPointer;
                
            }
            free(begin->key);
            free(begin);
	    map->size -=1;
            return oldValue;
            begin = begin->next;

    }
} 
    return NULL;
}


/**
Indicate whether or not the supplied Map is empty

@param the MapADT to be tested
@return true if the map is empty, otherwise false
**/
bool map_empty( MapADT map ){
	return map->size == 0;
}

/**
 * Supposed to do alphabetical sorting of the tables 
 * considering their name
 * @param takes MAPADT structure as input
 * @return void pointers
**/ 
void** alpha(MapADT map){
	void** arr = malloc(map->size*sizeof(void*));
	Entry* n = map->first;
	Entry* temp = n->next;
	Entry* storage;
	int counter = 0;
	while (n!=NULL){
		if(strcmp(n->key, temp->key)>0){
			storage->key = n->key;
			n->key = temp->key;
			temp->key = storage->key;
			arr[counter] = n->value;
			counter +=1;
			n = n->next;	
		}

	}
	return arr;

}

/**
 * This function retuurns the value of the entry of the map.
 * @param take the MapADT structure as an input 
 * @return void pointer
 **/
void** mapValue(MapADT map){
	void** arr = malloc(map->size*sizeof(void*));
	Entry* n = map->first;
	int counter = 0;
        while (n != NULL){
		arr[counter] = n->value;
		counter +=1;
		n = n->next;		
	}
	return arr;

}
