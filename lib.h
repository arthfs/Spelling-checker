#ifndef LIB_H
#define LIB_H
#include "search.h"
 



typedef struct dirnode
{
    char* parent;
    char* name;
    struct dirnode* next;
} node; 

typedef struct word_node 
{
    char* path;
    char* word;
    int line;
    int column;

}wnode;

void initialization(char *);
void free_array(char** ,int );
void cleanup();
int is_afile(const char* ) ;
int test_arguments(int ,char** );
void free_node(node* ) ;
void push( char* , char*);
node* pop();
int read_words(char* ,char*** ,int*,int*);



int read_from_files(char* ,char*,int**) ;

void read_from_input();
#endif