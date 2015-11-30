	.text
	.globl main

main:
	sw $a0, x
	li $vo, 1
	syscall

li $v0, 10
syscall
