        ; 8080 assembler code
        .hexfile section5.hex
        .binfile section5.com
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
GET_RND		equ 7
TExit		equ 8
TJoin		equ 9
TYield		equ 10
TCreate		equ 11

	; Position for stack pointer
stack   equ 0F000h
stack1  equ 0EA60h
stack2  equ 0CB20h
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

	;This program adds numbers from 0 to 10. The result is stored at variable
	; sum. The results is also printed on the screen.
	
string: dw '',00AH,00H
number: dw '',00AH,00H

sum	ds 2
arr ds 100

begin:
	LXI SP,stack 	; always initialize the stack pointer 

	LXI B,f1	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS
	
	LXI B,f3	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS


	MVI B,1	
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS

	MVI B,2	
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS

	hlt		; end program

f1:
	LXI SP,stack1
	MVI B,0
	NUM	equ 51;
f1_:  	
	MVI A, PRINT_B 
	call GTU_OS ; system call
	MVI A,TYield
	call GTU_OS ; system call
	INR B ;  b yi 2 artır
	MVI A, NUM ; size A ya atama yap
	CMP B ; A=A-b
	JNZ f1_ ; 
	MVI A,TExit
	call GTU_OS	


f3:
	LXI SP,stack2
	MVI B,50
	NUM equ 101
f3_:
	MVI A, PRINT_B 
	call GTU_OS ; system call
	MVI A,TYield
	call GTU_OS ; system call
	INR B ;  b yi 2 artır
	MVI A, NUM ; size A ya atama yap
	CMP B ; A=A-b
	JNZ f3_ ; 
	MVI A,TExit
	call GTU_OS
