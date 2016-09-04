#ifndef __SCHED_RSJF__
#define __SCHED_RSJF__

#include <vector>
#include <queue>
#include <algorithm>
#include "basesched.h"

using namespace std;

struct ProcesoRSJF {
	int pid;
	int tiempo;

	ProcesoRSJF(int p, int t) : pid(p), tiempo(t) {}

	bool operator >(const ProcesoRSJF& other) const {
    return this->tiempo > other.tiempo;
	}

};

class SchedRSJF : public SchedBase {
	public:
		SchedRSJF(std::vector<int> argn);
    ~SchedRSJF();
		virtual void initialize() {};
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);
	private:

		vector<int> quantum_por_core;
		vector<int> quantum_actual;

		vector<ProcesoRSJF> proceso_en_core;

		queue<int> tiempos_procesos;
		priority_queue<ProcesoRSJF, vector<ProcesoRSJF>, greater<ProcesoRSJF> > cola_procesos;
};

#endif
