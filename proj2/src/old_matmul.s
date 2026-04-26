#matmul:
#
#    # Error checks
#    li t0 1 # temp 1
#    blt a1 t0 error
#    blt a2 t0 error
#    blt a4 t0 error
#    blt a5 t0 error
#    bne a2 a4 error
#
#    addi sp sp -20
#    sw ra 0(sp) # save address
#    sw s0 4(sp)
#    sw s1 8(sp)
#    sw s2 12(sp)
#    sw a6 16(sp)
#
#    mv s0 a0 # pointer of A
#    mv s1 a3
#    mv s2 a1 # save a1
#
#    li t0 0 # index of arr0 = i
#
#outer_loop_start:
#    li t1 0 # index of arr1 = j
#    bge t0 s2 outer_loop_end # i < n
#
#inner_loop_start:
#    bge t1 a5 inner_loop_end
#
#    mv a0 s0 # arr0
#    mul t2 t0 a2 # i * C_arr0
#    slli t2 t2 2 # * 4
#    add a0 a0 t2
#
#    mv a1 s1 # arr1
#    slli t2 t1 2 # j * 4
#    add a1 a1 t2 # arr1 + j
#
#    mv a2 a1
#    li a3 1 # stride
#    mv a4 a2 # stride
#
#    addi sp sp -24
#    sw t0 0(sp)
#    sw t1 4(sp)
#    sw a1 8(sp)
#    sw a5 12(sp)
#    sw a6 16(sp)
#    sw a2 20(sp)
#
#    jal ra dot # call dot
#
#    lw t0 0(sp)
#    lw t1 4(sp)
#    lw a1 8(sp)
#    lw a5 12(sp)
#    lw a6 16(sp)
#    lw a2 20(sp)
#    addi sp sp 24
#
#    sw a0 0(a6) # answer
#    addi a6 a6 4 # increase a6
#
#    addi t1 t1 1 # j++
#    j inner_loop_start
#
#inner_loop_end:
#    addi t0 t0 1 # increase t0 arr0 = i
#    j outer_loop_start
#
#outer_loop_end:
#    lw ra 0(sp) # restore ra
#    lw s0 4(sp)
#    lw s1 8(sp)
#    lw s2 12(sp)
#    lw a0 16(sp)
#    addi sp sp 20
#
#    # Epilogue
#    jr ra

#matmul:
#    # Error checks
#    li t0 1
#    blt a1 t0 error
#    blt a2 t0 error
#    blt a4 t0 error
#    blt a5 t0 error
#    bne a2 a4 error
#
#    addi sp sp -20
#    sw ra 0(sp)
#    sw s0 4(sp)
#    sw s1 8(sp)
#    sw s2 12(sp)
#    sw a6 16(sp)
#
#    mv s0 a0
#    mv s1 a3
#    mv s2 a1
#
#    li t0 0  # i = 0
#
#outer_loop_start:
#    li t1 0  # j = 0
#    bge t0 s2 outer_loop_end
#
#inner_loop_start:
#    bge t1 a5 inner_loop_end
#
#    # 计算m0当前行的指针
#    mv a0 s0
#    mul t2 t0 a2
#    slli t2 t2 2
#    add a0 a0 t2
#
#    # 计算m1当前列的指针
#    mv a1 s1
#    slli t2 t1 2
#    add a1 a1 t2
#
#    # 设置点积参数
#    mv a2 a2      # 元素个数 = m0的列数
#    li a3 1       # m0的步幅是1
#    mv a4 a5      # m1的步幅是m1的列数
#
#    addi sp sp -24
#    sw t0 0(sp)
#    sw t1 4(sp)
#    sw a1 8(sp)
#    sw a5 12(sp)
#    sw a6 16(sp)
#    sw a2 20(sp)
#
#    jal ra dot
#
#    lw t0 0(sp)
#    lw t1 4(sp)
#    lw a1 8(sp)
#    lw a5 12(sp)
#    lw a6 16(sp)
#    lw a2 20(sp)
#    addi sp sp 24
#
#    sw a0 0(a6)
#    addi a6 a6 4
#
#    addi t1 t1 1
#    j inner_loop_start
#
#inner_loop_end:
#    addi t0 t0 1
#    j outer_loop_start
#
#outer_loop_end:
#    lw ra 0(sp)
#    lw s0 4(sp)
#    lw s1 8(sp)
#    lw s2 12(sp)
#    lw a6 16(sp)
#    addi sp sp 20
#
#    jr ra
#