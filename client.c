#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>


int main(int argc, char *argv[]){
    int semid, sc;
    int shmid, shc;
    int sem_key=ftok("control.c", 22);
    int shm_key=ftok("client.c", 22); 
    int readfd,writefd;

    semid = semget(sem_key, 1, IPC_CREAT | 0644 );
	shmid = shmget(shm_key, 100, IPC_CREAT | 0644);

	char input[1000];
	struct sembuf buf = {0, -1, SEM_UNDO};
	semop(semid, &buf, 1);
	shmat(shmid, 0, 0);
    readfd = open("story.txt", O_RDONLY);
	writefd = open("story.txt", O_CREAT | O_APPEND, 0644);
	char message[(*mem) + 1];
	lseek(readfd, (*mem) * -1, SEEK_END);
	read(writefd, message, (*mem));
	message[(*mem)] = 0;
	printf("story: %s",message);
    
    printf("Type to add to story");
	fgets(input, 1000, stdin);
	write(writefd, input, strlen(input));
	*mem = strlen(input);

	buf.sem_num = 1;
	semop(semid, &buf, 1);
   
    close(readfd);
    close(writefd);   
	return 0;
}