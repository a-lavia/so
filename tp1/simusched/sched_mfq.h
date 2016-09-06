#ifndef __SCHED_MFQ__
#define __SCHED_MFQ__

#include <vector>
#include <queue>
#include <list>
#include "basesched.h"

typedef struct {int pid; int ultima_cola; int quantum;} proceso;
#define COLAS_VACIAS -1


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

		int hayProcesosEnColas();
		void eliminarProcesoDeLista(int pid);
		void agregarProcesoALista(int pid, int cola, int quantum);
		int actualizoQuantum(int pid, int q);
		int dameCola(int pid);

		vector< queue<int> > colas_de_procesos;
		vector<int> quantums_por_cola;
		list<proceso> procesos_corriendo;
};

#endif
