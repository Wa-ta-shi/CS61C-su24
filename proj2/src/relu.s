# .import utils.s
.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
    # Prologue
    li t0 1 # length temp 1
    blt a1 t0 error # judge length bigger than 1

    mv t0 a0 # store pointer
    li t2 0 # current index

loop_start:
    bge t2 a1 loop_end
    
    lw t3 0(t0) # current num
    bge t3 zero skip # if current num smaller than 0
    
    sw zero 0(t0)

skip:
    addi t0 t0 4 # increase pointer
    addi t2 t2 1 # increase index
    j loop_start
    
loop_end:
    # Epilogue
    jr ra
    
error:
    li a0 36
    j exit
