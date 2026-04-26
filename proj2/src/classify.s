.globl classify

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
.globl classify

.text
classify:
    # Prologue
    addi sp sp -72
    sw ra 0(sp)
    sw s0 4(sp)
    sw s1 8(sp)
    sw s2 12(sp)
    sw s3 16(sp)
    sw s4 20(sp)
    sw s5 24(sp)
    sw s6 28(sp)
    sw s7 32(sp)

    # Check argc == 5
    li t0 5
    bne a0 t0 error_args

    # Save arguments
    mv s0 a0        # argc
    mv s1 a1        # argv
    mv s2 a2        # silent mode

    # Read pretrained m0
    lw a0 4(s1)     # argv[1] - m0 path
    addi a1 sp 36   # m0.rows
    addi a2 sp 40   # m0.cols
    call read_matrix
    mv s3 a0        # m0 pointer

    # Read pretrained m1
    lw a0 8(s1)     # argv[2] - m1 path
    addi a1 sp 44   # m1.rows
    addi a2 sp 48   # m1.cols
    call read_matrix
    mv s4 a0        # m1 pointer

    # Read input matrix
    lw a0 12(s1)    # argv[3] - input path
    addi a1 sp 52   # input.rows
    addi a2 sp 56   # input.cols
    call read_matrix
    mv s5 a0        # input pointer

    # Allocate memory for h = matmul(m0, input)
    lw t0 36(sp)    # m0.rows
    lw t1 56(sp)    # input.cols
    sw t0 60(sp)    # h.rows = m0.rows
    sw t1 64(sp)    # h.cols = input.cols
    mul a0 t0 t1
    slli a0 a0 2    # bytes = elements * 4
    call malloc
    beq a0 zero error_malloc
    mv s6 a0        # h pointer

    # Compute h = matmul(m0, input)
    mv a0 s3        # m0
    lw a1 36(sp)    # m0.rows
    lw a2 40(sp)    # m0.cols
    mv a3 s5        # input
    lw a4 52(sp)    # input.rows
    lw a5 56(sp)    # input.cols
    mv a6 s6        # h
    call matmul

    # Compute h = relu(h)
    mv a0 s6        # h
    lw t0 60(sp)    # h.rows
    lw t1 64(sp)    # h.cols
    mul a1 t0 t1    # total elements
    call relu

    # Allocate memory for o = matmul(m1, h)
    lw t0 44(sp)    # m1.rows
    lw t1 64(sp)    # h.cols
    sw t0 68(sp)    # o.rows = m1.rows
    sw t1 72(sp)    # o.cols = h.cols
    mul a0 t0 t1
    slli a0 a0 2    # bytes = elements * 4
    call malloc
    beq a0 zero error_malloc
    mv s7 a0        # o pointer

    # Compute o = matmul(m1, h)
    mv a0 s4        # m1
    lw a1 44(sp)    # m1.rows
    lw a2 48(sp)    # m1.cols
    mv a3 s6        # h
    lw a4 60(sp)    # h.rows
    lw a5 64(sp)    # h.cols
    mv a6 s7        # o
    call matmul

    # Write output matrix o
    lw a0 16(s1)    # argv[4] - output path
    mv a1 s7        # o
    lw a2 68(sp)    # o.rows
    lw a3 72(sp)    # o.cols
    call write_matrix

    # Compute and return argmax(o)
    mv a0 s7        # o
    lw t0 68(sp)    # o.rows
    lw t1 72(sp)    # o.cols
    mul a1 t0 t1    # total elements
    call argmax
    mv s0 a0        # save result

    # Print result if not silent
    bne s2 zero skip_print
    mv a0 s0
    call print_int
    li a0 '\n'
    call print_char

skip_print:
    # Free allocated memory
    mv a0 s3
    call free
    mv a0 s4
    call free
    mv a0 s5
    call free
    mv a0 s6
    call free
    mv a0 s7
    call free

    # Return result
    mv a0 s0

    # Epilogue
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    lw s2 12(sp)
    lw s3 16(sp)
    lw s4 20(sp)
    lw s5 24(sp)
    lw s6 28(sp)
    lw s7 32(sp)
    addi sp sp 72
    jr ra

error_args:
    li a0 31
    j exit

error_malloc:
    li a0 26
    j exit