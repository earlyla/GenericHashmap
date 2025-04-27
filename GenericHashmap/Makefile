CC = gcc
CFLAGS += -Wall -std=c99 -g
LDLIBS +=

driver: driver.o value.o map.o input.o

driver.o: driver.c

# map: map.o value.o input.o

map.o: map.c map.h value.o

value.o: value.c value.h

input.o: input.c input.h

clean:
	rm -f *.o
	rm -f *.g*
	rm -f driver
	rm -f map
	rm -f value
