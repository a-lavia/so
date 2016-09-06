#include "tasks.h"

using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0] - 1); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i] - 1);
		else uso_IO(pid, params[i]);
	}
}

void TaskConsola(int pid, vector<int> params) {
	// params: n bmin bmax
	
	// Entero que va a tomar el valor aleatorio entre bmin y bmax.
	int random_num;
	
	// Pasamos los valores de params a variables mas declarativas
	int n = params[0];
	int bmin = params[1];
	int bmax = params[2];

	// Iteramos n veces
	for(int i = 0; i < n; i++){
		
		// Generamos una duración al azar para la llamada bloqueante.
		random_num = bmin + rand() % (bmax - bmin + 1);

		// Se realiza la llamada bloqueante con la duración al azar.
		uso_IO(pid, random_num);

		// Ejecutamos 1 clock
		uso_CPU(pid, 1);
	}
	return;
}

void TaskBatch(int pid, vector<int> params) {
	// params: total_cpu cant_bloqueos
	
	// Entero que utlizamos para generar el valor pseudoaleatorio
	int random_num;
	
	// Pasamos los valores de params a variables mas declarativas
	int total_cpu = params[0];
	int cant_bloqueos = params[1];
	
	// El total del cpu que se utilice va a ser total_cpu - cant_bloqueos (ya que por cada uno de ellos
	// se utiliza 1 ciclo del cpu) y - 1 ya que return utiliza un ciclo de reloj.
	int cpu_restante = total_cpu - cant_bloqueos - 1;

	// Ciclo mientras todavia haya bloqueos para realizar
	while(cant_bloqueos > 0){
		// Genero un numero aleatorio entre 0 y cpu_restante
		random_num = rand() % (cpu_restante + 1);
		
		// Si el numero aleatorio es mayor que cero, uso el CPU
		// random_num ciclos.
		if(random_num > 0) uso_CPU(pid, random_num);
		
		// Descuento random_num ciclos a cpu_restante
		cpu_restante = cpu_restante - random_num;
		
		// Se realiza la llamada bloqueante
		uso_IO(pid, 2);
		
		cant_bloqueos--;
	}
	return;
}


void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2);
}
