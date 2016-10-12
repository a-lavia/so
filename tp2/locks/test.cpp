#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "RWLock.h"

#define NUM_THREADS 	100

using namespace std;

RWLock arr_critico_lock;
RWLock readers_lock;
int arr_critico[NUM_THREADS];
int cant_thread_readers = 0;

void* thread_reader(void* soy){
	arr_critico_lock.wlock();
	cout << "Soy thread reader num " << (long) soy << " | Leo arr_critico: "<< arr_critico[(long) soy] << endl;
	arr_critico_lock.wunlock();

	readers_lock.rlock();
	cant_thread_readers++;
	readers_lock.runlock();

	pthread_exit(NULL);
}

void* thread_writer(void* soy){
	arr_critico_lock.wlock();
	cout << "Soy thread writer num " << (long) soy << endl;
	arr_critico[(long) soy] = (long) soy;
	arr_critico_lock.wunlock();

	pthread_exit(NULL);
}


int main(){
	int j;
	bool hay_error;
	long i;

	// Inicializo el arreglo que esta en la sección critica
	for(i = 0; i < NUM_THREADS; i++)
		arr_critico[i] = 0;

	cout << endl;

	// Genero mi arreglo de pthreads, luego lanzo mitad como 
	// lectores y mitad como escritores
	pthread_t threads[NUM_THREADS];
	for(i = 0; i < NUM_THREADS; i++){
		if(i % 2 == 0)
			pthread_create(&threads[i], NULL, thread_reader, (void*) i);
		else
			pthread_create(&threads[i], NULL, thread_writer, (void*) i);
	}


	// pthread_join espera hasta que termina el thread de
	// ejecutarse.
	// Espero hasta que terminen todos los threads para verificar
	// si termino todo bien.
	for(i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);


	// Verifico si los writers escribieron bien,
	j = 0;
	hay_error = false;
	while(j < NUM_THREADS && !hay_error){
		if((j % 2 == 0 && arr_critico[j] != 0) || (j % 2 == 1 && arr_critico[j] != j)){
			cout << "Error en escritura, posición numero: " << j << ", valor escrito: " << arr_critico[j] << endl;
			hay_error = true;
		}
		j++;
	}

	if(hay_error){
		cout << "Hubo errores en escritura." << endl;
		pthread_exit(NULL);
	}

	if(cant_thread_readers != NUM_THREADS/2){
		cout << "Hubo errores en lectura. No leyeron todos." << endl;
		pthread_exit(NULL);
	}

	cout << "No hubo errores. Termina el test." << endl;
	pthread_exit(NULL);
}

