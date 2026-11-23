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


void all_present()
{   
    int correct_words = 0;
    printf("    running all_present (in dict)");
    char* words[] = {"arthur","Opaul","pierre" ,"john","louis","marc","damis","cademi"};
    int n = sizeof(words)/sizeof(char*);
    printf("\n");
    for (int h =0;h<n;h++) 
        {  int c = is_correct(words[h]);
            if (c) correct_words++;
           
        }

    assert(correct_words == n);
}

void all_missing()
{
    int correct_words = 0;
    printf("    running all_missing ( not in dict)");
    char* words[] = {"dad","mom","cousin" ,"sister","louisiana","missing","here","brother"};
    int n = sizeof(words)/sizeof(char*);
    printf("\n");
    for (int h =0;h<n;h++) 
        {  int c = is_correct(words[h]);
            if (c) correct_words++;
           
        }

    assert(correct_words == 0);
}

int main()
{
    initialization("dictionarry.txt");
    printf("Test3 (if a word is in the dictionarry)\n");
    all_present();
    all_missing();
    printf("All test passed\n");
    
    cleanup();
    return EXIT_SUCCESS;

}