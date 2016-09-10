#include "mt.h"

#define ADDR_LOCAL "127.000.000.001"

int main(int argc, char* argv[]) {
	int                   socket_fd;
	// int                   len;
	struct sockaddr_in    remote;
	char                  buf[MAX_MSG_LENGTH];
	struct in_addr        dir_local;

	/* Crear un socket de tipo UNIX con SOCK_STREAM */
	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 1){
		perror("Error creando socket");
		exit(1);
	}

	/* Resuelvo la direccion IP */
	if((inet_aton(ADDR_LOCAL, &dir_local)) == 0){
		perror("Error al intentar resolver dirección IP");
		exit(1);
	}


	/* Establecer la dirección a la cual conectarse. */
	remote.sin_family = AF_INET;
	remote.sin_port = htons(PORT);
	remote.sin_addr = dir_local;

	// len = sizeof(struct sockaddr_in);

	/* Conectarse */
	if(connect(socket_fd, (struct sockaddr *)&remote, sizeof(remote)) == 1){
		perror("Error conectandose");
		exit(1);
	}

	/* Establecer la dirección a la cual conectarse para escuchar. */
	while(printf("> "), fgets(buf, MAX_MSG_LENGTH, stdin), !feof(stdin)){
		if(sendto(socket_fd, buf, strlen(buf), 0, (struct sockaddr *)&remote, sizeof(remote)) == -1){
			perror("Error enviando");
			exit(1);
		}
	}

	/* Cerrar el socket. */
	close(socket_fd);

	return 0;
}
