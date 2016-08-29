export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_sjf.o sched_rsjf.o sched_mfq.o
MAIN=simusched

.PHONY: all clean new
all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h \
	sched_mfq.h sched_rr.h sched_rsjf.h sched_sjf.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

ejercicio1:
	./simusched ej1lote.tsk 1 1 1 SchedFCFS >ej1.txt ; cat ej1.txt | python graphsched.py > graficos_ej1/ej1.png ; rm ej1.txt

ejercicio2:
	./simusched ej2lote.tsk 1 2 1 SchedFCFS >ej2_1.txt ; cat ej2_1.txt | python graphsched.py > graficos_ej2/ej2_1.png ; rm ej2_1.txt
	./simusched ej2lote.tsk 2 2 1 SchedFCFS >ej2_2.txt ; cat ej2_2.txt | python graphsched.py > graficos_ej2/ej2_2.png ; rm ej2_2.txt
	./simusched ej2lote.tsk 4 2 1 SchedFCFS >ej2_4.txt ; cat ej2_4.txt | python graphsched.py > graficos_ej2/ej2_4.png ; rm ej2_4.txt

ejercicio3:
	./simusched ej3lote.tsk 1 3 1 SchedFCFS >ej3.txt ; cat ej3.txt | python graphsched.py > graficos_ej3/ej3.png ; rm ej3.txt

clean:
	rm -f $(OBJS) $(MAIN)

new: clean all
