#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

pthread_t utworz_watek(void *funkcja(void *), void *argument) {				//utworzenie nowego watku, ktory wykona fukncje i przyjmie argument
	pthread_t id_watku;
	int status=pthread_create(&id_watku, NULL, funkcja, argument);
	if (status==EAGAIN) {
		printf("Blad przy tworzeniu watku!\n");
		printf("Przekroczony zostal limit systemowy liczby watkow lub procesow LWP\n");
		exit(EXIT_FAILURE);
	} else if (status==ENOMEM) {
		printf("Blad przy tworzeniu watku!\n");
		printf("Brak pamieci do utworzenia nowego watku\n");
		exit(EXIT_FAILURE);
	} else if (status==EINVAL) {
		printf("Blad przy tworzeniu watku!\n");
		printf("Nieprawidlowa wartosc argumentu attr");
		exit(EXIT_FAILURE);
	} else if (status!=0) {
		printf("Nieznany blad przy tworzeniu watku!\n");
		exit(EXIT_FAILURE);
	}
	return id_watku;
}

void polacz_watek(pthread_t watek) {										//czekanie na zakonczenie watku i polaczenie watkow
	if(pthread_join(watek, NULL)!=0) {
		printf("Blad laczenia watku!\n");
		exit(EXIT_FAILURE);	
	}
}

void inicjalizuj_mutex(pthread_mutex_t *mutex) {							//inicjalizuj mutex
	if(pthread_mutex_init(mutex, NULL)!=0) {
		printf("Blad przy inicjalizacji mutexu!\n");
		exit(EXIT_FAILURE);
	}
}

void zamknij_mutex(pthread_mutex_t *mutex) {								//zamknij mutex
	if(pthread_mutex_lock(mutex)!=0) {
		printf("Blad zamykania mutexu!\n");
		exit(EXIT_FAILURE);	
	}
}

void otworz_mutex(pthread_mutex_t *mutex) {									//otworz mutex
	if(pthread_mutex_unlock(mutex)!=0) {
		printf("Blad otwierania mutexu!\n");
		exit(EXIT_FAILURE);
	}
}

void usun_mutex(pthread_mutex_t *mutex) {									//usun mutex
	if(pthread_mutex_destroy(mutex)!=0) {
		printf("Blad usuwania mutexu!\n");
		exit(EXIT_FAILURE);
	}
}