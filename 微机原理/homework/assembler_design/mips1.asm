.data
A: .half 1,2,3,4,5,6

.data
B: .word 11,22,33,44

.text
lui $s0, 34
srl $s0, $s0, 16
lui $s1, 68
srl $s1, $s1, 16
la $s2, A
la $s3, B
add $t1, $s0, $s1
lw $t0, 12($s3)
add $t1, $t1, $t0

lh $t3, 4($s2)
lw $t4, 4($s3)
add $t2, $t3, $t4
add $t2, $t2, $s0