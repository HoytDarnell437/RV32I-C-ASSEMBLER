                # this file is intended to test the functionality of a riscv32i isa cpu.
                # it will send instrcutions and if completed properly it will continue if done incorrectly it will produce a flag and go into an infinite loop.
                # Designer: Hoyt Darnell
                # Date: May 7th, 2026
                
.text
                
                # load immediates
                li s10, 2049
                
                # INSTRUCTION        | register value on fail
                
                # Shift left logical | x26 = 1
lSLL:  
                li s0, 8
                li s1, 1
                sll t1, s1, s0
                li t0, 256
                beq t0, t1, lSLLI
                j fail
                
                # Shift left logical imm | x26 = 2
lSLLI:  
                addi s10, s10, 1
                slli t0, s1, 8
                li t1, 256
                beq t1, t0, lSRL
                j fail
                
                # Shift right logical | x26 = 3
lSRL:  
                addi s10, s10, 1
                li s1, 256
                li t2, 8
                srl t1, s1, t2
                li t0, 1
                beq t1, t0, lSRLI
                j fail
                
                # Shift right logical imm | x26 = 4
lSRLI:  
                addi s10, s10, 1
                li s1, 256
                srli t0, s1, 8
                li t1, 1
                beq t1, t0, lSRA
                j fail
                
                # Shift right arrithmetic | x26 = 5
lSRA:  
                addi s10, s10, 1
                li s1, -5
                li t2, 1
                sra t1, s1, t2
                li t0, -3
                beq t1, t0, lSRAI
                j fail
                
                # Shift right arrithmetic imm | x26 = 6
lSRAI:  
                addi s10, s10, 1
                li s1, -5
                srai t0, s1, 1
                li t1, -3
                beq t1, t0, lADD
                j fail
                
                # addition | x26 = 7
lADD:  
                addi s10, s10, 1
                li s1, -5
                li s0, 24
                add t0, s1, s0
                li t1, 19
                beq t1, t0, lADDI
                j fail
                
                # addition imm | x26 = 8
lADDI:  
                addi s10, s10, 1
                li s1, -5
                addi t0, s1, 24
                li t1, 19
                beq t1, t0, lSUB
                j fail
                
                # subtraction | x26 = 9
lSUB:  
                addi s10, s10, 1
                li s1, -5
                li s0, 24
                li t0, -29
                sub t1, s1, s0
                beq t1, t0, lLUI
                j fail
                
                # load upper immediate | x26 = 10
lLUI:  
                addi s10, s10, 1
                li t0, 4096
                lui t1, 1 
                beq t1, t0, lAUIPC
                j fail
                
                # add upper imm to pc | x26 = 11
lAUIPC:  
                addi s10, s10, 1
here:  
                auipc t1, 0
                j lXOR
                
                # exclusive or | x26 = 12
lXOR:  
                addi s10, s10, 1
                li t0, 0x4 # 0b100
                li t1, 0x6 # 0b110
                li t2, 0x2 # 0b010
                xor t3, t1, t0 
                beq t3, t2, lXORI
                j fail
                
                # exclusive or imm | x26 = 13
lXORI:  
                addi s10, s10, 1
                li t0, 0x4 # 0b100
                li t2, 0x2 # 0b010
                xori t3, t0, 0x6 # 0b110 
                beq t3, t2, lOR
                j fail
                
                # or | x26 = 14
lOR:  
                addi s10, s10, 1
                li t0, 0x4 # 0b100
                li t1, 0x6 # 0b110
                li t2, 0x6 # 0b110
                or t3, t1, t0
                beq t3, t2, lORI
                j fail
                
                # or imm | x26 = 15
lORI:  
                addi s10, s10, 1
                li t0, 0x4 # 0b100
                li t2, 0x6 # 0b110
                ori t3, t0, 0x6 # 0b110 
                beq t3, t2, lAND
                j fail
                
                # and | x26 = 16
lAND:  
                addi s10, s10, 1
                li t0, 0x4 # 0b100
                li t1, 0x6 # 0b110
                li t2, 0x4 # 0b100
                and t3, t1, t0
                beq t3, t2, lANDI
                j fail
                
                # and imm | x26 = 17
lANDI:  
                addi s10, s10, 1
                li t0, 0x4 # 0b100
                li t2, 0x4 # 0b100
                andi t3, t0, 0x6 # 0b110 
                beq t3, t2, lSLT
                j fail
                
                # set less than | x26 = 18
lSLT:  
                addi s10, s10, 1
                li t0, 5
                li t1, -6
                li t2, 0
                li t3, 1
                slt t4, t1, t0
                bne t4, t3, fail
                slt t4, t0, t1
                beq t4, t2, lSLTI
                j fail
                
                # set less than imm | x26 = 19
lSLTI:  
                addi s10, s10, 1
                li t1, -6
                li t2, 0
                li t3, 1
                slti t4, t1, 5
                bne t4, t3, fail
                slti t4, t1, -9
                beq t4, t2, lSLTU
                j fail
                
                # set less than unsigned | x26 = 20
lSLTU:  
                addi s10, s10, 1
                li t0, 5
                li t1, -6
                li t2, 0
                li t3, 1
                sltu t4, t1, t0
                bne t4, t2, fail
                sltu t4, t0, t1
                beq t4, t3, lSLTIU
                j fail
                
                # set less than imm unsigned | x26 = 21
lSLTIU:  
                addi s10, s10, 1
                li t1, -6
                li t2, 0
                li t3, 1
                sltiu t4, t1, 5
                bne t4, t2, fail
                sltiu t4, t1, -4
                beq t4, t3, lBEQ
                j fail
                
                # branch if equal | x26 = 22
lBEQ:  
                addi s10, s10, 1
                li t0, -1
                li t1, -1
                li t2, 1
                beq t0, t2, fail
                beq t0, t1, lBNE
                j fail
                
                # branch if not equal | x26 = 23
lBNE:  
                addi s10, s10, 1
                li t0, -1
                li t1, -1
                li t2, 1
                bne t0, t1, fail
                bne t0, t2, lBLT
                j fail
                
                # branch if less than | x26 = 24
lBLT:  
                addi s10, s10, 1
                li t0, -1
                li t1, -1
                li t2, 1
                blt t0, t1, fail
                blt t2, t0, fail
                blt t0, t2, lBGE
                j fail
                
                # branch if greater or equal | x26 = 25
lBGE:  
                addi s10, s10, 1
                li t0, -1
                li t1, -1
                li t2, 1
                bge t0, t2, fail
                bge t0, t1, lBLTU
                j fail
                
                # branch if less than unsigned | x26 = 26
lBLTU:  
                addi s10, s10, 1
                li t0, -1
                li t1, 1
                li t2, 0
                bltu t0, t0, fail
                bltu t0, t1, fail
                bltu t1, t0, lBGEU
                j fail
                
                # branch if greater or equal unsigned | x26 = 27
lBGEU:  
                addi s10, s10, 1
                li t0, -1
                li t1, 1
                li t2, 0
                bgeu t1, t0, fail
                bgeu t0, t2, lJAL
                j fail
                
                # jump and link | x26 = 28
lJAL:  
                addi s10, s10, 1
                jal test
                j lJALR
                j fail
test:  
                ret
                j fail
                
                # jump and link register | x26 = 29
lJALR:  
                addi s10, s10, 1
                auipc t0, 0
                jalr ra, t0, -12
                j lSBLB
                j fail
                
                # store byte and load byte | x26 = 30
lSBLB:  
                addi s10, s10, 1
                li t0, -20
                li t1, 35
                sb t0, 0(t1)
                lb t2, 0(t1)
                bne t2, t0, fail
                # now unsigned
                lbu t2, 0(t1)
                beq t2, t0, fail
                bltu t2, t0, lSHLH
                j fail
                
                # store half and load half | x26 = 31
lSHLH:  
                addi s10, s10, 1
                li t0, -20
                li t1, 44
                sh t0, 0(t1)
                lh t2, 0(t1)
                bne t2, t0, fail
                # now unsigned
                lhu t2, 0(t1)
                beq t2, t0, fail
                bltu t2, t0, lSWLW
                j fail
                
                # store word and load word | x26 = 32
lSWLW:  
                addi s10, s10, 1
                li t0, -20
                li t1, 48
                sw t0, 0(t1)
                lw t2, 0(t1)
                beq t2, t0, lx0
                j fail
                
                # Begin non specific instruction testing
                
                # writing to x0 | x26 = 33
lx0:  
                addi s10, s10, 1
                li t0, 0
                addi x0, x0, 500
                beq x0, t0, regfile
                j fail
                
                # regfile integrity | x26 = 34
regfile:  
                addi s10, s10, 1
                li x1, 0xFFFF
                li x31, 0xAAAA
                beq x1, x31, fail
                j byte_isolation
                
                # byte isolation | 35
byte_isolation:  
                addi s10, s10, 1
                li t1, 32
                li t0, 0xFFFFFFFF
                sw t0, 0(t1)
                sb x0, 0(t1)
                li t0, 0xFFFFFF00
                lw t2, 0(t1)
                beq t2, t0, success
                j fail
                
fail:  
                li s11, -1
                j fail
                
success:  
                add s9, x0, x0
                
loop:  
                addi s9, s9, 1
                j loop
                
                