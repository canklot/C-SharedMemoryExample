#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
int main(int argc, char *argv[])
{

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

/* the size (in bytes) of shared memory object */
const int SIZE =4096;
/* name of the shared memory object */
const char *name ="OS";
/* shared memory file descriptor */
int shm_fd;
/* pointer to shared memory obect */
void *ptr;

pid_t mypid;
mypid=fork();

if(mypid<0){
printf("%s","error");
fprintf(stderr,"fork failed");
}

else if (mypid==0){

struct timeval startget;
gettimeofday(&startget,NULL);
double startms=startget.tv_sec*1000+startget.tv_usec/1000;
printf("%f", startms);

//sleep(2); //3saniye

shm_fd=shm_open(name,O_CREAT | O_RDWR, 0666);
ftruncate(shm_fd,SIZE);
ptr = mmap(0, SIZE,PROT_WRITE,MAP_SHARED,shm_fd,0);

char output[50];
snprintf(output, 50, "%f", startms);
sprintf(ptr,"%s",output);
ptr+=strlen(output);
//execlp(argv[1],argv[1],argv[2],argv[3],argv[4],NULL);

execvp(argv[1],argv+1);
//delay(3000000); //3saniye
//exit(0);

}

else{
wait(NULL);
//exit(0);

struct timeval endget;
gettimeofday(&endget,NULL);
double endms=endget.tv_sec*1000+endget.tv_usec/1000;

/* open the shared memory object */
shm_fd=shm_open(name,O_RDONLY, 0666);
/* memory map the shared memory object */
ptr= mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
/* read from the shared memory object */
//printf("%s","okunan alttaki\n");
//printf("%s", (char *)ptr);

char* yancar;
double okunan;
okunan = strtod(((char *)ptr), &yancar);
double fark= (double)(endms - okunan) / CLOCKS_PER_SEC;
//printf("%f %s", endms ,"end mikro saniye \n");
printf("%f %s", fark*1000, "saniye \n");


/* remove the shared memory object */
shm_unlink (name);

}

return 0;
}

