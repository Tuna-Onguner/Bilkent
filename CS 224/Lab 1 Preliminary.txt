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


# Part 1, Swaping array elements:


	.text
	la 	$s0, array		# $s0 = address of array

	li 	$v0, 4
    	la 	$a0, input_msg
    	syscall
	
	li 	$v0, 5
	syscall
	
	move	$s1, $v0		# $s1 = assigned size of array
	la	$t0, 0($s0)		# $t0 = temproray address of array
	
	beq	$s1, 0, size_is_0	# if size is assigned to zero
	
loop_save:
	li	$v0, 5
	syscall
	
	sw	$v0, 0($t0)
	
	addi	$t1, $t1, 1		# $t1 = temprorary loop variable
	addi	$t0, $t0, 4
	
	bne	$t1, $s1, loop_save
	
	li 	$v0, 4
    	la 	$a0, new_line
    	syscall
	
	li 	$v0, 4
    	la 	$a0, array_msg
    	syscall
    	
    	jal	print_array
    	
    	la	$t1, 0($s0)
    	addi	$t0, $t0, -4
    	
loop_swap:
	lw  	$t2, 0($t1)
	lw  	$t3, 0($t0)
	
	sw  	$t2, 0($t0)
	sw  	$t3, 0($t1)

	addi 	$t1, $t1,  4
	addi 	$t0, $t0, -4
	
	blt  	$t1, $t0, loop_swap
	
	li 	$v0, 4
    	la 	$a0, swapped_msg
    	syscall
    	
    	jal	print_array
	
	b 	end
	
size_is_0:
	li 	$v0, 4
    	la 	$a0, array_empty
    	syscall
	
end:
	li      $v0, 10
	syscall
	
	
print_array:
	move	$t0, $s0
    	move	$t1, $zero
    	
    	addi	$a1, $s1, -1

loop:
	lw	$a0, 0($t0)
	li	$v0, 1
	syscall
	
	bne	$a1, $t1, true
	b	endif
	
true:
	li 	$v0, 4
    	la 	$a0, comma
    	syscall

endif:
	addi	$t1, $t1, 1
	addi	$t0, $t0, 4
	
	bne	$t1, $s1, loop
	
	li 	$v0, 4
    	la 	$a0, new_line
    	syscall
    	
    	jr	$ra
	

	.data
array:	.space	80
comma:	.asciiz ", "
new_line:
	.asciiz "\n"
input_msg:	
	.asciiz "Enter a size for the array (max 20): "
array_msg:
	.asciiz "Array:\t   "
swapped_msg:
	.asciiz "Swapped:   "
array_empty:	
	.asciiz "Array is empty!"




#-----------------------------------------------------------#




# Part 2, Calculating an arithmetic expression:
	
	
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
	
	move	$t0, $s0
	
	blt	$s0, $s1, true
	b	loopBdivC
	
true:
	move	$t1, $zero
	b	endif

loopBdivC:
	sub	$t0, $t0, $s1
	addi	$t1, $t1,   1
	
	bge	$t0, $s1, loopBdivC
	
endif:
	mul	$t2, $s2, $s0
	add	$t2, $t2, $t1
	sub	$t2, $t2, $s1
	
	blt	$t2, 0, negModB
	
loopXmodB:
	sub	$t2, $t2, $s0
	bgt	$t2, $s0, loopXmodB
	
	b	end
	
negModB:
	add	$t2, $t2, $s0
	blt	$t2, 0, negModB
	
end:
	la	$a0, A
	li	$v0, 4
	syscall
	
	move	$a0, $t2
	
	li	$v0, 1
	syscall
	
	li	$v0, 10
	syscall
	
	
	.data
A:	.asciiz	"A = "
B:	.asciiz	"B: "
C:	.asciiz	"C: "
D:	.asciiz	"D: "

