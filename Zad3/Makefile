#Kacper Tatrocki 				Krakow, 29.03.22r 
#==========================================================================
#
# make: all, clean, tar  
#
#=========================================================================
DIR = $(notdir $(CURDIR))
COFLAGS = -Wall -O -pedantic -std=c99 -D_POSIX_SOURCE
CO = gcc
#------------------------

#----------------------
#############################

prog_a.x: a.c 
	$(CO) $(COFLAGS) -o $@ $^

prog_b.x: b.c 
	$(CO) $(COFLAGS) -o $@ $^

prog_c.x: c.c zgrupowanie.x prog_a.x
	$(CO) $(COFLAGS) -o $@ c.c

zgrupowanie.x: zgrupowanie_kadry.c
	$(CO) $(COFLAGS) -o $@ $^


#############################
ACTION =
NR =

.PHONY: all runA runB runC

all: prog_a.x prog_b.x prog_c.x

runA: prog_a.x
	./prog_a.x $(ACTION) $(NR) 

runB: prog_b.x
	./prog_b.x $(ACTION) $(NR) 

runC: prog_c.x
	./prog_c.x $(ACTION) $(NR) 

#############################
.PHONY: clean tar
# Porzadki

clean:                                                     
	rm -f *.o  *~ *.x
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )