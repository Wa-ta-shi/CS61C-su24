.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the number of elements to use is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:
    li t4 1 # temp 1
    blt a2 t4 error1 # judge length bigger than 1
    blt a3 t4 error2
    blt a4 t4 error2
    
    li t4 4 # temp 4
    mul t0 t4 a3 # arr0 increase pointer num
    mul t1 t4 a4 # arr1
    li t2 0 # i = 0
    li t3 0 # answer = 0

loop_start:
    bge t2 a2 loop_end # judge i and length
    lw t4 0(a0) # current num
    lw t5 0(a1) # current num
    mul t6 t4 t5 # current * current
    add t3 t3 t6 # add to answer
    
    add a0 a0 t0 # increase pointer
    add a1 a1 t1
    addi t2 t2 1 # i++

loop_end:
    mv a0 t3 # mv answer
    jr ra

error1:
    li a0 36
    j exit

error2:
    li a0 37
    j exit