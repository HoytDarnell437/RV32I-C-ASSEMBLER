.data
        .byte 'H' 'e' 'l' 'l' 'o' ' ' 'W' 'o' 'r' 'l' 'd' '!' '\\' '\'' '\0'
        .byte 0x2
        .align 2
        .byte 3
        .word 808 999 0 77
        .space 5
        .word 0xFFF

        # --- data_image edge-case tests ---
        .byte 0xFF 0xAB 0x00          # hex byte edge values
        .byte 255 0x10 32             # mixed decimal/hex formats
        .byte 1                       # set odd offset for .word auto-align test
        .word 42                      # .word auto-alignment from odd offset
        .space 3                      # space before .word, tests alignment after space
        .word 0xCAFE                  # .word after .space alignment
        .byte 1 2 3                   # misalign for .align 4 test
        .align 4                      # align to 16-byte boundary
        .word 99                      # word at 16-byte aligned offset
        .word 100                     # set aligned offset for no-op .align test
        .align 2                      # .align when already word-aligned (no padding expected)
        .word 200                     # word after no-op align
        .word -1                      # negative value, two's complement
        .byte 10 20                   # consecutive .byte directives
        .byte 30 40 50               # consecutive .byte directives

hola: .equ 49

.TEXT
        aDd x1, X2, x3

help:   sub x3, x4, x5
        lw x2, 5(x1)
SON:
        addi x1, x2, 60

hello: .equ 50
