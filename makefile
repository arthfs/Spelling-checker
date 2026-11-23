CC = gcc
CFLAGS = -std=c99 -Wall -Werror -fsanitize=undefined,address

spell: speller.o lib.o search.o
	$(CC) $(CFLAGS) $^ -o $@

speller.o: speller.c lib.h search.h
	$(CC) $(CFLAGS) -c $< -o $@

lib.o: lib.c lib.h search.h
	$(CC) $(CFLAGS) -c $< -o $@

search.o: search.c search.h
	$(CC) $(CFLAGS) -c $< -o $@

test1: test_part1.o lib.o search.o
	$(CC) $(CFLAGS) $^ -o $@

test_part1.o: test_part1.c lib.h search.h
		$(CC) $(CFLAGS) -c $< -o $@

test3: test_part3.o lib.o search.o
	$(CC) $(CFLAGS) $^ -o $@

test_part3.o: test_part3.c lib.h search.h
	$(CC) $(CFLAGS) -c $< -o $@

test5: test_edges_cases.o lib.o search.o
	$(CC) $(CFLAGS) $^ -o $@

test_edges_cases.o: test_edges_cases.c lib.h search.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o spell test1 test3 test5  

.PHONY: clean