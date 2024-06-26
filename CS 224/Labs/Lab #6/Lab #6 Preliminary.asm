#
# Bilkent University
# Fall 2022-2023
#
# CS 224: Computer Organization
# Lab 6
#
# Deniz Tuna Onguner
# Bilkent ID: 22001788
#


# =============== text segment =============== #

	.text
	.globl __start
	
# $s0: half matrix size (N)
# $s1: full matrix size (N * N)
# $s2: array size (2(N * N) bytes)
# $s3: array address
	
__start:
# (Part 1): Ask for the matrix size
	la	$a0, matrixSizePrompt
	li	$v0, 4
	syscall

# Get the half matrix size
	li	$v0, 5
	syscall

	move	$s0, $v0	# keep the half matrix size in $s0
	
# Calculate the full matrix size
	mul	$s1, $s0, $s0	# keep the full matrix size in $s1
	
# Calculate the array size
	mul	$s2, $s1, 2	# keep the array size in $s2
	
# (Part 2): Allocate the array	
	move	$a0, $s2
	
	li	$v0, 9
	syscall
	
	move	$s3, $v0	# keep the array address in $s3
	
# Fill the array
	move	$t0, $s3	# $t0: copy address of the array
	li	$t1, 1		# $t1: value tracker for the array filling
	
fill:
	sh	$t1, 0($t0)
	addi	$t0, $t0, 2
	addi	$t1, $t1, 1
	ble	$t1, $s1, fill
	
# (Part 3): Display the value in the desired index
	la	$a0, newLine
	li	$v0, 4
	syscall

loop:
	li	$v0, 4
	la	$a0, desiredIndexPrompt
	syscall
	
# Print prompt for row index
	la	$a0, desiredRowPrompt
	syscall
	
# Get desired row index
	li	$v0, 5
	syscall
	
	move	$t0, $v0	# $t0: desired row index
	ble	$t0, 0, exitLoop
	
# Print prompt for column index
	la	$a0, desiredColPrompt
	li	$v0, 4
	syscall
	
# Get desired column index
	li	$v0, 5
	syscall
	
	move	$t1, $v0	# $t1: desired column index
	ble	$t1, 0, exitLoop
	
# Calculate row index: i
	subi	$t0, $t0, 1
	mul	$t0, $t0, 2
	mul	$t0, $t0, $s0	# $t0: i
	
# Calculate column index: j
	subi	$t1, $t1, 1
	mul	$t1, $t1, 2	# $t1: j
	
# Calculate real index in the array
	add	$t0, $t0, $t1	# $t0: real index
	move	$t1, $s3	# $t1: copy address of the array
	
	add	$t0, $t1, $t0	# $t0: real address of the desired index
	
# Print the propmt
	la	$a0, displayElementPrompt
	li	$v0, 4
	syscall
	
# Load the element
	lh	$a0, 0($t0)
	
# Print the value
	li	$v0, 1
	syscall
	
	la	$a0, newLine
	li	$v0, 4
	syscall
	
	j	loop

exitLoop:
# (Part 4): Sum of rows
	move	$t0, $s3
	li	$t1, 0
	li	$t2, 0
	
	la	$a0, newLine
	li	$v0, 4
	syscall
	
sumRows:
	lh	$t3, 0($t0)
	
	addi	$t0, $t0, 2
	addi	$t2, $t2, 1
	addi	$t4, $t4, 1
	
	add	$t1, $t1, $t3
	bgt	$t4, $s1, exitSumRow
	beq	$t2, $s0, row
	
	j	sumRows
	
row:	
	la	$a0, displayRowSumPrompt
	li	$v0, 4
	syscall
	
	move	$a0, $t1
	add	$t5, $t5, $t1
	
	li	$v0, 1
	syscall
	
	la	$a0, newLine
	li	$v0, 4
	syscall
	
	li	$t1, 0
	li	$t2, 0
	
	j	sumRows
	
exitSumRow:
	la	$a0, sumOfAllRowsPrompt
	li	$v0, 4
	syscall
	
	move	$a0, $t5
	
	li	$v0, 1
	syscall
	
	la	$a0, newLine
	li	$v0, 4
	syscall

# (Part 5): Sum of columns
	mul	$t6, $s0, 2
	move	$t0, $s3
	
	li	$t1, 0
	li	$t2, 0
	li	$t3, 0
	li	$t4, 0
	li	$t5, 0
	
	la	$a0, newLine
	li	$v0, 4
	syscall
	
sumCols:
	lh	$t3, 0($t0)
	
	add	$t0, $t0, $t6
	addi	$t2, $t2, 1
	addi	$t4, $t4, 1
	
	add	$t1, $t1, $t3
	bgt	$t4, $s1, exitSumCol
	beq	$t2, $s0, col
	
	j	sumCols
	
col:
	la	$a0, displayColSumPrompt
	li	$v0, 4
	syscall
	
	move	$a0, $t1
	add	$t5, $t5, $t1
	
	li	$v0, 1
	syscall
	
	la	$a0, newLine
	li	$v0, 4
	syscall
	
	li	$t1, 0
	li	$t2, 0
	move	$t0, $s3
	add	$t7, $t7, 2
	add	$t0, $t0, $t7
	
	j	sumCols
	
exitSumCol:
	la	$a0, sumOfAllColsPrompt
	li	$v0, 4
	syscall
	
	move	$a0, $t5
	
	li	$v0, 1
	syscall
	
	la	$a0, newLine
	li	$v0, 4
	syscall
	
# Exit program
	li	$v0, 10
	syscall
	
	
# =============== data segment =============== #

	.data
matrixSizePrompt:
	.asciiz "Matrix size: "
desiredIndexPrompt:
	.asciiz "Displaying value at desired index\n"
desiredRowPrompt:
	.asciiz	"row: "
desiredColPrompt:
	.asciiz "col: "
displayElementPrompt:
	.asciiz	"= "
displayRowSumPrompt:
	.asciiz	"sum of row = "
sumOfAllRowsPrompt:
	.asciiz "Sum by rows: "
displayColSumPrompt:
	.asciiz	"sum of column = "
sumOfAllColsPrompt:
	.asciiz "Sum by columns: "
newLine:
	.asciiz	"\n"

# End program
