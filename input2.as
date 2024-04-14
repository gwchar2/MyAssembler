

LOOP: jmp w   
        prn #-5
mov STR[5],STR[2]
sub r1,r4
cmp K, #sz
bne w
L1: inc L3
.entry LOOP
        bne LOOP
.entry LOOP
        bne LOOP
END: hlt
.define len = 4
STR: .string "abcdef"
LIST: .data 6,-9,len
K: .data 22
.extern L3