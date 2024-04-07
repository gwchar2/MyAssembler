this is a file test
mcr test_macro
test_macro content
endmcr
test_macro
mcr SIZE
add r2, r3
sub r4, r5
.define sz=2
endmcr
command
SIZE
command number 2
mcr LOOP
add r2, r2
add r3,r3
sub r4,r5
endmcr
another line
LOOP