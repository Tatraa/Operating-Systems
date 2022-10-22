#ifndef WATKI_H
#define WATKI_H

#include <pthread.h>

pthread_t utworz_watek(void *funkcja(void *), void *argument);                 //utworzenie nowego watku, ktory wykona fukncje i przyjmie argument

void polacz_watek(pthread_t watek);                                            //czekanie na zakonczenie watku i polaczenie watkow

void inicjalizuj_mutex(pthread_mutex_t *mutex);                                //inicjalizuj mutex
       
void zamknij_mutex(pthread_mutex_t *mutex);                                    //zamknij mutex

void otworz_mutex(pthread_mutex_t *mutex);                                     //otworz mutex

void usun_mutex(pthread_mutex_t *mutex);                                       //usun mutex

#endif