# .import dot.s
.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
#   d = matmul(m0, m1)
# Arguments:
#   a0 (int*)  is the pointer to the start of m0
#   a1 (int)   is the # of rows (height) of m0
#   a2 (int)   is the # of columns (width) of m0
#   a3 (int*)  is the pointer to the start of m1
#   a4 (int)   is the # of rows (height) of m1
#   a5 (int)   is the # of columns (width) of m1
#   a6 (int*)  is the pointer to the the start of d
# Returns:
#   None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 38
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 38
# =======================================================
matmul:

    # Error checks
    li t0 1 # temp 1
    blt a1 t0 error
    blt a2 t0 error
    blt a4 t0 error
    blt a5 t0 error
    bne a2 a4 error
    
    addi sp sp -16
    sw ra 0(sp) # save address
    sw s0 4(sp)
    sw s1 8(sp)
    sw a6 12(sp)
    
    mv s0 a0 # pointer of A
    mv s1 a3

    li t0 0 # index of arr0 = i
    
outer_loop_start:
    li t1 0 # index of arr1 = j
    bge t0 a1 outer_loop_end # i < n
    
inner_loop_start:
    bge t1 a5 inner_loop_end
    
    mv a0 s0 # arr0
    mul t2 t0 a2 # i * C_arr0
    slli t2 t2 2 # * 4
    add a0 a0 t2
    
    mv a1 s1 # arr1
    slli t2 t1 2 # j * 4
    add a1 a1 t2 # arr1 + j

    # a2 already
    li a3 1 # stride
    mv a4 a5 # stride
    
    addi sp sp -24
    sw t0 0(sp)
    sw t1 4(sp)
    sw a1 8(sp)
    sw a5 12(sp)
    sw a6 16(sp)
    sw a2 20(sp)
    
    jal ra dot # call dot
    
    lw t0 0(sp)
    lw t1 4(sp)
    lw a1 8(sp)
    lw a5 12(sp)
    lw a6 16(sp)
    lw a2 20(sp)
    addi sp sp 24
    
    sw a0 0(a6) # answer
    addi a6 a6 4 # increase a6
    
    addi t1 t1 1 # j++
    j inner_loop_start

inner_loop_end:
    addi t0 t0 1 # increase t0 arr0 = i
    j outer_loop_start

outer_loop_end:
    lw ra 0(sp) # restore ra
    lw s0 4(sp)
    lw s1 8(sp)
    lw a0 12(sp)
    addi sp sp 16
    
    # Epilogue
    jr ra

error:
    li a0 38
    j exit