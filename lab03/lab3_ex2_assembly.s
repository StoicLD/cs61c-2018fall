.data
source:
    .word   3
    .word   1
    .word   4
    .word   1
    .word   5
    .word   9
    .word   0
dest:
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0

.text
main:
    addi t0, x0, 0
    la t1, source
    la t2, dest
loop:
    slli t3, t0, 2      #t3 represent the index of array
    add t4, t1, t3      #t4 is the index of array source
    lw t5, 0(t4)        #
    beq t5, x0, exit    #t5, the value of source[k]
    add t6, t2, t3      # t6 is the index of dest
    sw t5, 0(t6)        #dest[k] = source[k];  t6 is the index of 
    addi t0, t0, 1
    jal x0, loop  
exit:
    addi a0, x0, 10
    add a1, x0, x0
    ecall # Terminate ecall