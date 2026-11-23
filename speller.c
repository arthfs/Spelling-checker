#include<stdio.h>
#include<string.h>
#include <fcntl.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdbool.h>
#include "lib.h"









int main(int argc, char** argv)
{    int suffix_position =  test_arguments(argc,argv);
    
     int files_indexes;
    
     char* dict_path;
     if (suffix_position>-1)
     {
   
      dict_path = suffix_position == -1 ? argv[1] : argv[3];
     }

     else 
     {
        dict_path = argv[1];
     }
     
     
   
     initialization(dict_path);
   
    
    
    

   
    if (suffix_position == -99) suffix_position =1;
    else if (suffix_position!=-1) suffix_position++;
  
    if ( (argc ==2 && suffix_position ==-1) || (suffix_position!=-1 && argc-1 == suffix_position+1   ) )
    {
        printf("Press control d in an empty line stop entering words (if using windows control z) \n");
        read_from_input();
      
        
       
    }

    else if (argc>2)
    {
       
        files_indexes = suffix_position>1? 4:2;
        char* extension = suffix_position>1? argv[suffix_position]:".txt";

        
        int num_words = 0;
        int* ptr = &num_words;
        for (int h= files_indexes;h<argc;h++)
        {
           if ( read_from_files(argv[h],extension,&ptr) == EXIT_FAILURE) return EXIT_FAILURE;
            
        }
        
       
        
    }
    
   
    cleanup();
    return EXIT_SUCCESS;
}