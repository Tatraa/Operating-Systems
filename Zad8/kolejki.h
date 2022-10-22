#ifndef KOLEJKI_H
#define KOLEJKI_H

#define KOLEJKA_NAZWA "/kolejka_komunikatow"
#define MAX_WIADOMOSCI 5
#define DLUGOSC_KOMUNIKATU 20

mqd_t kolejka_utworz(const char *name, int flag);								//utworz kolejke

mqd_t kolejka_otworz(const char *name, int flag);								//otworz istniejaca kolejke

void kolejka_wyslij(mqd_t mq_des, const char *msg_ptr, unsigned int msg_prio);	//wyslij wiadomosc do kolejki z priorytetem

void kolejka_odbierz(mqd_t mq_des, char *msg_ptr, unsigned int *msg_prio);		//odbierz wiadomosc z danej kolejki

void kolejka_zamknij(mqd_t mq_des);												//zamknij nieuzywana kolejke
	
void kolejka_usun(const char *name);											//usun kolejke o danej nazwie

struct mq_attr kolejka_pobierz_atrybuty(mqd_t mq_des);							//pobierz atrybuty kolejki

#endif