#include<stdio.h>
#include<string.h>
#include <fcntl.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
#include<stdbool.h>
#include"lib.h"
#include <errno.h>

#define INITIAL_MAX_WORDS 1000
char** words = NULL;
char** dict = NULL;

int current = 0, current2 = 0,extension_length = 10 , max_path_len = 4096,current_path_len = 0, MAX_WORDS = 40;;


node*  head = NULL;

void free_node(node* n) {
    if (n) {
        free(n->parent);
        free(n->name);
        free(n);
    }
}

void push( char* parent, char*name)
{
    node* new_node = malloc(sizeof(node));
    
    if (new_node == NULL) 
    {
        perror("not enough memory");
        return  ;
    }

    else 
    {   new_node->next = NULL;
        
        new_node->parent = malloc(strlen(parent) + 1);
        new_node->name = malloc(strlen(name) + 1);
        strcpy(new_node->name,name);
        strcpy(new_node->parent,parent);
        if (head == NULL) head = new_node;
        else 
        {
            new_node->next = head;
            head = new_node;
           
        }
       
    }
  
}

node* pop()
{
    if (head == NULL) return NULL;
    node* popped = head;
    head = head->next;
    return popped;
}


int read_words(char* path, char*** destination, int* start, int* capacity) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
       
        perror("open failed");
        return *start;
      
    }

    int len = 128;
    char buff[len];
    int bytes;
    int num_words = *start, cur_line = 1, column = 1;
    int in_word = 0, word_start_col = 1;
    char current_word[256];
    int word_len = 0;

    while ((bytes = read(fd, buff, len)) > 0) {
        for (int h = 0; h < bytes; h++) {
            char c = buff[h];
            
          
            if (!isspace(c)) {
                if (!in_word) {
                 
                    in_word = 1;
                    word_start_col = column;
                    word_len = 0;
                }
             
                if (word_len < sizeof(current_word) - 1) {
                    current_word[word_len++] = c;
                }
                column++;
            } else {
                if (in_word) {
                    // End of word - process it
                    current_word[word_len] = '\0';
                    
                    
                    char temp_word[256];
                    strcpy(temp_word, current_word);
                    
            
                    if (validate(temp_word) == 0 && strlen(temp_word) > 0) {

                       
                        if (num_words >= *capacity) {
                            *capacity *= 2;
                            *destination = realloc(*destination, *capacity * sizeof(char*));
                        }
                        
                        (*destination)[num_words] = malloc(strlen(temp_word) + 1);
                        if ((*destination)[num_words] != NULL) {
                            strcpy((*destination)[num_words], temp_word);
                            
                           
                            if (!is_correct((*destination)[num_words])) {
                                printf("%s %d:%d %s\n", path, cur_line, word_start_col, (*destination)[num_words]);
                            }
                            num_words++;
                        }
                    }
                    in_word = 0;
                    word_len = 0;
                }
                
            
                if (c == '\n') {
                    cur_line++;
                    column = 1;
                } else if (c == '\t') {
                    column = ((column + 7) / 8) * 8 + 1;
                } else {
                    column++;
                }
            }
        }
    }

   
    if (in_word) {
        current_word[word_len] = '\0';
        char temp_word[256];
        strcpy(temp_word, current_word);
        if (validate(temp_word) == 0 && strlen(temp_word) > 0) {
            if (num_words >= *capacity) {
                *capacity *= 2;
                *destination = realloc(*destination, *capacity * sizeof(char*));
            }
            
            (*destination)[num_words] = malloc(strlen(temp_word) + 1);
            if ((*destination)[num_words] != NULL) {
                strcpy((*destination)[num_words], temp_word);
                if (!is_correct((*destination)[num_words])) {
                    printf("%s %d:%d %s\n", path, cur_line, word_start_col, (*destination)[num_words]);
                }
                num_words++;
            }
        }
    }

    close(fd);
    *start = num_words;
    return num_words;
}


void initialization(char* dict_path)
{
   
    
    load_dict(dict_path);
    words = calloc(MAX_WORDS,sizeof(char*));
}

void free_array(char** array,int n)
{
    for (int h = 0;h <n;h++) free(array[h]);
}

void cleanup()
{
    free_array(words,current);
  
    for (int h =0;h<numdef;h++)
    {
        free(dnodes[h]->lword);
        free(dnodes[h]->word);
        free(dnodes[h]);
    }


}

int is_afile(const char* path) {
    struct stat statbuf;
    if (stat(path, &statbuf) == 0) {
        return S_ISREG(statbuf.st_mode);
    }
    return 0;
}

void read_from_input() {
    int fd = STDIN_FILENO;
    const int BUFF_SIZE = 1024;  
    char buff[BUFF_SIZE + 1];
    
    int max_words = INITIAL_MAX_WORDS;
    int current_word_count = 0;
    wnode **wnodes = calloc(max_words, sizeof(wnode*));
    if (wnodes == NULL) {
        perror("Failed to allocate word array");
        return;
    }

    int bytes_read;
    int current_line = 1, current_column = 1;
    int word_start_column = 1;
    
    char *current_word = malloc(BUFF_SIZE);
    int word_length = 0;
    
    if (current_word == NULL) {
        perror("Failed to allocate word buffer");
        free(wnodes);
        return;
    }

    int interupted = 0;
    while ((bytes_read = read(fd, buff, BUFF_SIZE)) > 0) 
    {
        if (interupted)
        {
            break;
        }
        buff[bytes_read] = '\0';
        
        for (int i = 0; i < bytes_read; i++) {
            if (interupted)
            {
                break;
            }
            char c = buff[i];
            
            // MODIFIED: Accept any non-whitespace character as part of a word
            // Let validate() handle the cleaning/validation later
            if (!isspace(c) && c != '\n') {
                if (word_length == 0) {
                    word_start_column = current_column;
                }
                
                current_word[word_length++] = c;
                current_column++;
            }
            // Word boundary found
            else if (word_length > 0) {
                current_word[word_length] = '\0';
                
                // NEW: Use validate() to clean the word
                char *word_to_validate = malloc(word_length + 1);
                if (word_to_validate != NULL) {
                    strcpy(word_to_validate, current_word);
                    
                    if (validate(word_to_validate) == 0 && strlen(word_to_validate) > 0) {
                        // Only store the word if validate() approves it
                        if (current_word_count >= max_words) {
                            max_words *= 2;
                            wnode **temp = realloc(wnodes, sizeof(wnode*) * max_words);
                            if (temp == NULL) {
                                perror("Failed to expand word array");
                                free(word_to_validate);
                                goto cleanup;
                            }
                            wnodes = temp;
                        }
                        
                        wnodes[current_word_count] = malloc(sizeof(wnode));
                        if (wnodes[current_word_count] == NULL) {
                            perror("Failed to allocate word node");
                            free(word_to_validate);
                            goto cleanup;
                        }
                        
                        wnodes[current_word_count]->word = malloc(strlen(word_to_validate) + 1);
                        wnodes[current_word_count]->path = NULL;
                        if (wnodes[current_word_count]->word == NULL) {
                            perror("Failed to allocate word string");
                            free(wnodes[current_word_count]);
                            free(word_to_validate);
                            goto cleanup;
                        }
                        
                        strcpy(wnodes[current_word_count]->word, word_to_validate);
                        wnodes[current_word_count]->line = current_line;
                        wnodes[current_word_count]->column = word_start_column;
                        
                        current_word_count++;
                    }
                    free(word_to_validate);
                }
                
                word_length = 0;
                
                if (c == '\n') {
                    current_line++;
                    current_column = 1;
                } else {
                    current_column++;
                }
            }
            else {
                if (c == '\n') {
                    current_line++;
                    current_column = 1;
                } else {
                    current_column++;
                }
            }
        }
        interupted = errno == EINTR;
    }
    
    // Handle the last word if file doesn't end with whitespace
    if (word_length > 0) {
        current_word[word_length] = '\0';
        
        // NEW: Use validate() for the last word too
        char *word_to_validate = malloc(word_length + 1);
        if (word_to_validate != NULL) {
            strcpy(word_to_validate, current_word);
            
            if (validate(word_to_validate) == 0 && strlen(word_to_validate) > 0) {
                if (current_word_count >= max_words) {
                    max_words += 1;
                    wnode **temp = realloc(wnodes, sizeof(wnode*) * max_words);
                    if (temp != NULL) {
                        wnodes = temp;
                    }
                }
                
                if (current_word_count < max_words) {
                    wnodes[current_word_count] = malloc(sizeof(wnode));
                    if (wnodes[current_word_count] != NULL) {
                        wnodes[current_word_count]->word = malloc(strlen(word_to_validate) + 1);
                        if (wnodes[current_word_count]->word != NULL) {
                            strcpy(wnodes[current_word_count]->word, word_to_validate);
                            wnodes[current_word_count]->line = current_line;
                            wnodes[current_word_count]->column = word_start_column;
                            current_word_count++;
                        } else {
                            free(wnodes[current_word_count]);
                        }
                    }
                }
            }
            free(word_to_validate);
        }
    }
    
    // Process all validated words through is_correct
    for (int i = 0; i < current_word_count; i++) {
        if (!is_correct(wnodes[i]->word)) 
        {
            printf("%s%d:%d: %s\n", wnodes[i]->path != NULL ? strcat(wnodes[i]->path,":") : "", 
                   wnodes[i]->line, wnodes[i]->column, wnodes[i]->word);
        }
    }

cleanup:
    free(current_word);
    for (int i = 0; i < current_word_count; i++) {
        if (wnodes[i] != NULL) {
            free(wnodes[i]->word);
            free(wnodes[i]);
        }
    }
    free(wnodes);
    
    if (bytes_read < 0) {
        perror("Error reading from input");
    }
}



int test_arguments(int n,char** args)
{
  int suffix_position = -1;
  for (int h = 0;h<n;h++)
 {
  
    if (h == 1 && (strcmp(args[h],"-s")==0)) 
    {
        suffix_position = h;
       
        break;
    }
    
    if (h>1 && strcmp(args[h],"-s")==0) 
    {   
        
        fprintf(stderr, "invalid suffix position\n");
        return EXIT_FAILURE;
        
       
    }
    
    
}



    int pos = -1;
    if (suffix_position>-1)
    {
       
        for (int h = 0;h<strlen(args[suffix_position]);h++)
        {
            if (args[suffix_position][h] == '.') 
            {   
                pos = h ;
            }

        }

      
        if (pos!=0) 
        { pos = -1;
        
        }
    }

   

 int dictionnary_index ; 
 if (suffix_position !=-1 && pos == 0) 
 {
     dictionnary_index = suffix_position+1;
  
 }

  if (suffix_position != -1 && pos == -1 )
 {
     dictionnary_index = 2;
     suffix_position = -99;
 }

 else if (suffix_position==-1 && pos==-1)
 {
     dictionnary_index = 1;
 }


 if (is_afile(args[dictionnary_index]) == 0)
 {  

    fprintf(stderr, "dictionary path is absent or is not a file\n");
    return EXIT_FAILURE;
 }

return suffix_position;

}


void get_extension(const char* source, char* result, size_t result_size) {
   
    const char* dot = strrchr(source, '.');
    
    if (dot == NULL) {
    
        result[0] = '\0';
        return;
    }
    

    const char* ext = dot ;
    size_t ext_len = strlen(ext);
    
 
    if (ext_len >= result_size) {
        ext_len = result_size - 1;
    }
    
    memcpy(result, ext, ext_len);
    result[ext_len] = '\0';  
}

int read_from_files(char* source, char* extension,int** n) {
    *n = &current;
    int total = 0;
    
    if (access(source, F_OK) < 0) {
        perror("invalid path");
        return EXIT_FAILURE;
    } else if (access(source, R_OK) < 0) {
        perror("insufficient permission");
        return -1;
    }

   
    if (is_afile(source)) {
        printf("Processing single file: %s\n", source);
        
        char file_ext[extension_length];
        get_extension(source, file_ext, extension_length);
        
        if (strcmp(file_ext, extension) == 0) {
            //printf("yes");
            int k = read_words(source, &words, &current, &MAX_WORDS);
            if (k>0) total+= (k-total);
            
           
            printf("Read %d words from %s\n\n", current, source);
        } else {
            printf("Skipping file - extension mismatch: %s\n", source);
        }
    } else {
        // It's a directory - traverse recursively
        printf("\n\nProcessing directory: %s\n", source);
        
        push(source, "");
        node* top = NULL;
        
        while ((top = pop()) != NULL) {
           
            char full_path[max_path_len];
            
            if (strlen(top->name) == 0) {
                
                strcpy(full_path, top->parent);
            } else if (top->parent[strlen(top->parent)-1] == '/') {
                snprintf(full_path, sizeof(full_path), "%s%s", top->parent, top->name);
            } else {
                snprintf(full_path, sizeof(full_path), "%s/%s", top->parent, top->name);
            }
            
            printf("\nProcessing: %s\n", full_path);
            
            if (!is_afile(full_path)) {
              
                DIR* directory = opendir(full_path);
                if (directory == NULL) {
                    perror("opendir failed");
                    free_node(top);  
                    continue;
                }
                
                struct dirent* dir_entry;
                while ((dir_entry = readdir(directory)) != NULL) {
                    if (strcmp(dir_entry->d_name, ".") != 0 && 
                        strcmp(dir_entry->d_name, "..") != 0) {
                        push(full_path, dir_entry->d_name);
                    }
                }
                closedir(directory);
            } else {
                // It's a file - check extension and process
                char file_ext[extension_length];
                get_extension(full_path, file_ext, extension_length);
                
                if (strcmp(file_ext, extension) == 0) {

                    int k = read_words(full_path, &words, &current, &MAX_WORDS);
                    if (k>0) total+= (k-total);
                   
                  
                    
                    //printf("Added %d words from %s\n", current - old_count, full_path);
                } else {
                    //printf("%s %s\n",file_ext,extension);
                    //printf("Skipping file (wrong extension): %s\n", full_path);
                }
            }
            
            free_node(top);  
        }
    }


return total;
}

