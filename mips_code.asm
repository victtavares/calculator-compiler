	.text
	.globl main

main:
	li $a0, 1
	sw $a0, 0($sp)
	addiu $sp, $sp -4
	li $a0, 1
	lw $t1, 4($sp)
	addiu $sp, $sp, 4
	beq $a0, $t1, true_branch1
false_branch1:
	li $a0, 6
	li $vo, 1
	syscall
	b end_if
true_branch1:
	li $a0, 2
	sw $a0, 0($sp)
	addiu $sp, $sp -4
	li $a0, 2
	lw $t1, 4($sp)
	addiu $sp, $sp, 4
	beq $a0, $t1, true_branch2
true_branch2:
	li $a0, 5
	li $vo, 1
	syscall
	end_if2:
	end_if1:

li $v0, 10
syscall
