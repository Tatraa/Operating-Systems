#Kacper Tatrocki 10.05.2022, Krakow

DIR = `basename $(CURDIR)`

NAME1 = powielacz
NAME2 = dodawacz
NAME3 = semafory
ILOSC_PROCESOW = 3
ILOSC_SEKCJI = 7
PLIK_WE_WY = numer.txt
SEMAFOR = /sem_powielacz
SYNCHRONIZUJ = 1

#####################################

EXEC1  = $(NAME1).x
EXEC2  = $(NAME2).x
OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o
EXEC3  = $(NAME3).c

####################################

CFLAGS = -pthread -Wall -std=c99 -pedantic -O
LFLAGS = -pthread -Wall -std=c99 -pedantic -O

####################################

CO = gcc
LD = $(CO)

AR = ar

ARFLAGS = rsv

####################################

%.o: %.c
	$(CO) $(CFLAGS) -c $<

####################################

.PHONY: all
all: $(EXEC1) $(EXEC2)

$(EXEC1): $(OBJS1) $(EXEC3) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXEC2): $(OBJS2) $(EXEC3) 
	$(LD) -o $@ $(LFLAGS) $^

####################################

.PHONY: runSemafor

runSemafor: $(EXEC1) $(EXEC2)
	clear
	./$(EXEC1) $(EXEC2) $(ILOSC_PROCESOW) $(ILOSC_SEKCJI) $(PLIK_WE_WY) $(SEMAFOR) $(SYNCHRONIZUJ)

.PHONY: clean tar

clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
