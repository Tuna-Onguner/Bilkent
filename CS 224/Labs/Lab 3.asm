#
# Bilkent University
# Fall 2022-2023
#
# CS 224: Computer Organization
# Lab 3
#
# Deniz Tuna Onguner
# Bilkent ID: 22001788
#


# Part 1: Recursive Division

#   *   *   *   *   *   *   *   *   *   *   Text Segment   *   *   *   *   *   *   *   *   *   *   #


	.text
	.globl	__main
	
__main:
	la	$a0, dividentPrompt
	li	$v0, 4
	syscall
	
	li	$v0, 5
	syscall
	
	move	$s0, $v0
	ble	$s0, 0, exit
	
	la	$a0, divisorPrompt
	li	$v0, 4
	syscall
	
	li	$v0, 5
	syscall
	
	move	$a0, $s0
	move	$a1, $v0
	
	ble	$a1, 0, exit
	
	jal	recursiveDivider
	
	move	$s0, $v0
	
	la	$a0, resultPrompt
	li	$v0, 4
	syscall
	
	move	$a0, $s0
	
	li	$v0, 1
	syscall
	
	la	$a0, newLine
	li	$v0, 4
	syscall
	
	j	__main
	
exit:
	li	$v0, 10
	syscall


#   *   *   *   *   *   *   *   *   *   *   *  ||  *   *   *   *   *   *   *   *   *   *   *   *   #


recursiveDivider:
# Store s register values in the stack
	addi	$sp, $sp, -16	# allocate stack memory for s registers

	sw	$s0, 12($sp)
	sw	$a0,  8($sp)
	sw	$a1,  4($sp)
	sw	$ra,  0($sp)	# Store the return addres for other function calls
	
	bge	$a0, $a1, else
	
	addi	$sp, $sp, +16	# deallocate the stack
	addi	$v0, $zero, 0
	
# return
	jr	$ra
	
else:
	sub	$a0, $a0, $a1
	
	jal	recursiveDivider

	lw	$ra,  0($sp)
	lw	$a1,  4($sp)
	lw	$a0,  8($sp)
	lw	$s0, 12($sp)
	
	addi	$v0, $v0, 1
	
	addi	$sp, $sp, +16	# deallocate the stack
	
# return
	jr	$ra
	
	
#   *   *   *   *   *   *   *   *   *   *   Data Segment   *   *   *   *   *   *   *   *   *   *   #


	.data
dividentPrompt:
	.asciiz	"Enter a number to divide: "
divisorPrompt:
	.asciiz	"Enter the divisor: "
resultPrompt:
	.asciiz	"Result = "
newLine:
	.asciiz	"\n"





# ========================================  End of Part 1  ======================================== #


# Part 2: Display Reverse Order Recursive

#   *   *   *   *   *   *   *   *   *   *   Text Segment   *   *   *   *   *   *   *   *   *   *   #


	.text
	.globl	__main
	
__main:
	li	$v0, 10
	syscall
	
	
#   *   *   *   *   *   *   *   *   *   *   *  ||  *   *   *   *   *   *   *   *   *   *   *   *   #


displayLinkedListInReverseOrderRecursive:
# Store s register values in the stack
	addi	$sp, $sp, -16	# allocate stack memory for s registers

	sw	$s0, 12($sp)
	sw	$s1,  8($sp)
	sw	$s2,  4($sp)
	sw	$ra,  0($sp)	# Store the return addres for other function calls

	bne	$a0, 0, else
	
	addi	$sp, $sp, +16

	jr	$ra

else:
	lw	$s0, 4($a0)
	lw	$a0, 0($a0)
	
	jal	displayLinkedListInReverseOrderRecursive
	
	move	$s1, $a0
	move	$a0, $s0
	
	li	$v0, 1
	syscall
	
	la	$a0, blank
	
	li	$v0, 4
	syscall

	move	$a0, $s1

	lw	$ra,  0($sp)
	lw	$s2,  4($sp)
	lw	$s1,  8($sp)
	lw	$s0, 12($sp)
		
	addi	$sp, $sp, +16	# deallocate the stack
	
	jr	$ra
	
	
#   *   *   *   *   *   *   *   *   *   *   Data Segment   *   *   *   *   *   *   *   *   *   *   #


	.data
blank:	.asciiz	"  "





# =========================================  End of File  ========================================= #
