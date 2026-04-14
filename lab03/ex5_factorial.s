.globl factorial

.data
n: .word 0

.text
# Don't worry about understanding the code in main
# You'll learn more about function calls in lecture soon
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

# factorial takes one argument:
# a0 contains the number which we want to compute the factorial of
# The return value should be stored in a0
factorial:
    # YOUR CODE HERE
    li t1 1 # answer
    li t2 0 # count
    li t3 1 # current num
    
loop_start:
    bge t2 a0 loop_end
    mul t1 t1 t3 # answer *
    addi t2 t2 1 # count +
    addi t3 t3 1 # current num +
    j loop_start
    
loop_end:
    mv a0 t1 # give answer
    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
