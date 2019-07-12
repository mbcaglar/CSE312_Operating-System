        ; 8080 assembler code
        .hexfile section2.hex
        .binfile section2.com
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
FileCreate  equ 12
FileClose	equ 13
FileOpen	equ 14
FileRead	equ 15
FileWrite	equ	16
FileSeek	equ	17
DirRead		equ	18
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

	;This program prints a null terminated string to the screen

arr2 ds 260
input_s: dw '',00AH,00H
input_s1: dw '',00AH,00H
input_f: dw '',00AH,00H


begin:
	LXI SP,stack 	; always initialize the stack pointer

LXI B,input_s1
MVI A,READ_STR
call GTU_OS

LXI B,input_s1
MVI A, FileOpen
call GTU_OS ; system cal

MOV E,B
LXI B,arr2
MVI D,250
MVI A, FileRead 
call GTU_OS ; system cal

MOV B,E
MVI A, FileClose 
call GTU_OS ; system cal

LXI B,arr2
MVI A,PRINT_STR
call GTU_OS
hlt
