#ifndef __SCHED_SJF__
#define __SCHED_SJF__

#include <vector>
#include <queue>
#include <algorithm>
#include "basesched.h"

using namespace std;

struct Proceso{
	int pid;
	int tiempo;

	Proceso(int p, int t) : pid(p), tiempo(t) {}

	bool operator >(const Proceso& p2) const {
		return this->tiempo > p2.tiempo;
	}
};

class SchedSJF : public SchedBase {
	public:
		SchedSJF(std::vector<int> argn);
        ~SchedSJF();
		virtual void initialize() {};
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);	
	private:

	queue<int> tiempos_procesos;
	priority_queue<Proceso, vector<Proceso>, greater<Proceso> > cola_procesos;
};

#endif
