#include <vector>
#include <queue>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro

	// Guardo en un vector los valores de los quantums que estan asociados a los cores
	for(int i = 1; i <= argn[0]; i++)
		this->quantum_por_core.push_back(argn[i]);

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

	if(m == EXIT){
		// Terminó su ejecución, pasa a la proxima. Actualizo el quantum para la proxima tarea.
		if(!(this->cola_procesos.empty())) {
			prox_pid = this->cola_procesos.front();
			this->cola_procesos.pop();
		}
		// Actualizo el quantum aunque no haya ninguna tarea
		this->quantum_proceso[cpu] = this->quantum_por_core[cpu];
	
	} else if(m == BLOCK){
		// Se bloqueó un proceso, devuelvo un proceso nuevo de la pila para que termine el quantum
		prox_pid = this->cola_procesos.front();
		this->cola_procesos.pop();

	} else {
		// Si es la tarea NO es idle: actualizo el quantum y reviso si se le terminó o no.
		if(current_pid(cpu) != IDLE_TASK){
			// La tarea no termino. Descuento el quantum.
			this->quantum_proceso[cpu]--;
		
			// Reviso si termino el quantum que tenía.
			if(this->quantum_proceso[cpu] <= 0){
				//Terminó su quantum, lo actualizo, la encolo de nuevo y desencolo la nueva.
				this->quantum_proceso[cpu] = this->quantum_por_core[cpu];
				
				int pid_tarea_a_encolar = current_pid(cpu);
				
				this->cola_procesos.push(pid_tarea_a_encolar);
				
				prox_pid = this->cola_procesos.front();
				
				this->cola_procesos.pop();
			} else {
				// No terminó su quantum, continua
				prox_pid = current_pid(cpu);
			}
		} else {
			// La tarea es idle, me fijo si hay una tarea nueva en la cola, si la hay la ejecuto y actualizo el quantum
			if(this->cola_procesos.size() > 1){
				prox_pid = this->cola_procesos.front();
				this->cola_procesos.pop();
			}
		}
	}

	return prox_pid;
}
