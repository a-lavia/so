#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro

	// Guardo en un vector los valores de los quantums que estan asociados a los cores
	for(int i = 1; i <= argn[0]; i++){
		this->quantum_por_core.push_back(argn[i]);
	}

	// Copio los valores de los quantums al vector que lo va a asociar a cada tarea
	// para que vaya disminuyendo por la cantidad de ticks
	this->quantum_proceso = this->quantum_por_core;
}

SchedRR::~SchedRR() {
}


void SchedRR::load(int pid) {
	// LLegó un proceso nuevo, lo encolo.
	this->cola_procesos.push(pid);
}

void SchedRR::unblock(int pid) {
	// Se desbloqueó un proceso, lo encolo.
	this->cola_procesos.push(pid);
}

int SchedRR::tick(int cpu, const enum Motivo m) {
	int prox_pid = IDLE_TASK;

	if(current_pid(cpu) == IDLE_TASK){
		if(!this->cola_procesos.empty()){
			prox_pid = this->cola_procesos.front();
			this->cola_procesos.pop();
			this->quantum_proceso[cpu] = this->quantum_por_core[cpu];
		}
	
	} else {
		if(m == TICK){
			this->quantum_proceso[cpu]--;
			prox_pid = current_pid(cpu);

			if(this->quantum_proceso[cpu] == 0){
				if(!this->cola_procesos.empty()){
					this->cola_procesos.push(prox_pid);
					prox_pid = this->cola_procesos.front();
					this->cola_procesos.pop();
				}

				this->quantum_proceso[cpu] = this->quantum_por_core[cpu];
			}
		
		} else {
			if(!this->cola_procesos.empty()){
				prox_pid = this->cola_procesos.front();
				this->cola_procesos.pop();
				this->quantum_proceso[cpu] = this->quantum_por_core[cpu];
			}
		}
	}

	return prox_pid;
}
