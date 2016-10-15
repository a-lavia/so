#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "RWLock.h"

#define CANT_THREADS 	10000

using namespace std;


// Variables Globales Test 1
RWLock lock_writers_test_1;
RWLock lock_readers_test_1;
int array_test_1[CANT_THREADS];
int readers_test_1 = 0;
int writers_test_1 = 0;

// Variables Globales Test 2
RWLock lock_writers_test_2;
int writers_test_2 = 0;
int writers_hasta_reader_test_2[CANT_THREADS];

// Variables Globales Test 3
RWLock lock_writers_test_3;
int readers_test_3 = 0;
int readers_hasta_writer_test_3[CANT_THREADS];



// Funciones Test 1
void* thread_reader_test_1(void* soy){
	lock_writers_test_1.rlock();
	array_test_1[(long) soy] = writers_test_1;
	lock_writers_test_1.runlock();

	lock_readers_test_1.wlock();
	readers_test_1++;
	lock_readers_test_1.wunlock();

	pthread_exit(NULL);
}

void* thread_writer_test_1(void* soy){
	lock_writers_test_1.wlock();
	writers_test_1++;
	lock_writers_test_1.wunlock();

	lock_readers_test_1.rlock();
	array_test_1[(long) soy] = readers_test_1;
	lock_readers_test_1.runlock();

	pthread_exit(NULL);
}

// Funciones Test 2
void* thread_reader_test_2(void* soy){
	lock_writers_test_2.rlock();
	writers_hasta_reader_test_2[(long) soy] = writers_test_2;
	lock_writers_test_2.runlock();

	pthread_exit(NULL);
}

void* thread_writer_test_2(void* soy){
	lock_writers_test_2.wlock();
	writers_test_2++;
	lock_writers_test_2.wunlock();

	pthread_exit(NULL);
}

// Funciones Test 3
void* thread_reader_test_3(void* soy){
	lock_writers_test_3.rlock();
	readers_test_3++;
	lock_writers_test_3.runlock();

	pthread_exit(NULL);
}

void* thread_writer_test_3(void* soy){
	lock_writers_test_3.wlock();
	readers_hasta_writer_test_3[(long) soy] = readers_test_3;
	lock_writers_test_3.wunlock();

	pthread_exit(NULL);
}

// Test 1 - Mitad Escritores Y Mitad Lectores
int testMitadEscritoresMitadLectores(){
	// int j;
	// bool hay_error;
	long i;

	// Inicializo el arreglo que esta en la sección critica
	for(i = 0; i < CANT_THREADS; i++)
		array_test_1[i] = 0;

	// Genero mi arreglo de pthreads, luego lanzo mitad como 
	// lectores y mitad como escritores
	pthread_t threads[CANT_THREADS];
	for(i = 0; i < CANT_THREADS; i++){
		if(i % 2 == 0)
			pthread_create(&threads[i], NULL, thread_reader_test_1, (void*) i);
		else
			pthread_create(&threads[i], NULL, thread_writer_test_1, (void*) i);
	}


	// pthread_join espera hasta que termina el thread de
	// ejecutarse.
	// Espero hasta que terminen todos los threads para verificar
	// si termino todo bien.
	for(i = 0; i < CANT_THREADS; i++)
		pthread_join(threads[i], NULL);

	// En los i pares se encuentra la cantidad de threads escritores hasta el momento que el
	// thread lector leyo, y viceversa para los i impares.
	// Consideramos que si el rango i-15 <= array_test_1[i] <= i+15 es falso hay deadlock. Siendo i el nro de thread.
	// Si el i es par y (i-15 > array_test_1[i]) quiere decir que tarda mucho en leer y tienen mas prioridad los escritores
	// Si el i es par y (array_test_1[i] > i+15) quiere decir que se tarda mucho en escribir y tienen mas prioridad los lectores
	// Viceversa si i es impar
	for(i = 0; i < CANT_THREADS; i++){
		if(i-15 > array_test_1[i] && array_test_1[i] > i+15){
			cout << "ERROR! Deadlock en test 1." << endl;
			return -1;
		}
	}

	return 0;
}

// Test 2 - Mas Escritores Que Lectores
int testMasEscritoresQueLectores(){
	long i;

	// Inicializo el arreglo writers_hasta_reader_test_2
	for(int i = 0; i < CANT_THREADS; i++)
		writers_hasta_reader_test_2[i] = 0;

	// Genero mi arreglo de pthreads, luego lanzamos el 10% de CANT_THREADS como
	// lectores y el resto como escritores.
	pthread_t threads[CANT_THREADS];
	for(i = 0; i < CANT_THREADS; i++){
		if(i % 10 == 0)
			pthread_create(&threads[i], NULL, thread_reader_test_2, (void*) i);
		else
			pthread_create(&threads[i], NULL, thread_writer_test_2, (void*) i);
	}

	// Espero hasta que terminen todos los threads para verificar
	// si termino todo bien.
	for(i = 0; i < CANT_THREADS; i++)
		pthread_join(threads[i], NULL);


	// Consideramos que si, siendo j = nro thread, j-15 <= writers_hasta_reader_test_2[j] <= j+15 el rango
	// es falso hay deadlock.
	// O sea, que si cuando fue lanzado el thread y llega a leer tienen que haber escrito cant. de
	// threads en ese rango.
	// Si hay más que el rango hay deadlock (tardaría en leer porque tienen más prioridad los escritores)
	// Si hay menos que el rango también hay deadlock (los lectores tienen más prioridad que los escritores)
	for(i = 0; i < CANT_THREADS; i++){
		if(i % 10 == 0 && i-15 > writers_hasta_reader_test_2[i] && writers_hasta_reader_test_2[i] > i+15){
			cout << "ERROR! Deadlock en test 2." << endl;
			return -1;
		}
	}

	// Si no escribieron el 90% de los threads entonces hay un error.
	if(writers_test_2 != (int) CANT_THREADS*0.9){
		cout << "ERROR! No escribieron todos en el test 2." << endl;
		return -1;
	}

	return 0;
}

// Test 3 - Mas Lectores Que Escritores
int testMasLectoresQueEscritores(){
	// int j;
	// bool hay_error;
	long i;

	// Inicializo el arreglo writers_hasta_reader_test_2
	for(int i = 0; i < CANT_THREADS; i++)
		readers_hasta_writer_test_3[i] = 0;

	// Genero mi arreglo de pthreads, luego lanzamos 10% de CANT_THREADS como escritores
	// y el resto como lectores
	pthread_t threads[CANT_THREADS];
	for(i = 0; i < CANT_THREADS; i++){
		if(i % 10 == 0)
			pthread_create(&threads[i], NULL, thread_writer_test_3, (void*) i);
		else
			pthread_create(&threads[i], NULL, thread_reader_test_3, (void*) i);
	}

	// Espero hasta que terminen todos los threads para verificar
	// si termino todo bien.
	for(i = 0; i < CANT_THREADS; i++)
		pthread_join(threads[i], NULL);


	// Consideramos que si, siendo j = nro thread, j-15 <= writers_hasta_reader_test_2[j] <= j+15 hay deadlock
	// O sea, que si cuando fue lanzado el thread y llega a leer tienen que haber leído cant. de
	// threads en ese rango.
	// Si hay más que el rango hay deadlock (tardaría en escribir porque tienen más prioridad los lectores)
	// Si hay menos que el rango también hay deadlock (los escritores tienen más prioridad que los lectores)
	for(int j = 0; j < CANT_THREADS; j++){
		if(i % 10 == 0 && j-15 > readers_hasta_writer_test_3[j] && writers_hasta_reader_test_2[j] > j+15){
			cout << "ERROR! Deadlock en test 3." << endl;
			return -1;
		}
	}

	// Si no leyeron el 90% de los threads entonces hay un error.
	if(readers_test_3 != (int) CANT_THREADS*0.9){
		cout << "ERROR! No leyeron todos en el test 3." << endl;
		return -1;
	}

	return 0;
}


int main(){

	int test1, test2, test3;

	cout << endl << "Corriendo el test 1." << endl;
	test1 = testMitadEscritoresMitadLectores();

	if(test1 == -1){
		cout << "ERROR! en test: testMitadEscritoresMitadLectores." << endl;
		return 0;
	} else {
		cout << "Test 1 pasó correctamente." << endl;
	}

	cout << endl << "Corriendo el test 2." << endl;
	test2 = testMasEscritoresQueLectores();

	if(test2 == -1){
		cout << "ERROR! en test: testMasEscritoresQueLectores." << endl;
		return 0;
	} else {
		cout << "Test 2 pasó correctamente." << endl;
	}

	cout << endl << "Corriendo el test 3." << endl;
	test3 = testMasLectoresQueEscritores();

	if(test3 == -1){
		cout << "ERROR! en test: testMasEscritoresQueLectores." << endl;
		return 0;
	} else {
		cout << "Test 3 pasó correctamente." << endl;
	}


	cout << endl << "Pasaron los test correctamente." << endl;
	return 0;
}


