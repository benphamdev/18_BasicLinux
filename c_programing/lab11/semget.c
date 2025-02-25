#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
union semun
{
 int val;
};
int main(void)
{
 int semid;
 int key;
 pid_t pid;
 struct sembuf sem;
 union semun semun_union;
 key = ftok("./a.c", 0666);
 semid = semget(key, 1, 0666 | IPC_CREAT);
 semun_union.val = 0;
 semctl(semid, 0, SETVAL, semun_union);
 pid = fork();
 if (pid > 0)
 {
 sem.sem_num = 0;
 sem.sem_op = -1; // P operation
 sem.sem_flg = 0;
 semop(semid, &sem, 1);
 printf("This is parents\n");
 sem.sem_num = 0;
 sem.sem_op = 1;
 sem.sem_flg = 0;
 semop(semid, &sem, 1);
 }
 if (pid == 0)
 {
 sleep(2);
 sem.sem_num = 0;
 sem.sem_op = 1; //V operation
 sem.sem_flg = 0;
 semop(semid, &sem, 1);
 printf("This is son\n");
 }
 return 0;
}
