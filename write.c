#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define KEY 24602

int main(){
  char buffer[512];
  int shmd = shmget(KEY, 1, 0);
  int semd = semget(KEY + 1, 1, 0);
  int file = open("story", O_RDONLY);
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  semop(semd, &sb, 1);
  int *d = shmat(shmd, 0, 0);
  struct stat size;
  stat("story", &size);
  if (size.st_size){
    lseek(file, *d * -1, SEEK_END);
    read(file, buffer, *d);
    printf("Last Line: %s\n", buffer);
  }
  else {
    printf("Last Line: \n");
  }
  fgets(buffer,512,stdin);
  file = open("story", O_WRONLY | O_APPEND, 0644);
  int test = write(file, buffer, strlen(buffer));
  close(file);
  *d = strlen(buffer);
  shmdt(d);
	sb.sem_op = 1;
	semop(semd,&sb,1);
}
