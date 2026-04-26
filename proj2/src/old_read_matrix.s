

    # Prologue
    addi sp sp -32
    sw ra 0(sp)
    sw s0 4(sp)
    sw s1 8(sp)
    sw s2 12(sp)
    sw s3 16(sp)
    sw s4 20(sp)
    sw s5 24(sp)
    sw s6 28(sp)

    mv s0 a0   # 保存文件名指针
    mv s1 a1   # 保存行数指针地址
    mv s2 a2   # 保存列数指针地址

    # 打开文件
    mv a0 s0   # 文件名
    li a1 0    # 只读模式
    jal ra fopen
    li t0 -1
    beq a0 t0 error_open
    mv s3 a0   # 保存文件描述符

    # 读取行数
    mv a0 s3   # 文件描述符
    mv a1 s1   # 直接读取到行数指针指向的位置
    li a2 4    # 读取4字节
    jal ra fread
    li t0 4
    bne a0 t0 error_read

    # 读取列数
    mv a0 s3
    mv a1 s2   # 直接读取到列数指针指向的位置
    li a2 4
    jal ra fread
    li t0 4
    bne a0 t0 error_read

    # 加载行数和列数的值到寄存器
    lw t0 0(s1)    # 行数
    lw t1 0(s2)    # 列数

    # 保存行数和列数到saved registers
    mv s4 t0       # s4 = 行数
    mv s5 t1       # s5 = 列数

    # 计算矩阵元素总数
    mul t0 s4 s5   # t0 = 行数 × 列数 = 元素总数
    mv s6 t0       # 保存元素总数到s6
    slli a0 t0 2   # 每个int4字节，所以乘以4
    jal ra malloc
    beq a0 x0 error_malloc
    mv s0 a0       # 保存矩阵内存地址到s0

    # 读取矩阵数据
    mv a0 s3       # 文件描述符
    mv a1 s0       # 缓冲区地址
    slli a2 s6 2   # 字节数 = 元素数 * 4（使用s6）
    jal ra fread

    # 检查读取的字节数
    slli t0 s6 2   # 字节数 = 元素数 * 4
    bne a0 t0 error_read

    # 关闭文件
    mv a0 s3
    jal ra fclose
    li t0 -1
    beq a0 t0 error_close

    # 设置返回值
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
    addi sp sp 32

    jr ra

error_malloc:
    li a0 26
    j exit

error_open:
    li a0 27
    j exit

error_close:
    li a0 28
    j exit

error_read:
    li a0 29
    j exit