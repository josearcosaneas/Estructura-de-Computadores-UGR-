#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>   
#include <sys/time.h> 

char password[]="califrajilistico\n";
int  passcode  = 9128;

void boom(){
	printf("***************\n");
	printf("*** BOOM!!! ***\n");
	printf("***************\n");
	exit(-1);
}

void defused(){
	printf("·························\n");
	printf("··· bomba desactivada ···\n");
	printf("·························\n");
	exit(0);
}

int main(){
#define SIZE 100
	char pass[SIZE];
	int  pasv;
#define TLIM 5
	struct timeval tiempo1,tiempo2;	

	gettimeofday(&tiempo1,NULL);

	printf("Introduce la contraseña: ");
	fgets(pass,SIZE,stdin);
	if (pass != password)
		boom();

	gettimeofday(&tiempo2,NULL);
	if (tiempo2.tv_sec - tiempo1.tv_sec > TLIM)
		boom();

	printf("Introduce el código: ");
	scanf("%i",&pasv);
	if (pasv!=passcode)
		boom();

	gettimeofday(&tiempo1,NULL);
	if (tiempo1.tv_sec - tiempo2.tv_sec > TLIM)
		boom();

	defused();

	return 0;
}
