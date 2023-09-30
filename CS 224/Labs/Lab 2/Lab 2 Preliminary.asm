#
# Bilkent University
# Fall 2022-2023
#
# CS 224: Computer Organization
# Lab 2
#
# Deniz Tuna Onguner
# Bilkent ID: 22001788
#


### Part 1, convertHexToDec:

# Lab 02
# MIPS Assembly Language Programming Using Subprograms
# Preliminary Part 1: convertHexToDec
# Sun Oct 9, 2022

# Text Segment:
	.text
	.globl	__main

__main:
	la 	$a0, newLine
        li 	$v0, 4
        syscall
        
	# Print prompt for the string input
	la 	$a0, inputMessage
        li 	$v0, 4
        syscall
        
        # Ask for the user's input
	li	$v0, 8
	la	$a0, stringBuffer	# $a0 = string as char array
	li 	$a1, 12			# $a1 = max lenght of the string
	syscall
	
	# Call the subprogram
	jal	hexToDec
	
	# Print the result of convertion
	la 	$a0, labelDecimal
        li 	$v0, 4
        syscall
        
        move	$a0, $a3
        
        li 	$v0, 1
        syscall
        
        la 	$a0, newLine
        li 	$v0, 4
        syscall
        
        j	__main
	
exit:
	# Terminate program
	li	$v0, 10
	syscall


# Subprogram(s):
hexToDec:
	li	$s0, 0
	li	$s1, 0
	li	$s2, 0
	li	$s3, 0
	li	$s4, 0
	li	$s5, 0
	li	$s6, 0

	move	$s0, $a0		# $s0 = (relocated) string as char array
	la	$a2, 0($sp)		# $a2 = pointer to the initial stack

loop:
	lbu	$s1, 0($s0)		# $s1 = current char of the string input
	
	# if the string input is completely allocated
	beq	$s1, 10, pop
	
	# if the current char is not matched with hexadecimal number system
	bltu	$s1, 48, exit
	
	# if the current char is alphabetical
	bgtu	$s1, 57, alphabetical
	
	# 0 -> 48, 1 -> 49,...
	# numeric char value - 48 = real value in hex
	addiu	$s1, $s1, -48		
	j	push
	
alphabetical:
	# if the current char is not matched with hexadecimal number system
	bgtu	$s1, 70, exit
	
	# A -> 65, B -> 66,...
	# alphabetical char value - 55 = real value in hex
	addiu	$s1, $s1, -55
	
push:
	# push the char to the stack
	addi	$sp, $sp, 4 
	sw	$s1, 0($sp)
	
	# move on to the next char of the string
	addi	$s0, $s0, 1
	j	loop

	# All chars are pushed to the stack
pop:
	# Pop them and calculate the decimal value
	lw	$s1, 0($sp)		# $s1 (re-used) = current poped char
	addi	$sp, $sp, -4
	
	# Check if the power n is 0
	beq 	$s2, $zero, else	# $s2 = power n
	
	# Set $s3 to 1, initial value for multiplication
	addi 	$s3, $zero,    1	# $s3 = 16 to the power of n, where n is $s3
	
	# Set temp var to 0 for loop
	addi 	$s4, $zero,    0	# $s4 = loop temp
pow:
	# Execute loop to find 16 to the power of n
	slt 	$s5, $s4, $s2		# $s5 = condition of loop execution
	
	# Move on, execution is done
	beq 	$s5, $zero, next

	# Compute mul for n times
	mul 	$s3, $s3, 16
	
	# Increment the temp var
	addi 	$s4, $s4,  1
	j	pow

else:
	# 16 to the power of 0
	addi 	$s3, $zero, 1		# $s3 = 16 to the power of n, where n is $s3
	
next:
	addi	$s2, $s2,   1
	
	mul	$s1, $s1, $s3
	addu	$s6, $s6, $s1		# $s6 = sum
	
	bne	$a2, $sp, pop		# $a2 = pointer to the initial stack
	
	move	$a3, $s6		# $a3 = RETURN VALUE

	# return to main
	jr	$ra


# Data Segment:
	.data
inputMessage:
	.asciiz	"Enter a hexadecimal number: 0x"
labelDecimal:
	.asciiz "In Decimal\t\t  : "
stringBuffer:
	.space	12
newLine:
	.asciiz	"\n"


#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#--#


### Part 2, invertBytes:

# Lab 02
# MIPS Assembly Language Programming Using Subprograms
# Preliminary Part 2: invertBytes
# Mon Oct 10, 2022

# Text Segment:
	.text
	.globl	__main

__main:
	la 	$a0, newLine
        li 	$v0, 4
        syscall
        
	# Print prompt for the string input
	la 	$a0, inputMessage
        li 	$v0, 4
        syscall
        
        # Ask for the user's input
	li	$v0, 5
	syscall
	
	beq	$v0, 0, quit
	
	move	$s0, $v0
        
        la 	$a0, hexRepresent
        li 	$v0, 4
        syscall
        
        move	$a0, $s0
	
	li	$v0, 34
	syscall
	
	# Call the subprogram
	jal	invertBytes
	
	la 	$a0, newLine
        li 	$v0, 4
        syscall

	# Print the result of invertion
	la 	$a0, labelInverted
        li 	$v0, 4
        syscall

	move	$a0, $s3
	
	li	$v0, 34
	syscall
	
	la 	$a0, newLine
        li 	$v0, 4
        syscall
	
	j	__main

quit:
	# Terminate program
	li	$v0, 10
	syscall
	
	
# Subprogram(s):
invertBytes:
	li	$s0, 0
	li	$s1, 0
	li	$s2, 0
	li	$s3, 0

        sw 	$a0, 0($sp)
        li	$s2, 24
        
loop:
        lbu	$a1, 0($sp)
        sllv	$a1, $a1, $s2
        
        add	$s3, $s3, $a1
        
	addi	$sp, $sp,  1
	addi	$s1, $s1,  1
	addi	$s2, $s2, -8
	
	bne	$s1, 4, loop
	jr	$ra
	
	
# Data Segment:
	.data
inputMessage:
	.asciiz	"Enter an integer to invert: "
hexRepresent:
	.asciiz	"Integer in hexadecimal: "
labelInverted:
	.asciiz "Inverted\t      : "
newLine:
	.asciiz	"\n"

