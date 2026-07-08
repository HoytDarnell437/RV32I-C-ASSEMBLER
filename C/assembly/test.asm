.data
        .byte 'H' 'e' 'l' 'l' 'o' ' ' 'W' 'o' 'r' 'l' 'd' '!' 0

.TEXT 
        aDd x1, X2, x3

help:   sub x3, x4, x5
        lw x2, 5(x1)
SON:
        addi x1, x2, 60
