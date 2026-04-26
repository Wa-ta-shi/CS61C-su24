
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