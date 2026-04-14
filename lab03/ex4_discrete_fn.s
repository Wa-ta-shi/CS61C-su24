.globl f # this allows other files to find the function f

.import utils.s

.data
nums:
    .word 6, 61, 17, -38, 19, 42, 5

.text
# f takes in two arguments:
# a0 is the value we want to evaluate f at
# a1 is the address of the "output" array (read the lab spec for more information).
# The return value should be stored in a0
f:
    # Your code here
    la t0 nums # t0 array address
    addi t1 a0 3 # +3
    slli t2 t1 2 # *4
    add t0 t0 t2 # add offset

    lw a0 0(t0) # load elements

    # This is how you return from a function. You'll learn more about this later.
    # This should be the last line in your program.
    jr ra
