CC = gcc
CFLAGS = -l X11 -Wall
PROGS = dsgwm

all: 
	$(CC) $(CFLAGS) winman.c winman.h -o $(PROGS)

.PHONY: clean

clean:
	@echo "Cleaning..."
	@rm -f *.o $(PROGS) 
