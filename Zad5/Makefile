#Kacper Tatrocki 23.04.2002, Krakow
DIR = `basename $(CURDIR)`
########################################

NAME1 = a
PROD = producent
KONS = konsument
PLIK_WE = in
PLIK_WY = out
POTOK = FIFO

########################################

EXEC1  = $(NAME1).x
EXECP  = $(PROD).x
EXECK  = $(KONS).x

OBJS1  = $(NAME1).o
OBJSP  = $(PROD).o
OBJSK  = $(KONS).o

########################################

CFLAGS = -Wall -std=c99 -pedantic -O
LFLAGS = -Wall -std=c99 -pedantic -O

########################################

CO = gcc
LD = $(CO)

AR = ar

ARFLAGS = rsv

#######################################

%.o: %.c
	$(CO) $(CFLAGS) -c $<

.PHONY: all
all: $(EXEC1) $(EXECP) $(EXECK)

$(EXEC1): $(OBJS1) $(LIBS1) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXECP): $(OBJSP) $(LIBSP) 
	$(LD) -o $@ $(LFLAGS) $^

$(EXECK): $(OBJSK) $(LIBSK) 
	$(LD) -o $@ $(LFLAGS) $^

########################################

.PHONY: runA runB

runA: $(EXEC1) $(EXECP) $(EXECK)
	clear
	./$(EXEC1) $(EXECP) $(EXECK) $(PLIK_WE) $(PLIK_WY) $(POTOK)
	@echo "\n=============================\n"
	@diff -s $(PLIK_WE) $(PLIK_WY)

runB: $(EXECP) $(EXECK)
	clear
	mkfifo -m 0644 $(POTOK)
	xterm -hold -title $(PROD) -e ./$(EXECP) $(PLIK_WE) $(POTOK) & xterm -hold -title $(KONS) -e ./$(EXECK) $(PLIK_WY) $(POTOK)
	rm $(POTOK)
	@echo "\n=============================\n"
	@diff -s $(PLIK_WE) $(PLIK_WY)

.PHONY: clean tar

#######################################

clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
