.data 
a: .word 0
b: .word 0
c: .space 160

.text
main:

li $v0, 5
syscall
sw $v0, a

li $v0, 5
syscall
sw $v0, b

lw $t0, a
lw $t1, b
mul $t2, $t0, $t1 # a*b
add $t3, $t0, $t1 # a+b

la $t4, c
sll $t3, $t3, 2 # 4*(a+b)
add $t5, $t3, $t4 # address of c[a+b]
sw $t2, 0($t5)

add $a0, $0, $t5 
li $v0, 34 # display the hexadecimal address of c[a+b]
syscall

li $a0, 10
li $v0, 11 # new line
syscall

add $a0, $0, $t2
li $v0, 1 # display the value of c[a+b]
syscall

li $v0, 10
syscall