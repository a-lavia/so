#ifndef __SCHED_RSJF__
#define __SCHED_RSJF__

#include <vector>
#include <queue>
#include <algorithm>
#include "basesched.h"

using namespace std;

struct proceso {
	int pid;
	int tiempo;

	proceso(int p, int t) : pid(p), tiempo(t) {}

	bool operator <(const proceso& other) const {
    return this->tiempo < other.tiempo;
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

		vector<int> quantum_core_base;
		vector<int> quantum_core_actual;
		vector<proceso*> proceso_en_core;

		queue<int> tiempo_proceso;
		priority_queue<proceso> cola_procesos;
};

#endif
