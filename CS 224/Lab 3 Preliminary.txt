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


# Part 1: Merge Sorted Lists

#   *   *   *   *   *   *   *   *   *   *   Text Segment   *   *   *   *   *   *   *   *   *   *   #
	
	
	.text
	.globl	__main

__main:
	jal	createANewLinkedList
	
	move	$a0, $v0
	move	$a1, $v1
	
	move	$s0, $v0
	move	$s2, $v1
	
	jal	printLinkedList
	
	la	$a0, newLine
	
	li	$v0, 4
	syscall
	
	jal	createANewLinkedList
	
	move	$a0, $v0
	move	$a1, $v1
	
	move	$s1, $v0
	move	$s3, $v1
	
	jal	printLinkedList
	
	la	$a0, newLine
	
	li	$v0, 4
	syscall
	
	move	$a0, $s0
	move	$a1, $s1
	move	$a2, $s2
	move	$a3, $s3
	
	jal	mergeTwoSortedLinkedLists
	
	move	$a0, $v0
	move	$a1, $v1
	
	jal	printLinkedList

# Terminate
	li	$v0, 10
	syscall


#   *   *   *   *   *   *   *   *   *   *   *  ||  *   *   *   *   *   *   *   *   *   *   *   *   #


createANewLinkedList:
# Store s register values in the stack
	addi	$sp, $sp, -28	# allocate stack memory for s registers
	
	sw	$s0, 24($sp)
	sw	$s1, 20($sp)
	sw	$s2, 16($sp)
	sw	$s3, 12($sp)
	sw	$s4,  8($sp)
	sw	$s5,  4($sp)
	sw	$ra,  0($sp)	# Store the return addres for other function calls
	
	la	$a0, sizePrompt
	
	li	$v0, 4
	syscall
	
	li	$v0, 5
	syscall
	
	move	$s0, $v0	# $s0 -> size of the new linked list
	
	la	$a0, separatorLine
	
	li	$v0, 4
	syscall
	
	beqz	$s0, emptyLinkedList
	
	li	$s1, 1		# $s1 -> node counter
	
# Create the first node, the header
# Each node is 8 bytes
	li	$a0, 8
	li	$v0, 9
	syscall
	
	move	$s2, $v0	# $s2 -> header of the linked list
	move	$s3, $v0	# $s3 -> tail of the linked list
	
	la	$a0, valueNodePrompt
	
	li	$v0, 4
	syscall
	
	li	$v0, 5
	syscall
	
	move	$s4, $v0
	sw	$s4, 4($s2)	# Store the data value
	
addNewNode:
	beq	$s1, $s0, allDone
	addi	$s1, $s1, 1	# Increment node counter
	
	li	$a0, 8
	li	$v0, 9
	syscall
	
# Connect the this node to the lst node pointed by $s2
	sw	$v0, 0($s2)
	
	move	$s2, $v0	# $s2 now points to the new node
	
	la	$a0, valueNodePrompt
	
	li	$v0, 4
	syscall
	
	li	$v0, 5
	syscall
	
	move	$s4, $v0
	sw	$s4, 4($s2)	# Store the data value.
	
	j	addNewNode

allDone:
	la	$a0, separatorLine
	
	li	$v0, 4
	syscall
	
	sw	$zero, 0($s2)
	move	$v0, $s3
	move	$v1, $s0
	
	j	exitCreateANewLinkedList
	
emptyLinkedList:
	move	$v0, $zero
	move	$v1, $zero
	
exitCreateANewLinkedList:
# Load s register values from the stack
	lw	$ra,  0($sp)	# Reload the return address
	lw	$s5,  4($sp)
	lw	$s4,  8($sp)
	lw	$s3, 12($sp)
	lw	$s2, 16($sp)
	lw	$s1, 20($sp)
	lw	$s0, 24($sp)
	
	addi	$sp, $sp, +28	# deallocate the stack
	
# return
	jr	$ra


#   *   *   *   *   *   *   *   *   *   *   *  ||  *   *   *   *   *   *   *   *   *   *   *   *   #


printLinkedList:
# Store s register values in the stack
	addi	$sp, $sp, -28	# allocate stack memory for s registers
	
	sw	$s0, 24($sp)
	sw	$s1, 20($sp)
	sw	$s2, 16($sp)
	sw	$s3, 12($sp)
	sw	$s4,  8($sp)
	sw	$s5,  4($sp)
	sw	$ra,  0($sp)	#Store the return addres for other function calls
	
	move 	$s0, $a0	# $s0 -> Address of current node
	move	$s4, $a1
	
	li   	$s3, 0
	
	la	$a0, printedListPrompt
	
	li	$v0, 4
	syscall
	
printNextNode:
	beq	$s0, $zero, printedAll
				
	lw	$s1, 0($s0)	# $s1 -> Address of  next node
	lw	$s2, 4($s0)	# $s2 -> Data of current node
	addi	$s3, $s3, 1
	
# $s0 -> address of current node: print in hex
# $s1 -> address of next node: print in hex
# $s2 -> data field value of current node: print in decimal
	move	$a0, $s2	# $s2 -> Data of current node
	
	li	$v0, 1		
	syscall
	
	beq	$s4, $s3, noArrows
	
	la	$a0, listArrow
	
	li	$v0, 4
	syscall

noArrows:
# Now consider next node.
	move	$s0, $s1	# Consider next node
	j	printNextNode
	
printedAll:
	la	$a0, squareBracketRight
	
	li	$v0, 4
	syscall
	
	move	$a0, $s4
	
	li	$v0, 1
	syscall
	
	la	$a0, newLine
	
	li	$v0, 4
	syscall
	
# Load s register values from the stack
	lw	$ra,  0($sp)	# Reload the return address
	lw	$s5,  4($sp)
	lw	$s4,  8($sp)
	lw	$s3, 12($sp)
	lw	$s2, 16($sp)
	lw	$s1, 20($sp)
	lw	$s0, 24($sp)
	
	addi	$sp, $sp, +28	# deallocate the stack
	
# return
	jr	$ra


#   *   *   *   *   *   *   *   *   *   *   *  ||  *   *   *   *   *   *   *   *   *   *   *   *   #


mergeTwoSortedLinkedLists:
# function: mergeLinkLists
# $a0 -> header of the first link list
# $a1 -> header of the second link list
# $a2 -> size of the first link list
# $a3 -> size of the second link list
# $v0 -> header of the new (merged) link list
# $v1 -> size of the new (merged) link list

# Store s register values in the stack
	addi	$sp, $sp, -36	# allocate stack memory for s registers
	
	sw	$s0, 32($sp)
	sw	$s1, 28($sp)
	sw	$s2, 24($sp)
	sw	$s3, 20($sp)
	sw	$s4, 16($sp)
	sw	$s5, 12($sp)
	sw	$s6,  8($sp)
	sw	$s7,  4($sp)
	sw	$ra,  0($sp)	# Store the return addres for other function calls
	
	li	$s7, 0
	
	move	$s0, $a0	# $s0 -> header of the first link list
	move	$s1, $a1	# $s1 -> header of the second link list
	
	la	$a0, mergePerformedPrompt
	
	li	$v0, 4
	syscall
	
	beqz	$s0, firstListIsEmpty
	beqz	$s1, secondListIsEmpty
	
# Merge two fulled linked lists
	li	$a0, 8
	li	$v0, 9
	syscall
	
	move	$s2, $v0	# $s2 -> header of the linked list
	move	$s3, $v0	# $s3 -> tail of the linked list
	
	move	$a0, $s0
	move	$a1, $s1
	
	jal	computeNextNodes
	
	move	$s0, $a0
	move	$s1, $a1
	
	move	$s4, $v0
	
	sw	$s4, 4($s2)
	addi	$s7, $s7, 1
	
addNode:
	beqz	$s0, firstIsDone
	beqz	$s1, secondIsDone

	li	$a0, 8
	li	$v0, 9
	syscall
	
	sw	$v0, 0($s2)
	move	$s2, $v0	# $s2 -> header of the linked list
	
	move	$a0, $s0
	move	$a1, $s1
	
	jal	computeNextNodes
	
	move	$s0, $a0
	move	$s1, $a1
	
	move	$s4, $v0
	
	sw	$s4, 4($s2)
	addi	$s7, $s7, 1
	
	j	addNode
	
done:
	move	$v1, $s7
	move	$v0, $s3
	j	mergeIsDone
	
firstIsDone:
	beqz	$s1, done
	
	li	$a0, 8
	li	$v0, 9
	syscall
	
	sw	$v0, 0($s2)
	move	$s2, $v0
	
	lw	$s5, 0($s1)
	lw	$s4, 4($s1)
	sw	$s4, 4($s2)
	
	addi	$s7, $s7, 1
	move	$s1, $s5
	j	firstIsDone
	
secondIsDone:
	beqz	$s0, done
	
	li	$a0, 8
	li	$v0, 9
	syscall
	
	sw	$v0, 0($s2)
	move	$s2, $v0
	
	lw	$s6, 0($s0)
	lw	$s4, 4($s0)
	sw	$s4, 4($s2)
	
	addi	$s7, $s7, 1
	move	$s0, $s6
	j	secondIsDone
	
firstListIsEmpty:
	beqz	$s1, bothListsAreEmpty
	
	move	$v0, $s1
	move	$v1, $s3
	
	j	mergeIsDone
	
secondListIsEmpty:
	move	$v0, $s0
	move	$v1, $s2
	
	j	mergeIsDone

bothListsAreEmpty:
	move	$v0, $zero
	move	$v1, $zero

mergeIsDone:
	move	$a0, $s0
	move	$a1, $s1
	
# Load s register values from the stack
	lw	$ra,  0($sp)	# Reload the return address
	lw	$s7,  4($sp)
	lw	$s6,  8($sp)
	lw	$s5, 12($sp)
	lw	$s4, 16($sp)
	lw	$s3, 20($sp)
	lw	$s2, 24($sp)
	lw	$s1, 28($sp)
	lw	$s0, 32($sp)
	
	addi	$sp, $sp, +36	# deallocate the stack
	
# return
	jr	$ra
	
	
#   *   *   *   *   *   *   *   *   *   *   *  ||  *   *   *   *   *   *   *   *   *   *   *   *   #


computeNextNodes:
# Store s register values in the stack
	addi	$sp, $sp, -28	# allocate stack memory for s registers
	
	sw	$s0, 24($sp)
	sw	$s1, 20($sp)
	sw	$s2, 16($sp)
	sw	$s3, 12($sp)
	sw	$s4,  8($sp)
	sw	$s5,  4($sp)
	sw	$ra,  0($sp)	# Store the return addres for other function calls
	
	move	$s0, $a0
	move	$s1, $a1
		
	lw	$s2, 4($s0)
	lw	$s3, 4($s1)
	
	lw	$s4, 0($s0)
	lw	$s5, 0($s1)
	
	bgt	$s2, $s3, firstGreaterThanSecond
	beq	$s2, $s3, equal
	
	move	$v0, $s2
	move	$a0, $s4
	
	j	return
	
firstGreaterThanSecond:
	move	$v0, $s3
	move	$a1, $s5
	
	j	return
	
equal:
	move	$v0, $s3
	move	$a0, $s4
	move	$a1, $s5
	
return:
# Load s register values from the stack
	lw	$ra,  0($sp)	# Reload the return address
	lw	$s5,  4($sp)
	lw	$s4,  8($sp)
	lw	$s3, 12($sp)
	lw	$s2, 16($sp)
	lw	$s1, 20($sp)
	lw	$s0, 24($sp)
	
	addi	$sp, $sp, +28	# deallocate the stack
	
# return
	jr	$ra
	

#   *   *   *   *   *   *   *   *   *   *   Data Segment   *   *   *   *   *   *   *   *   *   *   #


	.data
mergePerformedPrompt:
	.asciiz	"Merge is performed successfully\n"
separatorLine:
	.asciiz	"-----------------------------\n"
sizePrompt:
	.asciiz	"Choose a size for the list: "
valueNodePrompt:
	.asciiz	"Enter a value for the node: "
squareBracketRight:
	.asciiz	"  ]  :  size = "
printedListPrompt:
	.asciiz	"The list:  [  "
listArrow:
	.asciiz "  ->  "
newLine:
	.asciiz	"\n"





# ========================================  End of Part 1  ======================================== #


# Part 2: Register Count

#   *   *   *   *   *   *   *   *   *   *   Text Segment   *   *   *   *   *   *   *   *   *   *   #


	.text
	.globl	__main
	
__main:
	la	$a0, registerPrompt
	li	$v0, 4
	syscall
	
	li	$v0, 5
	syscall
	
	move	$a0, $v0
	
	blt	$a0,  1, terminate
	bgt	$a0, 31, terminate
	
	jal	registerCounter
	
	move	$s0, $v0
	
	la	$a0, resultPrompt
	li	$v0, 4
	syscall
	
	move	$a0, $s0
	
	la	$v0, 1
	syscall
	
	la	$a0, newLine
	li	$v0, 4
	syscall
	
	j	__main
	
terminate:
	li	$v0, 10
	syscall
	
	
#   *   *   *   *   *   *   *   *   *   *   *  ||  *   *   *   *   *   *   *   *   *   *   *   *   #


registerCounter:
# Store s register values in the stack
	addi	$sp, $sp, -36	# allocate stack memory for s registers
	
	sw	$s0, 32($sp)
	sw	$s1, 28($sp)
	sw	$s2, 24($sp)
	sw	$s3, 20($sp)
	sw	$s4, 16($sp)
	sw	$s5, 12($sp)
	sw	$s6,  8($sp)
	sw	$s7,  4($sp)
	sw	$ra,  0($sp)	# Store the return addres for other function calls
	
	la	$s0, registerCounter	# Initial start
	la	$s1, end		# End point
	
	move	$s2, $a0		# Register number to be counted
	li	$s6, 0
	
next:
	bgt	$s0, $s1, exit
	
	lw	$s3, 0($s0)		# $s3 -> Unchanged value of $s0
	srl	$s4, $s3, 26		# $s4 -> opcode
	
	beq	$s4, 2, JTypeIns
	beq	$s4, 3, JTypeIns
	
	sll	$s5, $s3,  6
	srl	$s5, $s5, 27		# $s5 -> rs

	beq	$s5, $s2, incrementCountRS
	j	passRS
	
incrementCountRS:
	addi	$s6, $s6, 1
	
passRS:
	
	sll	$s5, $s3, 11
	srl	$s5, $s5, 27
	
	beq	$s5, $s2, incrementCountRT
	j	passRT
	
incrementCountRT:	
	addi	$s6, $s6, 1
	
passRT:
	beq	$s4, 0, RTypeIns
	j	JTypeIns

RTypeIns:
	sll	$s5, $s3, 16
	srl	$s5, $s5, 27
	
	beq	$s5, $s2, incrementCountRD
	j	JTypeIns
	
incrementCountRD:
	addi	$s6, $s6, 1
	
JTypeIns:
	addi	$s0, $s0, 4
	j	next
	
exit:
	move	$v0, $s6

# Load s register values from the stack
	lw	$ra,  0($sp)	# Reload the return address
	lw	$s7,  4($sp)
	lw	$s6,  8($sp)
	lw	$s5, 12($sp)
	lw	$s4, 16($sp)
	lw	$s3, 20($sp)
	lw	$s2, 24($sp)
	lw	$s1, 28($sp)
	lw	$s0, 32($sp)
	
	addi	$sp, $sp, +36	# deallocate the stack
	
end:
# return
	jr	$ra
	
	
#   *   *   *   *   *   *   *   *   *   *   Data Segment   *   *   *   *   *   *   *   *   *   *   #


	.data
newLine:
	.asciiz	"\n"
registerPrompt:
	.asciiz	"Choose a register number to count how many times it is used: "
resultPrompt:
	.asciiz	"Number of times the register appears is\t"





# =========================================  End of File  ========================================= #
