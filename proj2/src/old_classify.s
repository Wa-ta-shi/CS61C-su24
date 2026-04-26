.text
# =====================================
# COMMAND LINE ARGUMENTS
# =====================================
# Args:
#   a0 (int)        argc
#   a1 (char**)     argv
#   a1[1] (char*)   pointer to the filepath string of m0
#   a1[2] (char*)   pointer to the filepath string of m1
#   a1[3] (char*)   pointer to the filepath string of input matrix
#   a1[4] (char*)   pointer to the filepath string of output file
#   a2 (int)        silent mode, if this is 1, you should not print
#                   anything. Otherwise, you should print the
#                   classification and a newline.
# Returns:
#   a0 (int)        Classification
# Exceptions:
#   - If there are an incorrect number of command line args,
#     this function terminates the program with exit code 31
#   - If malloc fails, this function terminates the program with exit code 26
#
# Usage:
#   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
classify:
    addi sp sp -52
    sw ra 0(sp)
    sw s0 4(sp)
    sw s1 8(sp)
    sw s2 12(sp)
    sw s3 16(sp)
    sw s4 20(sp)
    sw s5 24(sp)
    sw s6 28(sp)
    sw s7 32(sp)
    sw s8 36(sp)
    sw s9 40(sp)
    sw s10 44(sp)
    sw s11 48(sp)

    mv s0 a0
    mv s1 a1
    mv s2 a2

    li t0 5
    bne a0 t0 error_num
    # Read pretrained m0
    addi a0 s1 4
    addi sp sp -8
    mv a1 sp
    addi a2 sp 4
    jal ra read_matrix
    mv s3 a1
    mv s4 a2
    mv s5 a0

    # Read pretrained m1
    addi a0 s1 8
    addi sp sp -8
    mv a1 sp
    addi sp sp 4
    mv a2 sp
    jal ra read_matrix
    mv s6 a1
    mv s7 a2
    mv s8 a0

    # Read input matrix
    addi a0 s1 12
    addi sp sp -8
    mv a1 sp
    addi sp sp 4
    mv a2 sp
    jal ra read_matrix
    mv s9 a1
    mv s10 a2
    mv s11 a0

    # Compute h = matmul(m0, input)
    # first malloc
    mul a0 s4 s9
    jal ra malloc

    beq a0 x0 error_malloc
    mv a6 a0 # !!! save

    mv a0 s5
    mv a1 s3
    mv a2 s4
    mv a3 s11
    mv a4 s9
    mv a5 s10
    jal ra matmul

    # Compute h = relu(h)
    mv a0 a6
    mul a1 s4 s9
    jal ra relu

    mv s3 a0 # cover m0

    # Compute o = matmul(m1, h)
    mul a0 s7 s9
    jal ra malloc

    beq a0 x0 error_malloc
    mv a6 a0

    mv s4 a0 # cover m0

    mv a0 s8
    mv a1 s6
    mv a2 s7
    mv a3 s11
    mv a4 s9
    mv a5 s10
    jal ra matmul

    # Write output matrix o
    addi a0 s1 16
    mv a1 a6
    mv a2 s7
    mv a3 s9
    jal ra write_matrix

    # Compute and return argmax(o)
    mv a0 a1
    mul a1 s7 s9
    jal ra argmax
    mv s5 a0 # save answer

    bne s2 x0 ending

    mv a0 s5
    jal ra print_int

    li a0 '\n'
    jal ra print_char


    # If enabled, print argmax(o) and newline
ending:
    mv a0 s5
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    lw s2 12(sp)
    lw s3 16(sp)
    lw s4 20(sp)
    lw s5 24(sp)
    lw s6 28(sp)
    lw s7 32(sp)
    lw s8 36(sp)
    lw s9 40(sp)
    lw s10 44(sp)
    lw s11 48(sp)
    addi sp sp 52
    jr ra

error_malloc:
    li a0 26
    j cleanup_and_exit

error_num:
    li a0 31

cleanup_and_exit:
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    lw s2 12(sp)
    lw s3 16(sp)
    lw s4 20(sp)
    lw s5 24(sp)
    lw s6 28(sp)
    lw s7 32(sp)
    lw s8 36(sp)
    lw s9 40(sp)
    lw s10 44(sp)
    lw s11 48(sp)
    addi sp sp 52

    j exit
