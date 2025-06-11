.text
.globl main

main:
li $a0, 4
jal fib
add $a0, $v0, $0
li $v0, 1
syscall

li $v0, 10
syscall

fib:
addi $sp, $sp, -12
sw $ra, 8($sp)
sw $s1, 4($sp)
sw $a0, 0($sp)
slti $t0, $a0, 2
beq $t0, $0, L1
add $v0, $a0, $0
j exit

L1:
addi $a0, $a0, -1
jal fib
add $s1, $v0, $0
addi $a0, $a0, -1
jal fib
add $v0, $v0, $s1

exit:
lw $ra, 8($sp)
lw $s1, 4($sp)
lw $a0, 0($sp)
addi $sp, $sp, 12
jr $ra
