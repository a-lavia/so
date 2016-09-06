#include <vector>
#include <queue>
#include "sched_mfq.h"
#include "basesched.h"

using namespace std;

SchedMFQ::SchedMFQ(vector<int> argn) {
	// MFQ recibe los quantums por parámetro

	for(unsigned int i = 1; i < argn.size(); i++){
		queue<int> nueva_cola;
		this->colas_de_procesos.push_back(nueva_cola);
		quantums_por_cola.push_back(argn[i]);
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
	(*it).ultima_cola = (unsigned int) (*it).ultima_cola != this->colas_de_procesos.size() -1 ? (*it).ultima_cola+1 : (*it).ultima_cola;
	this->colas_de_procesos[(*it).ultima_cola].push(pid);

	eliminarProcesoDeLista(pid);
}

int SchedMFQ::tick(int core, const enum Motivo m) {
	int prox_pid = IDLE_TASK;

	if(current_pid(core) == IDLE_TASK){
		// Si la tarea es idle, me fijo si hay tareas en alguna cola y tomo
		// la que tiene mayor prioridad

		int idx = hayProcesosEnColas();

		if(idx != COLAS_VACIAS){
			// hay procesos, tomo el de la cola con indice idx
			prox_pid = colas_de_procesos[idx].front();
			colas_de_procesos[idx].pop();
			agregarProcesoALista(prox_pid, idx, quantums_por_cola[idx]);
		}

	} else if(m == TICK) {
		// Actualizo el quantum del proceso que esta corriendo
		int quantum_proceso_actual = actualizoQuantum(current_pid(core), -1);
		prox_pid = current_pid(core);

		if(quantum_proceso_actual == 0){
			// Si el quantum terminó y si hay algun otro proceso encolado, lo devuelvo
			int idx = hayProcesosEnColas();
			int proceso_a_desalojar = current_pid(core);

			if(idx != COLAS_VACIAS){
				// Hay otro proceso, desalojo el que esta, lo agrego en la cola que debe estar
				// y devuelvo el de mayor prioridad

				prox_pid = colas_de_procesos[idx].front();
				colas_de_procesos[idx].pop();
				agregarProcesoALista(prox_pid, idx, quantums_por_cola[idx]);

				int cola_proceso_desalojado = (unsigned int) dameCola(proceso_a_desalojar) == colas_de_procesos.size()-1 ? dameCola(proceso_a_desalojar) : dameCola(proceso_a_desalojar)+1;
				colas_de_procesos[cola_proceso_desalojado].push(proceso_a_desalojar);
				eliminarProcesoDeLista(proceso_a_desalojar);

			} else {
				// No hay otros procesos encolados y su quantum terminó, lo actualizo
				actualizoQuantum(proceso_a_desalojar, quantums_por_cola[dameCola(proceso_a_desalojar)]);
			}

		}

	} else if(m == EXIT){
		// Si termino el proceso de correr, busco si hay alguno que continúe, sino devuelvo idle
		int idx = hayProcesosEnColas();
		// Quito el que termino de la lista
		eliminarProcesoDeLista(current_pid(core));


		if(idx != COLAS_VACIAS){
			// Hay otro proceso encolado, lo devuelvo
			prox_pid = colas_de_procesos[idx].front();
			colas_de_procesos[idx].pop();

			agregarProcesoALista(prox_pid, idx, quantums_por_cola[idx]);
		}


	} else {
		// m == block
		// Se bloqueó el proceso actual, si hay otro proceso lo devuelvo, sino devuelvo idle
		int idx = hayProcesosEnColas();

		if(idx != COLAS_VACIAS){
			// Hay otro proceso encolado, lo devuelvo
			prox_pid = colas_de_procesos[idx].front();
			colas_de_procesos[idx].pop();

			agregarProcesoALista(prox_pid, idx, quantums_por_cola[idx]);
		}
	}

	return prox_pid;
}

int SchedMFQ::hayProcesosEnColas(){
	int idx = -1;

	for(unsigned int i = 0; i < this->colas_de_procesos.size(); i++){
		if(!(this->colas_de_procesos[i].empty())){
			idx = i;
			break;
		}
	}

	return idx;
}

void SchedMFQ::eliminarProcesoDeLista(int pid){
	list<proceso>::iterator it = this->procesos_corriendo.begin();

	while(it != this->procesos_corriendo.end() && (*it).pid != pid)
		++it;

	this->procesos_corriendo.erase(it);
}

void SchedMFQ::agregarProcesoALista(int pid, int cola, int quantum){
	proceso nuevo_proceso;
	nuevo_proceso.pid = pid;
	nuevo_proceso.ultima_cola = cola;
	nuevo_proceso.quantum = quantum;
	this->procesos_corriendo.push_back(nuevo_proceso);
}

int SchedMFQ::actualizoQuantum(int pid, int q){
	list<proceso>::iterator it = this->procesos_corriendo.begin();

	while(it != this->procesos_corriendo.end() && (*it).pid != pid)
		++it;

	(*it).quantum += q;

	return (*it).quantum;
}

int SchedMFQ::dameCola(int pid){
	list<proceso>::iterator it = this->procesos_corriendo.begin();

	while(it != this->procesos_corriendo.end() && (*it).pid != pid)
		++it;

	return (*it).ultima_cola;
}