main:
add $4, $2, $3
addi $10, $2, 4 # addi

lw $4, 4($2)
sw $5, 8($2)

# instruction extension
# lb
lb $6, 0($2)
lb $6, 1($2)
lb $6, 2($2)
lb $6, 3($2)

# lbu
lbu $7, 0($2)
lbu $7, 1($2)
lbu $7, 2($2)
lbu $7, 3($2)

# lh
lh $8, 0($2)
lh $8, 2($2)

# lhu
lhu $9, 0($2)
lhu $9, 2($2)

sub $2, $4, $3
or $2, $4, $3
ori $2, $4, 4 # ori

and $2, $4, $3
slt $2, $4, $3
beq $3, $3, equ
bne $3, $0, ine # bne
lw $2, 0($3)

equ:
beq $3, $4, exit
sw $2, 0($3)

ine:
add $13, $11, $12

exit:
j main
