#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char* argv[]) {
	int status;
	pid_t child;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		/* Solo se ejecuta en el hijo */

		ptrace(PTRACE_TRACEME, 0, NULL, NULL);

		execvp(argv[1], argv+1);
		/* Si vuelve de exec() hubo un error */
		perror("ERROR child exec(...)"); exit(1);

	} else {
		/* Solo se ejecuta en el padre */
		int eskill = 0;

		ptrace(PTRACE_ATTACH, child, NULL, NULL);

		while(1) {

			if (wait(&status) < 0) { perror("waitpid"); break; }
			if (WIFEXITED(status)) break; /* Proceso terminado */

			//NUMERO DE SYSCALL: KILL = 62
			if(62 == ptrace(PTRACE_PEEKUSER, child, 8*ORIG_RAX, NULL)){
				ptrace(PTRACE_KILL, child, NULL, NULL);
				eskill = 1;
				break;
			}

			ptrace(PTRACE_SYSCALL, child, NULL, NULL); /* Continua */
		}

		if(eskill == 1)
			printf("Se ha hecho justicia!\n");

		ptrace(PTRACE_DETACH, child, NULL, NULL); /* Liberamos al hijo */
	}

	return 0;
}
