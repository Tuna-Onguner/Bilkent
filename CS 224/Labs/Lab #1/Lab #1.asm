#
# Bilkent University
# Fall 2022-2023
#
# CS 224: Computer Organization
# Lab 1
#
# Deniz Tuna Onguner
# Bilkent ID: 22001788
#


##
## Program1.asm - prints out "hello world"
##
##	a0 - points to the string
##

#################################
#				#
#	text segment		#	
#				#
#################################

	.text		
	.globl __start 

__start:			# execution starts here
	la 	$a0, str	# put string address into a0
	li 	$v0, 4		# system call to print
	syscall			# out a string

	li 	$v0, 10		# system call to exit
	syscall			# bye bye


#################################
#				#
#     	 data segment		#
#				#
#################################

	.data
str:	.asciiz "Hello World\n"
n:	.word	10

##
## end of file Program1.asm



##
## Program2.asm asks user for temperature in Celsius,
##  converts to Fahrenheit, prints the result.
##
##	v0 - reads in Celsius
##	t0 - holds Fahrenheit result
##	a0 - points to output strings
##

#################################
#				#
#	text segment		#
#				#
#################################

	.text		
	.globl __start	

__start:
	la 	$a0, prompt	# output prompt message on terminal
	li 	$v0, 4		# syscall 4 prints the string
	syscall

	li	 $v0, 5		# syscall 5 reads an integer
	syscall

	mul 	$t0, $v0,  9	# to convert,multiply by 9,
	div 	$t0, $t0,  5	# divide by 5, then
	add 	$t0, $t0, 32	# add 32

	la 	$a0, ans1	# print string before result
	li 	$v0, 4
	syscall

	move 	$a0, $t0	# print integer result
	li 	$v0, 1		# using syscall 1
	syscall

	la	$a0, endl	# system call to print
	li 	$v0, 4		# out a newline
	syscall

	li 	$v0, 10		# system call to exit
	syscall			#    bye bye


#################################
#				#
#     	 data segment		#
#				#
#################################

	.data
prompt:	.asciiz "Enter temperature (Celsius): "
ans1:	.asciiz "The temperature in Fahrenheit is "
endl:	.asciiz "\n"

##
## end of file Program2.asm



##
##	Program3.asm is a loop implementation
##	of the Fibonacci function
##        

#################################
#				#
#	text segment		#
#				#
#################################

	.text		
	.globl __start
 
__start:			# execution starts here
	li 	$a0, 7		# to calculate fib(7)
	jal 	fib		# call fib
	
	move 	$a0, $v0	# print result
	
	li 	$v0, 1
	syscall

	la 	$a0, endl	# print newline
	li 	$v0, 4
	syscall

	li 	$v0, 10
	syscall			# bye bye

#------------------------------------------------


fib:	
	move 	$v0, $a0	# initialise last element
	blt 	$a0, 2, done	# fib(0)=0, fib(1)=1

	li 	$t0, 0		# second last element
	li 	$v0, 1		# last element

loop:	
	add 	$t1, $t0, $v0	# get next value

	move 	$t0, $v0	# update second last
	move 	$v0, $t1	# update last element
	
	sub 	$a0, $a0,  1	# decrement count
	bgt 	$a0, 1, loop	# exit loop when count=0
	
done:	
	jr 	$ra

#################################
#				#
#     	 data segment		#
#				#
#################################

	.data
endl:	.asciiz "\n"

##
## end of Program3.asm



# A = (B/C - D) mod B

	.text
	la	$a0, B
	li	$v0, 4
	syscall
	
	li	$v0, 5
	syscall 
	
	la	$s0, 0($v0)
	
	la	$a0, C
	li	$v0, 4
	syscall
	
	li	$v0, 5
	syscall 
	
	la	$s1, 0($v0)
	
	la	$a0, D
	li	$v0, 4
	syscall
	
	li	$v0, 5
	syscall 
	
	la	$s2, 0($v0)
	
	div	$s0, $s1
	mflo	$t0
	
	sub	$t0, $t0, $s2
	
	div	$t0, $s0
	mfhi	$t0
	
	la	$a0, A
	li	$v0, 4
	syscall
	
	li	$v0, 1
	move	$a0, $t0
	syscall
	
	li	$v0, 10
	syscall
	
	
	.data
A:	.asciiz	"A = "
B:	.asciiz	"B: "
C:	.asciiz	"C: "
D:	.asciiz	"D: "

	

# Lab 1, Part 4: Using MIPS for implementing a program with a simple menu that involve loop

# text segment

	.text
	.globl __main__
	
__main__:
	la	$a1, array	# $a1 => address of the array
	
	li 	$v0, 4
	la	$a0, msgSize
	syscall
	
	li	$v0, 5
	syscall
	
	move	$a2, $v0	# $a2 => size of the array
	
	jal	fillArray
	
menu:
	li	$v0, 4
	la	$a0, msgMenu
	syscall
	
	li	$v0, 12
	syscall
	
	move	$a3, $v0
	
	li	$v0, 4
	la	$a0, newLine
	syscall
		
	beq	$a3,  97, option_a
	beq	$a3,  98, option_b
	beq	$a3,  99, option_c
	beq	$a3, 100, option_d
	
	b	menu
	
option_a:

	jal	OPTION_A
	b	menu
	
option_b:

	jal	OPTION_B
	b	menu
	
option_c:
	
	jal	OPTION_C
	b	menu
	
option_d:
	li	$v0, 10
	syscall
# text segment end



# Function: fillArray

fillArray:
	move	$t0, $a1
	move	$t1, $zero
	
faLoop:
	li	$v0, 4
	la	$a0, msgElement
	syscall
	
	li	$v0, 1
	la	$a0, 1($t1)
	syscall
	
	li	$v0, 4
	la	$a0, colon
	syscall
	
	li	$v0, 5
	syscall
	
	sw	$v0, 0($t0)
	
	addi	$t1, $t1, 1
	addi	$t0, $t0, 4
	
	bne	$t1, $a2, faLoop
	
	li	$v0, 4
	la	$a0, newLine
	syscall
	
	jr	$ra
# fillArray end



# Function: OPTION_A

OPTION_A:
	li 	$v0, 4
	la	$a0, inputNumber
	syscall
	
	li	$v0, 5
	syscall
	
	move	$t2, $v0
	move	$t0, $a1
	move	$t1, $zero
	move	$t3, $zero
	move	$t4, $zero
	
opALoop:
	lw	$t3, 0($t0)
	
	bgt	$t3, $t2, true
	b	endif
	
true:
	add	$t4, $t4, $t3

endif:
	addi	$t1, $t1, 1
	addi	$t0, $t0, 4
	
	bne	$t1, $a2, opALoop
	
	li 	$v0, 4
	la	$a0, result
	syscall
	
	li 	$v0, 1
	move	$a0, $t4 
	syscall
	
	li	$v0, 4
	la	$a0, newLineD
	syscall
	
	jr	$ra
# OPTION_A end



# Function: OPTION_B

OPTION_B:
	move	$t0, $a1
	move	$t1, $zero
	move	$t2, $zero
	move	$t3, $zero
	move	$t4, $zero
	move	$t5, $zero
	move	$t6, $zero
	addi	$t4, $t4, 2
	
opBLoop:
	lw	$t3, 0($t0)
	
	div	$t3, $t4
	mfhi	$t2
	
	bne	$t2, 0, odd
	b	even
	
odd:
	add	$t5, $t5, $t3
	b	next

even:
	add	$t6, $t6, $t3
	
next:
	
	addi	$t1, $t1, 1
	addi	$t0, $t0, 4
	
	bne	$t1, $a2, opBLoop
	
	li 	$v0, 4
	la	$a0, sumEven
	syscall
	
	li 	$v0, 1
	move	$a0, $t6 
	syscall
	
	li	$v0, 4
	la	$a0, newLine
	syscall
	
	
	li 	$v0, 4
	la	$a0, sumOdds
	syscall
	
	li 	$v0, 1
	move	$a0, $t5
	syscall
	
	li	$v0, 4
	la	$a0, newLineD
	syscall
	
	jr	$ra
# OPTION_B end



# Function: OPTION_C

OPTION_C:
	move	$t0, $a1
	move	$t1, $zero
	move	$t7, $zero
	
	li 	$v0, 4
	la	$a0, inputNumber
	syscall
	
	li	$v0, 5
	syscall
	
	move	$t2, $v0
	
opCLoop:
	lw	$t3, 0($t0)
	
	div	$t3, $t2
	mfhi	$t4
	
	beq	$t4, $zero, mod0
	b	ifend
	
mod0:
	addi	$t7, $t7, 1

ifend:
	addi	$t1, $t1, 1
	addi	$t0, $t0, 4
	
	bne	$t1, $a2, opCLoop
	
	li	$v0, 4
	la	$a0, count
	syscall
	
	li	$v0, 1
	move	$a0, $t7
	syscall
	
	li	$v0, 4
	la	$a0, newLineD
	syscall
	
	jr	$ra
# OPTION_C end



# data segment

	.data
array:	.space	400
comma:	.asciiz	", "
colon:	.asciiz ": "
newLine:
	.asciiz "\n"
newLineD:
	.asciiz "\n\n"
msgMenu:
	.asciiz "Option: "
msgSize:
	.asciiz "Enter the number of elements (max 100): "
msgElement:
	.asciiz "Element "
inputNumber:
	.asciiz "Choose a number: "
result:	.asciiz "result = "
count:	.asciiz	"count = "
sumEven:
	.asciiz "Even sum: "
sumOdds:
	.asciiz "Odds sum: "
# data segment end



