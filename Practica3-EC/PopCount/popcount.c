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
	/*#define SIZE 4
	unsigned lista[SIZE]={0x80000000, 0x00100000, 0x00000800, 0x00000001};
	// 1 ^ + 1 ^ + 1 ^ + 1 ^ = 4
	#define RESULT 4
*/ 
	/*#define SIZE 8
	unsigned lista[SIZE]={0x7fffffff, 0xffefffff, 0xfffff7ff, 0xfffffffe, 0x01000024, 0x00356700, 0x8900ac00, 0x00bd00ef};
	#define RESULT 156*/

	#define SIZE 8
	unsigned lista[SIZE]={0x0 , 0x10204080, 0x3590ac06, 0x70b0d0e0, 0xffffffff, 0x12345678, 0x9abcdef0, 0xcafebeef};
	#define RESULT 116
/* -------------------------------------------------------------------- */
#endif

int resultado=0;

int popcount1(unsigned int* array, int len)
{
    int  i, j,  result=0;
    unsigned x;
    for (i=0; i<len; i++){
       x=array[i];
       for(j=0; j<8*sizeof(unsigned); j++){
          result += x & 0x1;
          x >>= 1;
       } 
    }
    return result;
}

int popcount2(unsigned int* array, int len)
{
    int  i,  result=0;
    unsigned x;
    for (i=0; i<len; i++){
       x=array[i];
       do {
          result += x & 0x1;
          x >>= 1;
       } while (x);
    }
    return result;
}

int popcount3(unsigned int* array, int len) 
{
    int  i,  result=0;
    unsigned x;
    for (i=0; i<len; i++){
       x=array[i];
       asm("\n\t"
	".ini3: \n\t"
	"shr %[x] \n\t" // desplazamiento lógico a la deerecha afecta CF y ZF
	"adc $0, %[r] \n\t"//suma con acarreo
	"testl  %[x], %[x] \n\t"//comprueba
	"jne .ini3 \n\t"//salto if not equal
	: [r]"+r" (result) // e/s: inicialmente 0, salida valor final
	: [x] "r" (x) );
    }
    return result;
}

int popcount4(unsigned int* array, int len)
{
    int result = 0, aux=0;
    int i, j;
    unsigned x;
    for (i=0; i<len; i++){
	x=array[i];
	for (j = 0; j < 8; j++) {
	aux += x & 0x01010101;
	x >>= 1;
	}
	aux += (aux >> 16);
	aux += (aux >> 8);
	result += aux & 0xFF;
	aux=0;
    }
return result;
}

int popcount5(unsigned* array, int len)
{
int i;
int val, result=0;
int SSE_mask[] = {0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f, 0x0f0f0f0f};
int SSE_LUTb[] = {0x02010100, 0x03020201, 0x03020201, 0x04030302};
// 3 2 1 0 7 6 5 4 1110 9 8 15141312
if (len & 0x3) printf("leyendo 128b pero len no múltiplo de 4?\n");
for (i=0; i<len; i+=4)
{
asm("movdqu %[x], %%xmm0 \n\t"
"movdqa %%xmm0, %%xmm1 \n\t" // dos copias de x
"movdqu %[m], %%xmm6 \n\t" // máscara
"psrlw $4 , %%xmm1 \n\t"
"pand %%xmm6, %%xmm0 \n\t" //; xmm0 – nibbles inferiores
"pand %%xmm6, %%xmm1 \n\t" //; xmm1 – nibbles superiores
"movdqu %[l], %%xmm2 \n\t" //; ...como pshufb sobrescribe LUT
"movdqa %%xmm2, %%xmm3 \n\t" //; ...queremos 2 copias
"pshufb %%xmm0, %%xmm2 \n\t" //; xmm2 = vector popcount inferiores
"pshufb %%xmm1, %%xmm3 \n\t" //; xmm3 = vector popcount superiores
"paddb %%xmm2, %%xmm3 \n\t" //; xmm3 – vector popcount bytes
"pxor %%xmm0, %%xmm0 \n\t" //; xmm0 = 0,0,0,0
"psadbw %%xmm0, %%xmm3 \n\t" //; xmm3 = [pcnt bytes0..7|pcnt bytes8..15]
"movhlps %%xmm3, %%xmm0 \n\t" //; xmm0 = [ 0 |pcnt bytes0..7 ]
"paddd %%xmm3, %%xmm0 \n\t" //; xmm0 = [ no usado |pcnt bytes0..15]
"movd %%xmm0, %[val] \n\t"
: [val]"=r" (val)
: [x] "m" (array[i]),
[m] "m" (SSE_mask[0]),
[l] "m" (SSE_LUTb[0])
);
result += val;
}
return result;
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
    crono(popcount1, " popcount con bucle for      ");
    crono(popcount2, " popcount con bucle do-while ");
    crono(popcount3, " popcount con do-while(asm)  ");
    crono(popcount4, " popcount con grupos de 8 bit");
    crono(popcount5, " popcount con copiado        ");
    //crono(suma3, "suma3 (bloque asm entero)");

    #if ! COPY_PASTE
    printf("calculado = %d\n", RESULT); /*OF*/
    #endif

    exit(0);
}
