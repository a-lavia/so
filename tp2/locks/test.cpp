#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include "RWLock.h"

using namespace std;


RWLock lockTest;


void* thread_reader_test(void* soy){
	lockTest.rlock();
	cout << "Read" << endl;
	sleep(1);
	lockTest.runlock();

	pthread_exit(NULL);
}

void* thread_writer_test(void* soy){
	lockTest.wlock();
	cout << "Write" << endl;
	sleep(1);
	lockTest.wunlock();

	pthread_exit(NULL);
}



void test1(){
	pthread_t threads[101];
	int a=0;

	cout << endl << "TEST 1" << endl;
	cout << endl << "50 threads lectores" << endl;

	// Lanzo 50 thread lectores
	for(int i = 0; i < 50; i++){
		pthread_create(&threads[i], NULL, thread_reader_test, (void*) &i);
	}

	cout << endl << "1 thread escritor" << endl;

	// Lanzo un thread escritores
	pthread_create(&threads[50], NULL, thread_writer_test, (void*) &a);

	cout << endl << "50 threads lectores" << endl;

	// Lanzo 50 thread lectores
	for(int i = 51; i < 101; i++){
		pthread_create(&threads[i], NULL, thread_reader_test, (void*) &i);
	}

	// Espero hasta que terminen todos los threads
	for(int i = 0; i < 101; i++){
		pthread_join(threads[i], NULL);
	}

	return;
}


void test2(){
	pthread_t threads[110];

	cout << endl << "TEST 2" << endl;
	cout << endl << "50 threads lectores" << endl;
	
	// Lanzo 50 thread lectores
	for(int i = 0; i < 50; i++){
		pthread_create(&threads[i], NULL, thread_reader_test, (void*) &i);
	}

	cout << endl << "10 threads escritores" << endl;

	// Lanzo 10 thread escritores
	for(int i = 50; i < 60; i++){
		pthread_create(&threads[i], NULL, thread_writer_test, (void*) &i);
	}

	cout << endl << "50 threads lectores" << endl;

	// Lanzo 50 thread lectores
	for(int i = 60; i < 110; i++){
		pthread_create(&threads[i], NULL, thread_reader_test, (void*) &i);
	}

	// Espero hasta que terminen todos los threads
	for(int i = 0; i < 110; i++){
		pthread_join(threads[i], NULL);
	}

	return;
}


void test3(){
	pthread_t threads[50];

	cout << endl << "TEST 3" << endl;
	
	// Lanzo 25 threads lectores y 25 threads escritores.
	// si es par es lector, si es impar es escritor
	for(int i = 0; i < 50; i++){
		if(i % 2 == 0){
			pthread_create(&threads[i], NULL, thread_reader_test, (void*) &i);
		} else {
			pthread_create(&threads[i], NULL, thread_writer_test, (void*) &i);
		}
	}

	// Espero hasta que terminen todos los threads
	for(int i = 0; i < 50; i++){
		pthread_join(threads[i], NULL);
	}

	return;
}


int main(){
	test1();
	test2();
	test3();
	return 0;
}


