Name  : Marc-Arthur  Saint Louis
NetID : MS4050




** My Approach

- Before everything I ensured each possible combinations of arguments lead to the desired outcome.
if the user doesn't provide the dictionary path. The program stop after displaying the reason. If the user doesn't provide -s or
he provided -s without the suffix . ".txt" is used by default. iF everything if fine , i iterates through the arguments and 
call a function readfile . It will read and process words from files and directories.


-if the user provided files/directories path . I used the function read_from_files (to traverse recursively the files). 
 If it's a simple file I used the function read words otherwise I used the function read_from_input

- to verify the words 
  I used two approaches. If the user entered via STDIN, I store everything in an array. I iterated through 
  everything and reported the misspelled words.

  If the user provided files/directories path , I reported the misspelled words while I am going though the words


                                Test plan 

as mentioned in the project description we need to do these:
 
 "We can break spell into three major components:
1. Finding and opening all the specified files, including directory traversal
2. Reading the file and generating a sequence of position-annotated words
3. Checking whether a word is contained in the dictionary"

** for part 1 , I used the file test_part1.c to test it. This file has 3 methods.The first one reads 
a simple file. In the second methods, I read 3 files. For these methods, I stored these words in array (Initialized with 2 words).
at the end I compared the number of words I read versus the current size of the array. The numbers matched.
For the third method , I navigated though a directory and processed every files. I didn't interract with the words 
(because the array that has them is somewhere else), I used pointers to keep track of the size of the array .
because read_from_files() return the words read. I was able to compare the total numbers of word read and the size of the array 
(that contains the words);


** I didn't create specific test for part2 but , each time I processed a file . I analyzed the 
results to see if the line and column matched.

** for part 3 , I used the file test_part3.c. This file has 2 method. The first one,is called all_present.
I created an array that contains the same words in my dictionarry. I used my function is_correct (this function will 
search for the word and verify if it's spelled correctly) , to keep track of the correct_words .
At the end I compared the values of correct_words and the size of the array. The numbers matched.
The second function is called all_missing , I created an array of 8 words that are not in the dictionarry. 
I used the same process as in the first function but at the entered I verified that correct_words was 0

** I created a file called test_edges_cases ,it has two methods. The first one is called read_valid_words_with_character.
I used the file validalphanumsymbols.txt, this file has words that has digits,punctuation and letters.But they should be accepted.
I used a similar to the one I used to test part3 to compare the number of correct_words matched 
the number of words in the file. In the second method, called read_invalid_words_with_character. I used the file invalidalphanumsymbol.txt .
This file contains words that has has digits,punctuation and letters. They're consist of the same letters as 
the ones in the dictionarry but . They have different capitalization and other non alphabetic characters. 
At the end, I verified correct_words = 0 

**I also cused a method called read_big file. I made the program process this file (made up 189 words).Some of them .
They are different kind of words (has digits ,uppercase,lowercase , symbols). The program didn't run out of memory and processed every word 
successfully.


**helpful notes

If the user is entering words by STDIN they can stop by pressing control z in an empty line (if using linux)
or control d if using windows.

After the run the program, if nothing appeared in terminal , it means all the words were correct,otherwise 
line number ,column number, name of the file of the incorrect words and the world itself will be reported.

lib.h and lib.c are related to storing the words
search.h and search are related fo finding the misspelled/absent words

dictionary name : dictionarry.txt, dict2.txt
test files : file1.txt ,file2.txt,file3.txt
test directory (it also contains other directories and files ) : dir

I created a struct called dnodes (to hold the lowercase and normal version of a word in the dictionary), 
a struct called word_node ( to hold the word, it's column and it's line ) and dir_node (to hold the path,parent of a directory,a next pointer )

to traverse the directories recursively, I used a stack of dir_nodes, I processed it until it's empty.
to find a word . I had all the words gathered stored in an array of word_node. I sorted it by increasing order,
based on the lowercase version of the word. I used binary search to search for the lowercase version of the target.
If i find it and the original word in the dictionary is in lowercase no error is reported ,same thing if 
capitalization is required and the target has the same capitalization as the original word in the dictionary.
Otherwise , I returned -1. From there , I analyzed the result reported the lines and column the misspelled words was found.
If the user entered the words by STDIN , I reported the misspelled words their line and column . 
if not , I also printed the location of the file these misspelled words came from.

to run the first test do (related to the first part in the test plan) :
                        make test1 
                        ./test1

to run the second test (related to the second part in the test plan) do :
                        make test3 
                        ./test3


to run the second test (edges cases) do :
                        make test5 
                        ./test5


