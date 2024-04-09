add r1,r2
.entry LIST
.extern W
.define sz = 2
.define len = 4
STR: .string "abcdef"
LIST: .data 6,-9,len
K: .data 22