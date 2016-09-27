#ifndef __SCHED_MFQ__
#define __SCHED_MFQ__

#include <vector>
#include <queue>
#include "basesched.h"

struct proceso {
	int pid;
	int quantum;
	unsigned int ultima_cola;
	proceso(int pid, int quantum) : pid(pid), quantum(quantum), ultima_cola(0) { };
};

using namespace std;

class SchedMFQ : public SchedBase {
	public:
		SchedMFQ(std::vector<int> argn);
        ~SchedMFQ();
		virtual void initialize() {};
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int n, const enum Motivo m);
	
	private:

		proceso* siguiente_proceso();
		void sacar_proceso(int core);

		vector<int> quantums_por_cola; //El quantum que corresponde a cada cola
		vector< queue<proceso*> > colas_de_procesos; //Vector de colas de procesos
		vector<proceso*> proceso_en_core; //Procesos corriendo
		vector<proceso*> proceso_bloqueado; //Procesos bloqueados

};

#endif
