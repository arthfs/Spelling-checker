#include<stdio.h>
#include<string.h>
#include <fcntl.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdbool.h>
#include "lib.h"
#include<assert.h>

void read_simple_file()
{
    printf("    reading from a simple file\n");
    char** words = NULL;
    int counter = 0;
    int capacity = 2;
    words = calloc(capacity,sizeof(char*));
    if (words == NULL) 
    {
        fprintf(stderr,"failed allocation");
        return ;
    }

    int read = read_words("file3.txt",&words,&counter,&capacity);
   
   
    for (int h = 0;h <counter;h++) free(words[h]);
    free(words);
    assert(read == counter);
}

void read_multiples_files()
{
    printf("    reading from a multiple files\n");
    char* files[] = {"file1.txt","file2.txt","file3.txt"};
    char** words = NULL;
    int counter = 0;
    int capacity = 2;
    words = calloc(capacity,sizeof(char*));
    if (words == NULL) 
    {
        fprintf(stderr,"failed allocation");
        return ;
    }

    int read = 0;
    read+= (read_words("file3.txt",&words,&counter,&capacity) - read);
    read+= (read_words("file2.txt",&words,&counter,&capacity) -read);
    read+= (read_words("file1.txt",&words,&counter,&capacity) -read);

    
    assert(read == counter);
  
    for (int h = 0;h <counter;h++) free(words[h]);
    free(words);
}

void read_directory()
{
    printf("    reading from a multiple files\n");

    int counter = 0; 
    
    int read = 0;

    int* pointer = &counter;
    read = read_from_files("dir",".txt",&pointer);
    
    assert(*pointer == read);
    
}

int main()
{
    initialization("dictionarry.txt");
    printf("Test1 reading words from files and directories\n");
    read_simple_file();
    read_multiples_files();
    read_directory();
    printf("All test passed\n");
    
    cleanup();
    return EXIT_SUCCESS;
}