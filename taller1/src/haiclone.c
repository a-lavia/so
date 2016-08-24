#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

pid_t child;
int count;

void SIGURG_HANDLER() {
	printf("ya va! \n");
	count++;
}

void SIGINT_HANDLER() {
	if(count < 5) {
		printf("sup! \n");
		kill(child, SIGURG);
		sleep(1);
		count++;
	}
}

int main(int argc, char* argv[]) {
	int status;
	pid_t parent = getpid();

	count = 0;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}

  signal(SIGINT, &SIGINT_HANDLER);

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {

    signal(SIGURG, &SIGURG_HANDLER);

    while(1) {
			if(count == 5) {
				/* Solo se ejecuta en el hijo */
				execvp(argv[1], argv+1);
				/* Si vuelve de exec() hubo un error */
				perror("ERROR child exec(...)"); exit(1);
			} else {
				kill(parent, SIGINT);
			}
    }
	} else {
		/* Solo se ejecuta en el padre */
		while(1) {
			printf("aaaa");
			if (wait(&status) < 0) { perror("waitpid"); break; }
			if (WIFEXITED(status)) break; /* Proceso terminado */
		}

	}
	return 0;
}
