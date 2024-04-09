add r5, r4
.define X = 2
   add #X ,r4
y: .data 8
dec y
dec W
LIST: .data 6, -9, 5
mov LIST[1] , r3
mov r3 , LIST[X]
.entry HELLO
HELLO: .data 17
.extern W