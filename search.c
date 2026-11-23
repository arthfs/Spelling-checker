#include "search.h"
#include <fcntl.h>      
#include <unistd.h>     
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define INIT_DEF 100

int numdef = 0;
dnode** dnodes = NULL;
int MAX_DEFINITIONS = INIT_DEF;  

int validate(char* target) {
    if (target == NULL || strlen(target) == 0) {
        return -1;
    }
    
    int n = strlen(target);
    int first_c = 0, end = n - 1;
    char* exclu = "`{}()'[]\"";
    
   
    while (first_c < n && strchr(exclu, target[first_c]) != NULL) {
        first_c++;
    }
    
   
    while (end >= first_c && ( !isdigit(target[end]) && !isalpha(target[end])) ) {
        end--;
    }
    
    
    if (first_c > end) {
        target[0] = '\0';
        return -1;
    }
    
   
    int new_len = end - first_c + 1;
    if (first_c > 0) {
        memmove(target, target + first_c, new_len);
    }
    target[new_len] = '\0';
    
    return 0;
}


int require_case(char* arr)
{
    for (int i = 0;arr[i];i++ ) if (isupper(arr[i])) return 1;
    return 0;
}


void to_lower(char* arr)
{
 for (int i = 0;arr[i];i++ ) arr[i] = tolower(arr[i]);
}

int compare(const void* first,const  void* second)
{
  const dnode* a =  *(dnode**) first;
  const dnode* b = *(dnode**) second;
  return strcmp(a->lword,b->lword);
}

int load_dict(char* path) {
    dnodes = calloc(INIT_DEF, sizeof(dnode*));
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open failed");
        return numdef;
    }

    int len = 128;
    char buff[len];
    int bytes;
    int num_words = numdef;
    int line_start = 0;
    int buffer_end = 0;  

 
    char temp_word[256];  
    
    while ((bytes = read(fd, buff + buffer_end, len - buffer_end)) > 0) {
        buffer_end += bytes;  
        
        int h = line_start;
        while (h < buffer_end) {
            if (buff[h] == '\n') {
                int word_len = h - line_start;
                
                if (word_len > 0 && word_len < 256) {
                   
                    memcpy(temp_word, buff + line_start, word_len);
                    temp_word[word_len] = '\0';
                    
                    
                    dnodes[num_words] = malloc(sizeof(dnode));
                    if (dnodes[num_words] == NULL) {
                        perror("dnode allocation failed");
                        close(fd);
                        return num_words;
                    }

                    dnodes[num_words]->word = malloc(word_len + 1);
                    dnodes[num_words]->lword = malloc(word_len + 1);
                    
                    if (dnodes[num_words]->word == NULL || dnodes[num_words]->lword == NULL) {
                        perror("string allocation failed");
                        close(fd);
                        return num_words;
                    }

                  
                    strcpy(dnodes[num_words]->word, temp_word);
                    strcpy(dnodes[num_words]->lword, temp_word);
                    
                  
                    to_lower(dnodes[num_words]->lword);


                    num_words++;

                
                    if (num_words >= MAX_DEFINITIONS) {
                        MAX_DEFINITIONS *= 2;
                        dnode** temp = realloc(dnodes, MAX_DEFINITIONS * sizeof(dnode*));
                        if (temp == NULL) {
                            perror("realloc failed");
                            close(fd);
                            return num_words;
                        }
                        dnodes = temp;
                    }
                }
                line_start = h + 1;  
            }
            h++;
        }

       
        if (line_start < buffer_end) {
            
            int leftover = buffer_end - line_start;
            memmove(buff, buff + line_start, leftover);
            buffer_end = leftover;
            line_start = 0;
        } else {
           
            buffer_end = 0;
            line_start = 0;
        }
    }

   
    if (buffer_end > 0) {
        int word_len = buffer_end;
        
       
        char temp_word[word_len + 1];
        memcpy(temp_word, buff, word_len);
        temp_word[word_len] = '\0';
        
        dnodes[num_words] = malloc(sizeof(dnode));
        if (dnodes[num_words] == NULL) {
            perror("dnode allocation failed");
            close(fd);
            return num_words;
        }

        dnodes[num_words]->word = malloc(word_len + 1);
        dnodes[num_words]->lword = malloc(word_len + 1);
        
        if (dnodes[num_words]->word == NULL || dnodes[num_words]->lword == NULL) {
            perror("string allocation failed");
            close(fd);
            return num_words;
        }

       
        strcpy(dnodes[num_words]->word, temp_word);
        strcpy(dnodes[num_words]->lword, temp_word);
        
        
        to_lower(dnodes[num_words]->lword);

         num_words++;
        
        if (num_words >= MAX_DEFINITIONS) {
            MAX_DEFINITIONS *= 2;
            dnode** temp = realloc(dnodes, MAX_DEFINITIONS * sizeof(dnode*));
            if (temp == NULL) {
                perror("realloc failed");
                close(fd);
                return num_words;
            }
            dnodes = temp;
        }
    }

    close(fd);
    numdef = num_words;

   
   
    qsort(dnodes, numdef, sizeof(dnode*), compare);
    
    return num_words;
}

int search(char* target)
{
//binary search

 char* temp = calloc(strlen(target)+1,sizeof(char));
 if (temp == NULL) 
 {
    perror("failed allocation");
    return -1;
 }

 strcpy(temp,target);

 to_lower(temp);

 int i = 0,j = numdef-1,m;
    while ( i<=j )
    {   m = (i+j)/2;
        //printf("%d",m);
        int c = strcmp(dnodes[m]->lword,temp);
        if ( c == 0) 
            {   int required = require_case(dnodes[m]->word);
                 if ( (strcmp(dnodes[m]->word,target)==0 && (required)) || (!required) )
                 {
                    free(temp);
                    return m;
                 }
                 
         
                 free(temp);
                 return -1;

                
            }
        else if (c<0) i = m+1;
        else if (c>0) j = m-1;

    }
    free(temp);
    //printf("not found");
    return -1;
}

int is_correct(char* target)
{
   
   

  return search(target)!=-1;
   

}