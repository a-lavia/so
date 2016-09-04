#include "sched_rsjf.h"

using namespace std;

SchedRSJF::SchedRSJF(vector<int> argn) {
	// Recibe la cantidad de cores y sus cpu_quantum por parámetro
	ProcesoRSJF proceso_vacio(0,0);

	// Guardo los quantums de los procesos en los vectores quantum_actual y quantum_por_core
	// Inicializo el vector proceso_en_core con procesos vacios.
	for(int i = 1; i <= argn[0]; i++){
		this->quantum_por_core.push_back(argn[i]);
		this->quantum_actual.push_back(argn[i]);
		this->proceso_en_core.push_back(proceso_vacio);
	}

	// Encolo los tiempos de los procesos.
	for(unsigned int i = argn[0]+1; i < argn.size(); i++)
		this->tiempos_procesos.push(argn[i]);
}

SchedRSJF::~SchedRSJF() {
}

void SchedRSJF::load(int pid) {
	// Creo un struct nuevo del proceso y lo encolo en la cola de prioridad.
	ProcesoRSJF nuevo_proceso(pid, tiempos_procesos.front());
	cola_procesos.push(nuevo_proceso);
	//Quito de la cola tiempos_procesos el tiempo del proceso agregado.
	tiempos_procesos.pop();
}

void SchedRSJF::unblock(int pid) {
	// No toma procesos que se bloquean
}

int SchedRSJF::tick(int core, const enum Motivo m) {
	int prox_proc = IDLE_TASK;

	// Si el proceso actual es IDLE_TASK y cola_procesos no esta vacia, devuelvo el primero.
	// Sino devuelvo IDLE 
	if(current_pid(core) == IDLE_TASK){
		if(!cola_procesos.empty()){
			ProcesoRSJF proceso_a_ejec = cola_procesos.top();
			cola_procesos.pop();
			prox_proc = proceso_a_ejec.pid;
			proceso_en_core[core] = proceso_a_ejec;
			quantum_actual[core] = quantum_por_core[core];
		}
	
	} else {
		// Si el motivo es que el proceso termino y la cola_procesos no esta vacia, devuelvo el primero.
		// Sino devuelvo idle
		if(m == EXIT){
			if(!cola_procesos.empty()){
				ProcesoRSJF proceso_a_ejec = cola_procesos.top();
				cola_procesos.pop();

				proceso_en_core[core] = proceso_a_ejec;

				prox_proc = proceso_a_ejec.pid;
			
				quantum_actual[core] = quantum_por_core[core];
			}
		
		} else if (m == TICK){
			// Descuento el tiempo al proceso y el quantum del core
			proceso_en_core[core].tiempo--;
			quantum_actual[core]--;

			// Si el quantum del core termino y no esta vacia, desencolo el proximo, guardo el actual,
			// Actualizo el quantum y devuelvo
			if(quantum_actual[core] == 0 && !cola_procesos.empty()){
				ProcesoRSJF proceso_a_ejec = cola_procesos.top();
				cola_procesos.pop();
				prox_proc = proceso_a_ejec.pid;

				cola_procesos.push(proceso_en_core[core]);
				proceso_en_core[core] = proceso_a_ejec;
			
				quantum_actual[core] = quantum_por_core[core];

			} else if(quantum_actual[core] == 0){
				// Si el quantum terminó y la cola esta vacia, actualizo el quantum y devuelvo el actual.
				quantum_actual[core] = quantum_por_core[core];
				prox_proc = current_pid(core);
			} else {
				// El quantum no terminó, devuelvo el actual
				prox_proc = current_pid(core);
			}
		}
	}

	return prox_proc;
}
