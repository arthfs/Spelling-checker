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

void read_big_file()
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

    int read = read_words("big_test.txt",&words,&counter,&capacity);
   
   
    for (int h = 0;h <counter;h++) free(words[h]);
    free(words);
    assert(read == counter);
}

void read_valid_words_with_character()
{
    printf("    reading valid words (that have punctuation,digits...) \n");
    char** words = NULL;
    int counter = 0;
    int capacity = 2;
    words = calloc(capacity,sizeof(char*));
    if (words == NULL) 
    {
        fprintf(stderr,"failed allocation");
        return ;
    }

    read_words("validalphanumsymbols.txt",&words,&counter,&capacity);
    int correct_words = 0;
    for (int h =0;h<counter;h++) 
        {  int c = is_correct(words[h]);
            if (c) correct_words++;
           
        }

    assert(correct_words == counter);
   
   
    for (int h = 0;h <counter;h++) free(words[h]);
    free(words);
}

void read_invalid_words_with_character()
{
    printf("    reading invalid words (that have punctuation,digits...) \n");
     char** words = NULL;
    int counter = 0;
    int capacity = 2;
    words = calloc(capacity,sizeof(char*));
    if (words == NULL) 
    {
        fprintf(stderr,"failed allocation");
        return ;
    }

     read_words("invalidalphanumsymbols.txt",&words,&counter,&capacity);
    int correct_words = 0;
    for (int h =0;h<counter;h++) 
        {  int c = is_correct(words[h]);
            if (c) correct_words++;
           
        }

    
    assert(correct_words == 0);
   
   
    for (int h = 0;h <counter;h++) free(words[h]);
    free(words);
}

int main()
{
    initialization("dictionarry.txt");
    printf("Test5 edges cases\n");
    read_valid_words_with_character();
    read_invalid_words_with_character();
    read_big_file();
    printf("All test passed\n");
    
    cleanup();
    return EXIT_SUCCESS;
}