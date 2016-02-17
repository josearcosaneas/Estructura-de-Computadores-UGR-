#include <stdio.h>	// para printf()
#include <stdlib.h>	// para exit()
#include <sys/time.h>	// para gettimeofday(), struct timeval

#define TEST		1
#define COPY_PASTE	0
// Para ver si COPY_PASTE=1 for((i=0;i<11;i++)) do echo $i; ./suma_09_modificada; done | pr -11 -l 4 -w 80

#if ! TEST
#define NBITS 20
#define SIZE (1<<NBITS)	// tamaño suficiente para tiempo apreciable
unsigned int lista[SIZE];
#define RESULT SIZE*(NBITS/2)
#else
/* -------------------------------------------------------------------- */
	#define SIZE 4
	unsigned lista[SIZE]={0x80000000, 0x00100000, 0x00000800, 0x00000001};
	// 1 ^ + 1 ^ + 1 ^ + 1 ^ = 4
	#define RESULT 4
/* -------------------------------------------------------------------- * /
	#define SIZE 8
	unsigned lista[SIZE]={0x7fffffff, 0xffefffff, 0xfffff7ff, 0xfffffffe, 0x01000024, 0x00356700, 0x8900ac00, 0x00bd00ef};
	#define RESULT 8
/* -------------------------------------------------------------------- * /
	#define SIZE 8
	unsigned lista[SIZE]={0x0 , 0x10204080, 0x3590ac06, 0x70b0d0e0, 0xffffffff, 0x12345678, 0x9abcdef0, 0xcafebeef};
	#define RESULT 2
/* -------------------------------------------------------------------- */
#endif

int resultado=0;

int paridad1(unsigned int* array, int len)
{
    int  i, j,  result=0, aux=0;
    unsigned x;
    for (i=0; i<len; i++){
       x=array[i];
       for(j=0; j<8*sizeof(unsigned); j++){
          aux ^= x & 0x1;
          x >>= 1;
       } 
	result += aux;
	aux=0;
    }
    return result;
}

int paridad2(unsigned int* array, int len)
{
    int  i,  result=0, aux=0;
    unsigned x;
    for (i=0; i<len; i++){
       x=array[i];
       do {
          aux ^= x & 0x1;
          x >>= 1;
       } while (x);
	result += aux;
	aux=0;
    }
    return result;
}

int paridad3(unsigned int* array, int len){
    int i,result=0,aux=0;
    unsigned x,val;
 	for (i=0; i<len; i++){
        if (i & 1){
	x=array[i];
	val = 0;
	           }
	continue;
	val += i;
			      }

}


void crono(int (*func)(), char* msg){
    struct timeval tv1,tv2;	// gettimeofday() secs-usecs
    long           tv_usecs;	// y sus cuentas

    gettimeofday(&tv1,NULL);
    resultado = func(lista, SIZE);
    gettimeofday(&tv2,NULL);

    tv_usecs=(tv2.tv_sec -tv1.tv_sec )*1E6+
             (tv2.tv_usec-tv1.tv_usec);
#if ! COPY_PASTE
    printf("resultado = %d\t", resultado);
    printf("%s:%9ld us\n", msg, tv_usecs);
#else
    printf("%9ld \n", tv_usecs);
#endif
}

int main()
{
    #if ! TEST
    int i;			// inicializar array
    for (i=0; i<SIZE; i++)	// se queda en cache
	 lista[i]=i;
    #endif
    crono(paridad1, " paridad con bucle for      ");
    crono(paridad2, " paridad con bucle do-while ");
    crono(paridad3, " con for y if               ");
    
    #if ! COPY_PASTE
    printf("calculado = %d\n", RESULT); /*OF*/
    #endif

    exit(0);
}
