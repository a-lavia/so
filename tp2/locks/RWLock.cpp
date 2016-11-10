#include "RWLock.h"

// PUEDEN LEER MUCHOS THREADS A LA VEZ
// SOLO PUEDE ESCRIBIR UNO A LA VEZ

// pthread_cond_signal -> sólo libera a uno del lock
// pthread_cond_broadcast -> libera a todos del lock


RWLock :: RWLock() {

	// Inicializamos el mutex y el cond
	pthread_mutex_init(&this->mtx, NULL);
	pthread_mutex_init(&this->roomEmpty, NULL);
	pthread_cond_init(&this->turn, NULL);

	this->readers = 0;
}


void RWLock :: rlock() {
	pthread_mutex_lock(&this->roomEmpty);
	pthread_mutex_lock(&this->mtx);

	// Aquí solo va a haber lectores
	// Cuando entro tengo el mutex roomEmpty y lo libero así puede entrar cualquiera.

	this->readers++;

	pthread_mutex_unlock(&this->mtx);
	pthread_mutex_unlock(&this->roomEmpty);
}


void RWLock :: wlock() {
	pthread_mutex_lock(&this->roomEmpty);
	pthread_mutex_lock(&this->mtx);

	// Si hay lectores espero que todos terminen de leer.
	// pthread_cond_wait libera el mutex mtx a todos menos al que está esperando la condicion.
	// Como el mutex roomEmpty lo sigue teniendo, espera a que terminen todos los lectores.
	// Esto garantiza exclusividad del escritor.

	while(this->readers > 0){
		pthread_cond_wait(&this->turn, &this->mtx);
	}

	pthread_mutex_unlock(&this->mtx);
}


void RWLock :: runlock() {
	pthread_mutex_lock(&this->mtx);

	this->readers--;

	// Descuento la cantidad de lectores y cuando llega a cero despierto al escritor, si hay, que esperaba
	// En este lugar pasan tres cosas: la seccion critica solo tiene lectores, o no hay nadie en ella, o hay un escritor.
	// Si la seccion critica tiene solo lectores o no hay nadie -> el mutex roomEmpty no lo tiene nadie y puede entrar cualquiera
	// Si hay un escritor -> el mutex roomEmpty lo tiene el escritor y hasta que no terminen de leer y, el mismo de escribir, no lo devuelve.
	// Cuando lo devuelve puede entrar cualquiera.

	if(this->readers == 0){
		pthread_cond_broadcast(&this->turn);
	}

	pthread_mutex_unlock(&this->mtx);
}


void RWLock :: wunlock() {
	// Libero el mutex roomEmpty y puede entrar cualquiera.
	pthread_mutex_unlock(&this->roomEmpty);
}
