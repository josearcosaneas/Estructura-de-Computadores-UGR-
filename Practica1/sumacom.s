.section .data
lista:		.int 1, -2 ,5,5,-0xfffffff,-0xfffffff,-0xfffffff
longlista:	.int (.-lista)/4
resultado:	.quad -1 	#declaro resultado como un quad para que tenga 64 bytes


.section .text
_start:	.global _start

	mov    $lista, %ebx
	mov longlista, %ecx
	call suma

	mov %edx, resultado
	mov %eax, resultado

	mov $1, %eax
	mov $0, %ebx
	int $0x80

suma: 
	push %esi     		#esi es el indice
	mov $0, %eax  		#Inicio a 0 los acumuladores y registros
	mov $0, %edx
	mov $0, %esi
	mov $0, %ebp
bucle:
		
	CDQ
	mov (%ebx,%esi,4),%ebp
	add %ebp, %edx	         #cambio add por adc para sumar con el acarreo	
	adc $0, %edx		 #sumamos en eax con adc 0 para sumar el acarreo si lo hubiera

	inc %esi

	cmp %esi,%ecx
	jne bucle

	pop %esi
	ret
