#include <vector>
#include <queue>
#include <iostream>
#include "sched_sjf.h"

using namespace std;

SchedSJF::SchedSJF(vector<int> argn) {
	// Recibe la cantidad de cores

	// Encolo los tiempos de los procesos.
	for(unsigned int i = 1; i < argn.size(); i++)
		tiempos_procesos.push(argn[i]);

}

SchedSJF::~SchedSJF() {
}

void SchedSJF::load(int pid) {
	// Creo un struct nuevo del proceso y lo encolo en la cola de prioridad.
	Proceso nuevo_proceso(pid, tiempos_procesos.front());
	procesos.push(nuevo_proceso);
	//Quito de la cola tiempos_procesos el tiempo del proceso agregado.
	tiempos_procesos.pop();
}

void SchedSJF::unblock(int pid) {
	// No toma procesos que se bloquean
}

int SchedSJF::tick(int cpu, const enum Motivo m) {
	int prox_proc = IDLE_TASK;

	if(current_pid(cpu) == IDLE_TASK){
		if(!procesos.empty()){
			Proceso proceso_a_ejec = procesos.top();
			procesos.pop();
			prox_proc = proceso_a_ejec.pid;
		}
	
	} else {
		if(m == EXIT){
			if(!procesos.empty()){
				Proceso proceso_a_ejec = procesos.top();
				procesos.pop();
				prox_proc = proceso_a_ejec.pid;
			}
		
		} else {
			prox_proc = current_pid(cpu);
		}
	}

	return prox_proc;
}
