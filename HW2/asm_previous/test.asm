        ; 8080 assembler code
        .hexfile test.hex
        .binfile test.com
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

 
NUM equ 9
print: equ 0
deger: equ 0
string: dw 'HW1 CSE312 OS BEDIRHAN CAGLAR',00AH,00H
input_s: dw 'HELLO MY TEACHER',00AH,00H

begin:

	MVI A,READ_B
	call GTU_OS

	MVI A,PRINT_B
	CALL GTU_OS

	LXI B,deger
	MVI A,READ_MEM
	CALL GTU_OS


	LXI B,deger
	MVI A,PRINT_MEM
	CALL GTU_OS


	LXI B,input_s
	MVI A,READ_STR
	call GTU_OS

	LXI B,input_S
	MVI A,PRINT_STR
	call GTU_OS

	MVI A,GET_RND
	call GTU_OS

	MVI A,PRINT_B
	call GTU_OS

	LXI B,string
	MVI A,PRINT_STR
	call GTU_OS

	hlt

