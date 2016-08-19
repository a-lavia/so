#include </usr/include/linux/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
// #include <reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <signal.h>


void yava(int i){
	printf("ya va!\n");
	
}

void sup(){
	printf("sup!\n");
}


int main(int argc, char* argv[]) {
	int status;
	pid_t child;	
	int count;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		/* Solo se ejecuta en el hijo */
		signal(SIGURG, &yava);

		for(count = 0; count < 5; count++){
			kill(getppid(), SIGINT);
			pause();
		}

		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);

	} else {
		/* Solo se ejecuta en el padre */
		signal(SIGINT, &sup);

		while(1){

			for(count = 0; count < 5; count++){
				sleep(1);
				kill(child, SIGURG);
			}

			if (wait(&status) < 0) { perror("waitpid"); break; }
			if (WIFEXITED(status)) break; /* Proceso terminado */
		}
	}
	return 0;
}
