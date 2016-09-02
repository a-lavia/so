#include "sched_rsjf.h"

using namespace std;

SchedRSJF::SchedRSJF(vector<int> argn) {
	// Recibe la cantidad de cores y sus cpu_quantum por parámetro
	for(int i = 1; i <= argn[0]; i++) {
		this->quantum_core_base.push_back(argn[i]);
		this->quantum_core_actual.push_back(argn[i]);
		this->proceso_en_core.push_back(NULL);
	}

	for(unsigned int i = argn[0]+1; i < argn.size(); i++)
		this->tiempo_proceso.push(argn[i]);

}

SchedRSJF::~SchedRSJF() {

}

void SchedRSJF::load(int pid) {
	proceso nuevoProceso = proceso(pid, this->tiempo_proceso.front());
	this->tiempo_proceso.pop();
	cola_procesos.push(nuevoProceso);
}

void SchedRSJF::unblock(int pid) {
	//No se bloquean
}

int SchedRSJF::tick(int core, const enum Motivo m) {
	switch (m) {
		case TICK:
			//FIJARSE SI ES IDLE
			//Me fijo si hay un proceso corriendo en el core actualmente
			if(proceso_en_core[core] != NULL) {

				//Le descontamos el tiempo al proceso, y disminuimos el quantum
				proceso_en_core[core]->tiempo--;
				quantum_core_actual[core]--;

				//Se termino el proceso
				if(proceso_en_core[core]->tiempo == 0) {
					//No agregarlo a la cola, obtener el siguiente
				} else if(quantum_core_actual[core] == 0) { //Se termino su quantum
					quantum_core_actual[core] = quantum_core_base[core];

				}

			} else {
				//Obtenemos el proceso de la cola
			}
			break;


		default:
			break;
	}
	return 0;
}
