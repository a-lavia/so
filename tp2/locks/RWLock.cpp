#include "RWLock.h"

// PUEDEN LEER MUCHOS THREADS A LA VEZ
// SOLO PUEDE ESCRIBIR UNO A LA VEZ

// pthread_cond_signal -> sólo libera a uno del lock
// pthread_cond_broadcast -> libera a todos del lock


RWLock :: RWLock() {

	// Inicializamos el mutex y el cond
	pthread_mutex_init(&this->m, NULL);
	pthread_cond_init(&this->turn, NULL);

	// if (pthread_mutex_init(&this->m, NULL) != 0) {
	//	printf("\n mutex init failed\n");
	//	return 1;
	// }
}

void RWLock :: rlock() {
	pthread_mutex_lock(&this->m);

	// Puede haber alguien escribiendo, si lo hay que espere
	// hasta que termine
	/*
	 *	Si es un while podría haber siempre alguien que quiera
	 *	escribir y genera inanición. Mejor que pregunte una vez
	 *	y luego esperar a que alguno termine de escribir y leer
	 *	en ese momento.
	 */
	if(this->writers > 0)
		pthread_cond_wait(&this->turn, &this->m);

	// Espero que terminen de escribir para poder leer.
	while(this->writing > 0)
		pthread_cond_wait(&this->turn, &this->m);

	// Sección critica
	this->reading++;

	pthread_mutex_unlock(&this->m);
}

void RWLock :: wlock() {
	pthread_mutex_lock(&this->m);

	// Agrego un escritor más a la espera.
	this->writers++;

	// Espero mientras hay alguien escribiendo o la cantidad 
	// de lectores es mayor que cero
	while(this->writing > 0 || this->reading > 0)
		pthread_cond_wait(&this->turn, &this->m);

	// Sección critica - Aquí es el único que escribe
	this->writing++;

	pthread_mutex_unlock(&this->m);
}

void RWLock :: runlock() {
	pthread_mutex_lock(&this->m);

	this->reading--;

	// Si no hay mas threads leyendo hago un signal a todos
	if(this->reading == 0)
		pthread_cond_broadcast(&this->turn);

	pthread_mutex_unlock(&this->m);
}

void RWLock :: wunlock() {
	pthread_mutex_lock(&this->m);

	// Terminó la escritura, descuento la cantidad de escritores y 
	// seteo que nadie más está escribiendo
	// Luego hago un signal a todos
	this->writing = 0;
	this->writers--;

	pthread_cond_broadcast(&this->turn);

	pthread_mutex_unlock(&this->m);
}
