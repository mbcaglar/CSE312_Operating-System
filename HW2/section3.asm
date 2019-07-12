        ; 8080 assembler code
        .hexfile section3.hex
        .binfile section3.com
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
stack3  equ 0ABE0h
stack4  equ 08CA0h
stack5  equ 06D60h
stack6  equ 055F0h
stack7  equ 04268h
stack8  equ 02EE0h
stack9  equ 01F40h
stack10 equ 00BB8h



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


begin:
	LXI SP,stack 	; always initialize the stack pointer 

	LXI B,s1	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS

	LXI B,s2	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS

	LXI B,s3	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS

	LXI B,s4	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS

	LXI B,s5	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS

	LXI B,s6	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS

	LXI B,s7	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS

	LXI B,s8	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS

	LXI B,s9	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS

	LXI B,s10	
	MVI A,TCreate	; thread call
	call GTU_OS	; call the OS


	MVI B,1
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS

	MVI B,2
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS


	MVI B,3
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS

	MVI B,4
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS

	MVI B,5
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS

	MVI B,6
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS

	MVI B,7
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS

	MVI B,8
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS

	MVI B,9
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS

	MVI B,10
	MVI A,TJoin	; thread call
	call GTU_OS	; call the OS


	
	hlt		; end program


s1:
	LXI SP,stack1 
	JMP f3
	
s2:
	LXI SP,stack2 
	JMP f3
	
s3:
	LXI SP,stack3 
	JMP f3
	
s4:
	LXI SP,stack4 
	JMP f3
	
s5:
	LXI SP,stack5 
	JMP f3
	
s6:
	LXI SP,stack6 
	JMP f3

	
s7:
	LXI SP,stack7 
	JMP f3

s8:
	LXI SP,stack8 
	JMP f3
s9:
	LXI SP,stack9 
	JMP f3
s10:
	LXI SP,stack10 
	JMP f3

f3:
	MVI B,50
	NUM equ 101

f3_:
	MVI A, PRINT_B 
	call GTU_OS ; system call
	INR B ;  b yi 2 artır
	MVI A, NUM ; size A ya atama yap
	CMP B ; A=A-b
	JNZ f3_ ; 
	MVI A,TExit
	call GTU_OS

