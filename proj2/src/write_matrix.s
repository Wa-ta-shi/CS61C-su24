.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
#   - If you receive an fopen error or eof,
#     this function terminates the program with error code 27
#   - If you receive an fclose error or eof,
#     this function terminates the program with error code 28
#   - If you receive an fwrite error or eof,
#     this function terminates the program with error code 30
# ==============================================================================
write_matrix:

    # Prologue
    addi sp sp -40
    sw ra 0(sp)
    sw s0 4(sp)
    sw s1 8(sp)
    sw s2 12(sp)
    sw s3 16(sp)
    sw s4 20(sp)
    sw s5 24(sp)
    sw s6 28(sp)
    addi sp sp -8

    mv s0 a0  # filename
    mv s1 a1  # matrix pointer
    mv s2 a2  # rows
    mv s3 a3  # columns

    # 1. Open file
    mv a0 s0
    li a1 1   # "w" mode
    jal ra fopen
    li t0 -1
    beq a0 t0 error_open
    mv s4 a0  # file descriptor

    sw s2 0(sp)
    sw s3 4(sp)

    mv a0 s4
    mv a1 sp
    li a2 1
    li a3 4
    jal ra fwrite
    li t0 1
    bne a0 t0 error_write

    mv a0 s4
    addi a1 sp 4
    li a2 1
    li a3 4
    jal ra fwrite
    li t0 1
    bne a0 t0 error_write

    # 5. Write matrix data
    mv a0 s4
    mv a1 s1      # matrix data pointer
    mul a2 s2 s3  # number of elements
    li a3 4       # size of each element
    jal ra fwrite
    mul t0 s2 s3
    bne a0 t0 error_write

    # 6. Close file
    mv a0 s4
    jal ra fclose
    li t0 -1
    beq a0 t0 error_close

    addi sp sp 8

    # Epilogue
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    lw s2 12(sp)
    lw s3 16(sp)
    lw s4 20(sp)
    lw s5 24(sp)
    lw s6 28(sp)
    addi sp sp 40
    jr ra
    
error_open:
    li a0 27
    j cleanup_and_exit

error_write:
    li a0 30
    j cleanup_and_exit

error_close:
    li a0 28

cleanup_and_exit:
    addi sp sp 8     
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    lw s2 12(sp)
    lw s3 16(sp)
    lw s4 20(sp)
    lw s5 24(sp)
    lw s6 28(sp)
    addi sp sp 40
    j exit