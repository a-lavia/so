#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

pid_t child;
int count = 0;

void SIGURG_HANDLER() {
	write(1, "ya va!\n",7);
	count++;
	int i = 0;
	while(i < 10){
		i++;
	}
}

void SIGINT_HANDLER() {
	write(1, "sup!\n", 5);
	count++;
}

int main(int argc, char* argv[]) {
	int status;
	

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}


	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		/* Solo se ejecuta en el hijo */
		signal(SIGURG, &SIGURG_HANDLER);
		
		while(count < 5){
		}

		pid_t parent = getppid();
		kill(parent,SIGINT);

		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);

	} else {
		/* Solo se ejecuta en el padre */

		signal(SIGINT, &SIGINT_HANDLER);

		while(count < 1){
			sleep(1);
			write(1, "sup!\n", 5);
			kill(child, SIGURG);
		}

		while(1) {
			if (wait(&status) < 0) { perror("waitpid"); break; }
			if (WIFEXITED(status)) break; /* Proceso terminado */
		}
	}
	return 0;
}
