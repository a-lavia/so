#include <vector>
#include <queue>
#include "sched_mfq.h"
#include "basesched.h"
#define NULL 0

using namespace std;

SchedMFQ::SchedMFQ(vector<int> argn) {
	// En el primer parametro llegan la cantidad de cores
	proceso_en_core.resize(argn[0]);
	for(unsigned int i = 0; i < proceso_en_core.size(); i++) {
		proceso_en_core[i] = NULL;
	}

	// Luego los quantum para cada cola
	colas_de_procesos.resize(argn.size()-1);
	quantums_por_cola.resize(argn.size()-1);
	for(unsigned int i = 0; i < argn.size()-1; i++) {
		this->quantums_por_cola[i] = argn[i+1];
	}

}

SchedMFQ::~SchedMFQ() {
	//Limpiamos la memoria
	for(unsigned int i = 0; i < proceso_en_core.size(); i++){
		if(proceso_en_core[i] != NULL) {
			delete proceso_en_core[i];
		}
	}
	for(unsigned int i = 0; i<colas_de_procesos.size();i++) {
		while(!colas_de_procesos[i].empty()) {
			proceso* p = colas_de_procesos[i].front();
			colas_de_procesos[i].pop();
			delete p;
		}
	}
}

void SchedMFQ::load(int pid) {
	// Ingresa nuevo proceso, se encola en la cola de mayor prioridad
	proceso* p = new proceso(pid, quantums_por_cola[0]);
	colas_de_procesos[0].push(p);
}

void SchedMFQ::unblock(int pid) {
	//Buscamos el proceso bloqueado y lo sacamos
	proceso* p = NULL;
	for(vector<proceso*>::iterator it = proceso_bloqueado.begin(); it != proceso_bloqueado.end(); ++it) {
		if((*it)->pid == pid) {
			p = *it;
			proceso_bloqueado.erase(it);
			break;
		}
	}
	//Le subimos la prioridad en la cola
	unsigned int cola = p->ultima_cola; 
	cola = cola == 0 ? cola : cola - 1;
	colas_de_procesos[cola].push(p);
}

int SchedMFQ::tick(int core, const enum Motivo m) {

	if(current_pid(core) == IDLE_TASK) {
		//Le buscamos un proceso al core que esta en idle
		proceso_en_core[core] = siguiente_proceso();
	} else {
		switch(m) {
			case TICK:
				//Restamos el quantum
				proceso_en_core[core]->quantum--;
				//En caso de acabarse su quantum, le bajamos la prioridad y buscamos otro proceso
				if(proceso_en_core[core]->quantum == 0) {
					unsigned int cola = proceso_en_core[core]->ultima_cola;
					cola = cola + 1 == colas_de_procesos.size() ? cola : cola+1;

					proceso_en_core[core]->ultima_cola = cola;
					colas_de_procesos[cola].push(proceso_en_core[core]);
					proceso_en_core[core] = siguiente_proceso();
				}
				break;
			case BLOCK:
				//Lo agregamos a procesos bloqueados y buscamos el siguiente proceso
				proceso_bloqueado.push_back(proceso_en_core[core]);
				proceso_en_core[core] = siguiente_proceso();
				break;
			case EXIT:
				//Eliminamos el proceso, y buscamos el siguiente
				delete proceso_en_core[core];
				proceso_en_core[core] = siguiente_proceso();
				break;
		}
	}

	return proceso_en_core[core] == NULL ? IDLE_TASK : proceso_en_core[core]->pid;
}


proceso* SchedMFQ::siguiente_proceso() {
	//Buscamos el siguiente proceso mas prioritario
	for(unsigned int i = 0; i<colas_de_procesos.size();i++) {
		if(!colas_de_procesos[i].empty()) {

			proceso* p = colas_de_procesos[i].front();
			colas_de_procesos[i].pop();

			p->quantum = quantums_por_cola[i];

			return p;
		}
	}
	return NULL;
}
