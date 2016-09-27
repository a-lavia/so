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
	./simusched lotes_de_tareas/ej1lote.tsk 1 1 1 SchedFCFS  | python graphsched.py > ../informe/imagenes/ej1.png

ejercicio2:
	./simusched lotes_de_tareas/ej2lote.tsk 1 2 1 SchedFCFS | python graphsched.py > ../informe/imagenes/ej2_1.png
	./simusched lotes_de_tareas/ej2lote.tsk 2 2 1 SchedFCFS | python graphsched.py > ../informe/imagenes/ej2_2.png 
	./simusched lotes_de_tareas/ej2lote.tsk 4 2 1 SchedFCFS | python graphsched.py > ../informe/imagenes/ej2_4.png 

ejercicio3:
	./simusched lotes_de_tareas/ej3lote.tsk 1 3 1 SchedFCFS | python graphsched.py > ../informe/imagenes/ej3.png

ejercicio5:
	./simusched lotes_de_tareas/ej2lote.tsk 1 2 1 SchedRR 2 | python graphsched.py > ../informe/imagenes/ej5_2.png
	./simusched lotes_de_tareas/ej2lote.tsk 1 2 1 SchedRR 5 | python graphsched.py > ../informe/imagenes/ej5_5.png
	./simusched lotes_de_tareas/ej2lote.tsk 1 2 1 SchedRR 10 | python graphsched.py > ../informe/imagenes/ej5_10.png

ejercicio8:
	./simusched lotes_de_tareas/ej8lote1.tsk 1 2 1 SchedFCFS  | python graphsched.py > ../informe/imagenes/ej8_prueba1_fcfs.png
	./simusched lotes_de_tareas/ej8lote1.tsk 1 2 1 SchedSJF 5 7 3 11 2  | python graphsched.py > ../informe/imagenes/ej8_prueba1_sjf.png
	./simusched lotes_de_tareas/ej8lote1.tsk 1 2 1 SchedRR 3  | python graphsched.py > ../informe/imagenes/ej8_prueba1_rr.png
	./simusched lotes_de_tareas/ej8lote1.tsk 1 2 1 SchedRSJF 3 5 7 3 11 2  | python graphsched.py > ../informe/imagenes/ej8_prueba1_rsjf.png
	./simusched lotes_de_tareas/ej8lote1.tsk 2 2 1 SchedFCFS  | python graphsched.py > ../informe/imagenes/ej8_prueba1_fcfs2.png
	./simusched lotes_de_tareas/ej8lote1.tsk 2 2 1 SchedSJF 5 7 3 11 2  | python graphsched.py > ../informe/imagenes/ej8_prueba1_sjf2.png
	./simusched lotes_de_tareas/ej8lote1.tsk 2 2 1 SchedRR 3 3 | python graphsched.py > ../informe/imagenes/ej8_prueba1_rr2.png
	./simusched lotes_de_tareas/ej8lote1.tsk 2 2 1 SchedRSJF 3 3 5 7 3 11 2  | python graphsched.py > ../informe/imagenes/ej8_prueba1_rsjf2.png
	./simusched lotes_de_tareas/ej8lote2.tsk 1 2 1 SchedFCFS  | python graphsched.py > ../informe/imagenes/ej8_prueba2_fcfs.png
	./simusched lotes_de_tareas/ej8lote2.tsk 1 2 1 SchedSJF 5 7 3 11 2  | python graphsched.py > ../informe/imagenes/ej8_prueba2_sjf.png
	./simusched lotes_de_tareas/ej8lote2.tsk 1 2 1 SchedRR 3  | python graphsched.py > ../informe/imagenes/ej8_prueba2_rr.png
	./simusched lotes_de_tareas/ej8lote2.tsk 1 2 1 SchedRSJF 3 5 7 3 11 2  | python graphsched.py > ../informe/imagenes/ej8_prueba2_rsjf.png
	./simusched lotes_de_tareas/ej8lote2.tsk 2 2 1 SchedFCFS  | python graphsched.py > ../informe/imagenes/ej8_prueba2_fcfs2.png
	./simusched lotes_de_tareas/ej8lote2.tsk 2 2 1 SchedSJF 5 7 3 11 2  | python graphsched.py > ../informe/imagenes/ej8_prueba2_sjf2.png
	./simusched lotes_de_tareas/ej8lote2.tsk 2 2 1 SchedRR 3 3  | python graphsched.py > ../informe/imagenes/ej8_prueba2_rr2.png
	./simusched lotes_de_tareas/ej8lote2.tsk 2 2 1 SchedRSJF 3 3 5 7 3 11 2  | python graphsched.py > ../informe/imagenes/ej8_prueba2_rsjf2.png

ejercicio9:
	./simusched lotes_de_tareas/ej9lote.tsk 1 2 1 SchedMFQ 2 7 13  | python graphsched.py > ../informe/imagenes/ej9_1.png
	./simusched lotes_de_tareas/ej9lote.tsk 1 2 1 SchedMFQ 4 6 8  | python graphsched.py > ../informe/imagenes/ej9_2.png
	./simusched lotes_de_tareas/ej9lote.tsk 1 2 1 SchedMFQ 1 2 3  | python graphsched.py > ../informe/imagenes/ej9_3.png


clean:
	rm -f $(OBJS) $(MAIN)

new: clean all
