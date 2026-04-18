.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
    # Prologue
    li t0 1 # lenght temp 1
    blt a1 t0 error # judge length bigger than 1

    mv t0 a0 # store pointer
    lw t4 0(t0) # store current max num
    addi t0 t0 4 # pointer + 4
    li t2 1 # current index
    li a0 0 # return index

loop_start:
    bge t2 a1 loop_end
    lw t3 0(t0) # current num
    beq t3 t4 skip # equal to skip
    blt t3 t4 skip # smaller to skip
    mv a0 t2 # update return index
    mv t4 t3 # update max num
    
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