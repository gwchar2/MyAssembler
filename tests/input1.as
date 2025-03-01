; Advanced example demonstrating macros and external labels

.define arraySize = 5
.extern someFunction
myArray: .data 1, 2, 3, 4, 5
anotherLabel: .string "Array Initialized"
entryPoint: .entry myArray

main:   inc r1
        add r1, r2
        clr r1
        add r1,arraySize
        prn r1
        hlt