MAC_CC = /usr/local/Cellar/gcc/8.2.0/bin/gcc-8

radix_sort: src/main.c src/serial_radix_sort.c src/parallel_radix_sort.c src/helpers.c
	gcc -g -Wall -o radix_sort src/main.c src/serial_radix_sort.c src/parallel_radix_sort.c src/helpers.c -fopenmp

mac: src/main.c src/serial_radix_sort.c src/parallel_radix_sort.c src/helpers.c
	$(MAC_CC) -g -Wall -o radix_sort src/main.c src/serial_radix_sort.c src/parallel_radix_sort.c src/helpers.c -fopenmp