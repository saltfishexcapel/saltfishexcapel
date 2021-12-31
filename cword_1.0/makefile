OBJS = cword_main.o detect.o filename_detect.o fix.o help.o show.o write.o

CFLAGS := -Wall -g

HOME := $(shell cd ~ ; pwd)
export HOME

main: $(OBJS)
	gcc $(OBJS) -o cword_temp.o
cwordtest.o: cwordtest.c
	gcc -c $(CFLAGS) cword_main.c -o cword_main.o
detect.o: detect.c
	gcc -c $(CFLAGS) detect.c -o detect.o
filename_detect.o: filename_detect.c
	gcc -c $(CFLAGS) filename_detect.c -o filename_detect.o
fix.o: fix.c
	gcc -c $(CFLAGS) fix.c -o fix.o
help.o: help.c
	gcc -c $(CFLAGS) help.c -o help.o
show.o: show.c
	gcc -c $(CFLAGS) show.c -o show.o
write.o: write.c cword.h
	gcc -c $(CFLAGS) write.c -o write.o
install:
	cp cword_temp.o /usr/bin/cword
clean:
	rm *.o
purge:
	sudo rm /usr/bin/cword
