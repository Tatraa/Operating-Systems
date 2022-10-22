#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "kolejki.h"

mqd_t kolejka_utworz(const char *name, int flag) {								//utworz kolejke
	struct mq_attr attr;
	attr.mq_maxmsg=MAX_WIADOMOSCI;
	attr.mq_msgsize=DLUGOSC_KOMUNIKATU;
	mqd_t deskryptor=mq_open(name, O_CREAT | O_EXCL | flag, 0644, &attr);
	if(deskryptor==-1) {
		perror("blad tworzenia kolejki");
		exit(EXIT_FAILURE);
	}
	return deskryptor;
}

mqd_t kolejka_otworz(const char *name, int flag) {								//otworz istniejaca kolejke
	mqd_t deskryptor=mq_open(name, flag);
	if(deskryptor==-1) {
		perror("blad otwierania kolejki");
		exit(EXIT_FAILURE);
	}
	return deskryptor;
}

void kolejka_wyslij(mqd_t mq_des, const char *msg_ptr, unsigned int msg_prio) {	//wyslij wiadomosc do kolejki z priorytetem
	if(mq_send(mq_des, msg_ptr, DLUGOSC_KOMUNIKATU, msg_prio)==-1) {	//msg_ptr wskazuje na bufor w ktorym komunikat jest umieszczony
		perror("blad wysylania komunikatu");							//msg_prio okresla priorytet komunikatu
		exit(EXIT_FAILURE);
	}
}

void kolejka_odbierz(mqd_t mq_des, char *msg_ptr, unsigned int *msg_prio) {		//odbierz wiadomosc z danej kolejki
	if(mq_receive(mq_des, msg_ptr, DLUGOSC_KOMUNIKATU, msg_prio) == -1) {
		perror("blad odbierania komunikatu");
		exit(EXIT_FAILURE);
	}
}

void kolejka_zamknij(mqd_t mq_des) {											//zamknij nieuzywana kolejke
	if(mq_close(mq_des)==-1) {
		perror("blad zamykania kolejki");
		exit(EXIT_FAILURE);
	}
}

void kolejka_usun(const char *name) {											//usun kolejke o danej nazwie
	if(mq_unlink(name)==-1) {
		perror("blad usuwania kolejki");
		exit(EXIT_FAILURE);
	}
}

struct mq_attr kolejka_pobierz_atrybuty(mqd_t mq_des) {							//pobierz atrybuty kolejki
	struct mq_attr atrybuty;
	if(mq_getattr(mq_des, &atrybuty)==-1) {
		perror("blad pobierania atrybutow kolejki");
		exit(EXIT_FAILURE);
	}
	return atrybuty;
}