; 8080 assembler code
        .hexfile Sort.hex
        .binfile Sort.com
        ; try "hex" for downloading in hex format
        .download bin  
        .objcopy gobjcopy
        .postbuild echo "OK!"
        ;.nodump

	; OS call list
PRINT_B		equ 1
PRINT_MEM	equ 2
READ_B		equ 3
READ_MEM	equ 4
PRINT_STR	equ 5
READ_STR	equ 6
GET_RND     equ 7
	; Position for stack pointer
stack   equ 0F000h

	org 000H
	jmp begin

	; Start of our Operating System
GTU_OS:	PUSH D
	push D
	push H
	push psw
	nop	; This is where we run our OS in C++, see the CPU8080::isSystemCall()
		; function for the detail.
	pop psw
	pop h
	pop d
	pop D
	ret
	; ---------------------------------------------------------------
	; YOU SHOULD NOT CHANGE ANYTHING ABOVE THIS LINE        

arr ds 100

begin:
	LXI SP,stack 	; always initialize the stack pointer
	LXI D, arr
	MVI C,0 ;
	MVI L,0 
	NUM	equ 50;

Loop_array: 
			MVI A, GET_RND ;
			call GTU_OS ; system call
			MOV A, B
			STAX D ;arraye yükleme
			MVI A, NUM
			INR C
			INX D
			INX D
			CMP C
			JNZ Loop_array

			MVI C,0
			LXI D,arr
			JMP while1

i:
	INR C
	MVI L,0 

while1:
		MVI A,49
		CMP C
		JZ Exit
		LXI D,arr
while2:
		MVI A,49
		CMP L
		JZ i
		LDAX D ;arrayin ilk elamnını a yükleme
		MOV H,A
		INX D
		INX D
		LDAX D ;arrayin 2. elamnını a yükleme
		CMP H
		JC SWAP
		JMP COUNTER
SWAP:
	LDAX D
	MOV B,A
	MOV A,H
	STAX D ; arr[i+1] = d
	DCX D
	DCX D
	MOV A,B
	STAX D ; arr[i] = e
	INX D
	INX D

COUNTER:
	INR L
	JMP while2


Exit:
		MVI C,0 ; i
		LXI D,arr ;

Loop_array_print:
			LDAX D;
			MOV B,A
			MVI A,1
			call GTU_OS
			MVI A, NUM
			INR C
			INX D
			INX D
			CMP C
			JNZ Loop_array_print
	hlt		; end program
