#include <vector>
#include <queue>
#include "sched_mfq.h"
#include "basesched.h"

using namespace std;

SchedMFQ::SchedMFQ(vector<int> argn) {
	// MFQ recibe los quantums por parámetro

	for(unsigned int i = 0; i < argn.size(); i++){
		queue<int> nueva_cola;
		this->colas_de_procesos.push_back(nueva_cola);

		quantums_por_cola.push_back(argn[i]);
		quantum_proceso.push_back(argn[i]);
	}

}

SchedMFQ::~SchedMFQ() {
}

void SchedMFQ::load(int pid) {
	// Ingresa nuevo proceso, se encola en la cola de mayor prioridad
	colas_de_procesos[0].push(pid);
}

void SchedMFQ::unblock(int pid) {
	// Pusheo el proceso en la pila siguiente a la que estaba, si es la ultima, queda ahí
	list<proceso>::iterator it = this->procesos_corriendo.begin();
	while(it != this->procesos_corriendo.end() && (*it).pid != pid)
		++it;

	// Acutualizo el valor de la ultima cola del proceso y agrego el valor en la cola
	(*it).ultima_cola = (*it).ultima_cola != this->colas_de_procesos.size() -1 ? (*it).ultima_cola+1 : (*it).ultima_cola;
	this->colas_de_procesos[(*it).ultima_cola].push(pid);

}

int SchedMFQ::tick(int core, const enum Motivo m) {
	int prox_pid = IDLE_TASK;

	if(current_pid(core) == IDLE_TASK){
		// Si la tarea que esta corriendo es idle y hay alguna en las colas, devuelvo la de mayor prioridad.

		// Miro las colas desde la de mayor prioridad.
		int idx = hayProcesosEnColas();

		if(idx != COLAS_VACIAS){
			// Hay colas no vacias, devuelvo el de mayor prioridad y actualizo el quantum
			prox_pid = colas_de_procesos[idx].front();
			colas_de_procesos[idx].pop();

			agregarProcesoALista(prox_pid, idx);

			quantum_proceso[idx] = quantums_por_cola[idx];
		}

	} else { 

		if(m == TICK){
		// Descuento el tick del proceso actual
		int cola_de_proceso_actual = dameCola(current_pid(core));
		quantum_proceso[cola_de_proceso_actual]--;

			if(quantum_proceso[cola_de_proceso_actual] == 0){
				// Si hay otro proceso lo devuelvo ese

				int idx = hayProcesosEnColas();

				if(idx != COLAS_VACIAS){
					// Hay otro proceso lo devuelvo
					prox_pid = colas_de_procesos[idx].front();
					colas_de_procesos[idx].pop();

					// Agrego el proceso que se le termino el quantum en la siguiente
					// cola, salvo que sea la ultima
					list<proceso>::iterator it = procesos_corriendo.begin();
					while(it != procesos_corriendo.end() && (*it).pid != current_pid(core))
						++it;

					if((*it).ultima_cola == colas_de_procesos.size()-1){
						colas_de_procesos[(*it).ultima_cola].push((*it).pid);
					} else {
						colas_de_procesos[(*it).ultima_cola+1].push((*it).pid);
					}

					// Lo elimino de la lista
					eliminarProcesoDeLista(current_pid(core));

					// Agrego el nuevo a la lista
					agregarProcesoALista(prox_pid, idx);
					quantum_proceso[idx] = quantums_por_cola[idx];

				}

				// Actualizo quantum, del core actual
				quantum_proceso[cola_de_proceso_actual] = quantums_por_cola[cola_de_proceso_actual];
			}


		} else if(m == EXIT){
			// Terminó el proceso, si hay otro proceso encolado lo devuelvo, sino devuelvo idle
			// Lo quito de la lista de procesos corriendo
			eliminarProcesoDeLista(current_pid(core));

			int idx = hayProcesosEnColas();
			if(idx != COLAS_VACIAS){
				// Hay otro proceso lo devuelvo
				prox_pid = colas_de_procesos[idx].front();
				colas_de_procesos[idx].pop();

				agregarProcesoALista(prox_pid, idx);

				quantum_proceso[idx] = quantums_por_cola[idx];
			}

		} else if(m == BLOCK){
			// El proceso se bloqueó, paso al siguiente si lo hay
			int idx = hayProcesosEnColas();
			if(idx != COLAS_VACIAS){
				// Hay otro proceso lo devuelvo
				prox_pid = colas_de_procesos[idx].front();
				colas_de_procesos[idx].pop();

				agregarProcesoALista(prox_pid, idx);

				quantum_proceso[idx] = quantums_por_cola[idx];
			}
		}
	}

	return prox_pid;
}

int SchedMFQ::hayProcesosEnColas(){
	int idx = -1;

	for(unsigned int i = 0; i < colas_de_procesos.size(); i++){
		if(!colas_de_procesos[i].empty()){
			idx = i;
			break;
		}
	}

	return idx;
}

void SchedMFQ::eliminarProcesoDeLista(int pid){
	list<proceso>::iterator it = procesos_corriendo.begin();

	while(it != procesos_corriendo.end() && (*it).pid != pid)
		++it;

	procesos_corriendo.erase(it);
}

void SchedMFQ::agregarProcesoALista(int pid, int cola){
	proceso nuevo_proceso;
	nuevo_proceso.pid = pid;
	nuevo_proceso.ultima_cola = cola;
	this->procesos_corriendo.push_back(nuevo_proceso);
}

int SchedMFQ::dameCola(int pid){
	list<proceso>::iterator it = procesos_corriendo.begin();

	while(it != procesos_corriendo.end() && (*it).pid != pid)
		++it;	

	return (*it).ultima_cola;
}