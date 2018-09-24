.data
n: .word 8

.text
main:
    la t0, n
    lw a0, 0(t0)			#a0 is the argument of the function fib
    jal ra, factorial
    
    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result
    
    addi a0, x0, 10
    ecall # Exit

factorial:
    # YOUR CODE HERE
    mv t0, a0
loop:
	beq t0, x0, done
	mul t1, t1,t0
	addi t0,t0,-1
loop

done:

	jr ra
	#while(n!=0):
	# 