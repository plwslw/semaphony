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
    int semid;
    int shmid, shc;
    int sem_key=ftok("control.c", 22);
    int shm_key=ftok("client.c", 22); 
    int readfd,writefd;
    
    printf("wait for semaphore...\n");
    
    //semaphore down
    semid = semget(sem_key, 1, 0);
    struct sembuf buf = {0, -1, SEM_UNDO};
    semop(semid, &buf, 1);
    
    //access memory segment
	shmid = shmget(shm_key, 4, 0644);
	int* mem = shmat(shmid, &shc, 0);
    shc = 0;
    int size = *mem;
    
    //get last line from story.txt
    char message[size + 1];
    readfd = open("story.txt", O_RDONLY);
	lseek(readfd, -1 * size, SEEK_END);
	read(writefd, message, size);
    message[size] = 0;
	printf("previous line: %s\n",message);
    
    //write new line from user to story.txt
    char input[1000];
    printf("your new line: ");
	fgets(input, 1000, stdin);
	writefd = open("story.txt", O_WRONLY | O_APPEND, 0644);
    write(writefd, input, strlen(input));
    
    //update shm
    *mem = strlen(input);
    
    //semaphore up
	buf.sem_op = 1;
	semop(semid, &buf, 1);
    
    //detach semaphore
    shmdt(&shc);
    
    //close fd
    close(readfd);
    close(writefd);   
	return 0;
}