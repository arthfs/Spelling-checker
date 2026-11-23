#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct dict_node 
{
    char* word;
    char* lword;
} dnode;

extern dnode** dnodes;
extern int MAX_DEFINITIONS;
extern int numdef;

int validate( char* );
int require_case(char* );
int is_correct(char*);
void to_lower(char*);
int load_dict(char*);
int search(char*);

#endif