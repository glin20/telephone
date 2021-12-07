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

union semun {
  int val;                  //used for SETVAL
  struct semid_ds *buf;     //used for IPC_STAT and IPC_SET
  unsigned short  *array;   //used for SETALL
  struct seminfo  *__buf;
};

int main(int argc, char *argv[]){
  if (argc > 1){
    if (!strcmp(argv[1],"-create")){
      int file = open("story", O_RDWR | O_CREAT | O_TRUNC, 0644);
      int shmd = shmget(KEY, sizeof(int), IPC_CREAT | 0640);
      int semd = semget(KEY + 1, 1, IPC_CREAT | IPC_EXCL | 0644);
      union semun us;
      us.val = 1;
      int r = semctl(semd, 0, SETVAL, us);
      printf("Initialized shared memory, semaphore, file\n");
    }
    else if (!strcmp(argv[1],"-remove")){
      int file = open("story", O_RDONLY);
      struct stat sb;
      stat("story", &sb);
      char *story = malloc(sb.st_size);
      int readden = read(file, story, sb.st_size);
      printf("Story so far: \n");
      printf("%s\n", story);
      int shmd = shmget(KEY, 1, 0);
      int semd = semget(KEY + 1, 1, 0);
      shmctl(shmd, IPC_RMID, 0);
      semctl(semd, IPC_RMID, 0);
      close(file);
    }
  }
}
