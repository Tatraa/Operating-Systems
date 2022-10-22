//Kacper Tatrocki 10.05.2022, Krakow
#ifndef SEMAFORY_H
#define SEMAFORY_H
//wlasna biblioteka funkcji do tworzenia, otwierania, uzyskiwania wartosci, operowania, zamykania i usuwania semafora
sem_t *stworz_semafor(const char *name);
sem_t *otworz_semafor(const char *name);
void zamknij_semafor(sem_t *sem_t);
void wartosc_semafor(sem_t *sem, int *value);
void podnies_semafor(sem_t *sem);
void opusc_semafor(sem_t *sem);
void usun_semafor(const char *name);

#endif