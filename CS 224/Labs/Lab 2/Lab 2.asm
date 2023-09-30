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


	.text
	.globl	__main
	
__main:
	la	$a0, arraySizePrompt
	li	$v0, 4
	syscall
	
	li	$v0, 5
	syscall
	
	move	$a0, $v0
	
	li	$v0, 9
	syscall
	
	move	$a1, $a0		# $a1 => size of the array
	move	$a0, $v0		# $a2 => address of the array
	
	jal	monitor
	
	move	$s0, $a0
	
loop1:
    	bge     $t0, $a1, exit

  	lw      $t2, 0($s0)
    	addi    $s0, $s0, 4

    	li      $v0, 1      
    	move    $a0, $t2
    	syscall
    	
    	li      $a0, 32
    	li      $v0, 11  
    	syscall

	addi    $t0, $t0, 1
    	j       loop1
exit:

	li	$v0, 10
	syscall



monitor:
	addi	$sp, $sp, -16
	sw	$s0,  0($sp)
	sw	$s1,  4($sp)
	sw	$s2,  8($sp)
	sw	$ra, 12($sp)
	
	move	$s2, $a0
	move	$s1, $s2
loopMonitor:
	li	$v0, 4
	la	$a0, elementPrompt
	syscall
	
	li	$v0, 1
	la	$a0, 1($s0)		# $s0 = counter for the loop
	syscall
	
	li	$v0, 4
	la	$a0, colon
	syscall
	
	li	$v0, 5
	syscall
	
	sw	$v0, 0($s1)
	
	addi	$s0, $s0, 1
	addi	$s1, $s1, 4
	
	bne	$s0, $a1, loopMonitor
	
	li	$v0, 4
	la	$a0, newLine
	syscall
	
	move	$a0, $s2
	
	jal	bubbleSort

	lw	$ra, 12($sp)
	lw	$s2,  8($sp)
	lw	$s1,  4($sp)
	lw	$s0,  0($sp)
	addi	$sp, $sp,  16
	
	jr	$ra



bubbleSort:
	addi	$sp, $sp, -20
	sw	$s0,  0($sp)
	sw	$s1,  4($sp)
	sw	$s2,  8($sp)
	sw	$s3, 12($sp)
	sw	$ra, 16($sp)
	
	move	$s0, $a0
	addi	$a1, $a1, -1
	
	##########  Bubble Sort  ##########
	
	li	$s1, 0
	
externalLoop:

	move	$s0, $a0
	li	$s2, 0
	
	blt	$s1, $a1, internalLoop
	j	exitLoop
	
internalLoop:

	sub	$s3, $a1, $s1

	blt	$s2, $s3, cond
	j	exitInternalLoop
		
cond:

	lw	$s4, 0($s0)
	lw	$s5, 4($s0)
	
	bgt	$s4, $s5, swap
	j	next
	
swap:

	sw	$s5, 4($s0)
	sw	$s4, 0($s0)
	
	addi	$s0, $s0, 4
		
next:

	addi	$s2, $s2, 1
	j	internalLoop

exitInternalLoop:

	addi	$s1, $s1, 1
	j	externalLoop
	
exitLoop:

	
	###################################
	
	jal	minMax

	lw	$ra, 16($sp)
	lw	$s3, 12($sp)
	lw	$s2,  8($sp)
	lw	$s1,  4($sp)
	lw	$s0,  0($sp)
	addi	$sp, $sp,  20
	
	jr	$ra



minMax:
	addi	$sp, $sp, -8
	sw	$s0, 0($sp)
	sw	$s1, 4($sp)
	
	move	$s0, $a0
	move	$s2, $s0 
	
	la	$a0, maxPrompt
	li	$v0, 4
	syscall
	
	lw	$a0, 0($s2)
	li	$v0, 1
	syscall
	
	li	$v0, 4
	la	$a0, newLine
	syscall
	
	mul	$s2, $s2,   4
	add	$s0, $s0, $s1
	
	la	$a0, minPrompt
	li	$v0, 4
	syscall
	
	lw	$a0, 0($s0)
	li	$v0, 1
	syscall
	
	move	$a0, $s0
	
	lw	$s1, 4($sp)
	lw	$s0, 0($sp)
	addi	$sp, $sp,  8

	jr	$ra



	.data
colon:	.asciiz ":\t"
newLine:
	.asciiz "\n"
arraySizePrompt:
	.asciiz	"Enter a size for the array: "
elementPrompt:
	.asciiz "Element "
minPrompt:
	.asciiz "Min: "
maxPrompt:
	.asciiz	"Max: "	
