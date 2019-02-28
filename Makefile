CC = /usr/local/Cellar/gcc/8.2.0/bin/gcc-8

radix_sort: src/main.c src/parallel_helpers.c src/helpers.c
	$(CC) -g -Wall -o radix_sort src/main.c src/parallel_helpers.c src/helpers.c -fopenmp