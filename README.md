# Table of Contents

1. [Introduction](#introduction)
   - [Overview](#overview)
   - [Key Features](#key-features)
2. [Getting Started](#getting-started)
   - [Prerequisites](#prerequisites)
   - [Installation & Setup Instructions](#installation--setup-instructions)
3. [Usage](#usage)
   - [How to Assemble a File](#how-to-assemble-a-file)
4. [Instruction Set Summary](#instruction-set-summary)
   - [Instruction Word Encoding](#instruction-word-encoding)
   - [OpCodes](#opcodes)
   - [Addressing Modes](#addressing-modes)
   - [A,R,E Encoding](#are-encoding)
   - [Encryption Details](#encryption-details)
5. [File Generation](#file-generation)
     - [Object Files](#object-files)
     - [Entry Files](#entry-files)
     - [External Files](#external-files)
4. [Examples](#examples)
   - [Basic Example](#basic-example)
   - [Advanced Example](#advanced-example)
5. [Error Handling](#error-handling)
   - [Common Errors](#common-errors)
   - [Debugging Tips](#debugging-tips)
6. [FAQ](#faq)
7. [Detailed Project Flow](#project-flow)


## Introduction

### Overview
This project is a custom assembler that reads assembly files, processes 16 different commands, macros, and notes, and translates them into encrypted Base 4 format. The assembler outputs several file types, including .am, .external, .entry, and .o files. It also features built-in error handling, providing users with notifications about errors encountered during the assembly process. Designed to enhance the security and structure of assembly language projects, this tool simplifies the translation of low-level code into an efficient, encrypted format.

### Key Features
- **Support for 16 Commands**: Translates 16 distinct assembly language commands into encrypted base 4 format.
- **Macro Handling**: Processes and integrates macros within the assembly files.
- **File Generation**:
  - Outputs `.am` files (processed assembly).
  - Produces `.external` and `.entry` files for external and entry symbols.
  - Generates `.o` object files.
- **Encryption**: Outputs files in encrypted base 4 format for added security.
- **Error Handling**: Provides detailed notifications about syntax and semantic errors in the assembly code.
- **Compatibility**: Supports different file types, allowing seamless integration with various development environments.
- **Efficiency**: Optimized to handle large assembly files and translate them quickly.


## Getting Started

### Prerequisites
Before running the C-based assembler software, ensure that the following prerequisites are met:

- **C Compiler**: You need a C compiler installed on your machine (e.g., `gcc`, `clang`).
- **Make**: Ensure that `make` is installed to compile the project.
- **Assembly Files**: Have the assembly files ready for input, which will be processed by the assembler.

### Installation & Setup Instructions
1. **Download or Fork the Repository**:
   - Clone the repository to your local machine using the following command:
     ```bash
     git clone <repository_url>
     ```
   - Alternatively, you can fork the repository and then clone your fork.

2. **Navigate to the Project Directory**:
   - Move into the `MyAssembler/src` directory:
     ```bash
     cd MyAssembler/src
     ```

3. **Build the Assembler**:
   - Use `make` to compile the `mainAssembler.c` file:
     ```bash
     make mainAssembler
     ```

4. **Run the Assembler**:
   - After the build completes, run the assembler with your assembly file as input:
     ```bash
     ./MainAssembler <"User Input Here">
     ```
   Replace `"User Input Here"` with the actual path to your assembly file.

### Notes
- Rows that begin with `;` are designated as **comments** or **notes** and will not be processed by the assembler.

### How to Assemble a File
To assemble a file using the assembler, follow these steps:

1. **Prepare Your Assembly File**:
   - Create or edit your assembly file using a text editor. Ensure that the file contains valid commands according to the supported command structure.
   - Include comments where necessary, starting with a `;`.

2. **Run the Assembler**:
   - Open a terminal or command prompt.
   - Navigate to the directory where the `MainAssembler` executable is located.

3. **Execute the Assembler**:
   - Use the following command to run the assembler, replacing `<input_file.asm>` with the path to your assembly file:
     ```bash
     ./MainAssembler <input_file.asm>
     ```

4. **Check the Output**:
   - After running the assembler, check the directory for the generated output files:
     - **.am** files – processed assembly code.
     - **.external** files – external symbol definitions.
     - **.entry** files – entry point information.
     - **.o** files – object files.
   - Review any error messages or notifications provided during the assembly process to ensure the input file was processed correctly.

5. **Debugging Errors** (if any):
   - If errors are encountered, refer to the error messages for details on what needs to be corrected in your assembly file.
   - Make the necessary adjustments and re-run the assembler until the file assembles successfully.

## Instruction Set Summary

### Instruction Word Encoding

| Bits  | Usage |
|-------|-------|
| 0-1   | `A,R,E` |
| 2-3   | Destination Operand |
| 4-5   | Source Operand |
| 6-9   | Opcode |
| 10-13 | Not Used |

### Opcodes

| OpCode (Decimal) | Operation CMD | Usage |
|--------------------|------------|--------|
| 0  | mov | Move data between registers or memory locations. |
| 1  | cmp | Compare two operands |
| 2  | add | Add two operands. |
| 3  | sub | Subtract one operand from another. |
| 4  | not | Perform a bitwise NOT operation on the operand. |
| 5  | clr | Clear (reset to zero) the operand. |
| 6  | lea | Load the effective address of the operand into a register. |
| 7  | inc | Increment the operand by 1. |
| 8  | dec | Decrement the operand by 1. |
| 9  | jmp | Jump to a specified memory location. |
| 10 | bne | Branch to a specified location if the comparison is not equal. |
| 11 | red | Read input into the operand. |
| 12 | prn | Print the operand. |
| 13 | jsr | Jump to subroutine. |
| 14 | rts | Return from subroutine. |
| 15 | hlt | Halt the program. |

### Addressing Modes

| Mode Number | Addressing Mode Name | Example |
|------------|----------------------|---------|
| 0 | Immediate Addressing | `mov #-1,r2` | 
| 1 | Direct Addressing | `dec x` | 
| 2 | Indexed Addressing with Constant Index | `mov x[2],r2` | 
| 3 | Direct Register Addressing | `mov r1,r2` |

### `A,R,E` Encoding and Addressing Modes

| Value | Meaning |
|-------|---------|
| 00    | Absolute |
| 01    | External |
| 10    | Relocatable |

| Mode Number | Addressing Mode Name | Example | Explanation | A,R,E Value |
|------------|----------------------|---------|-------------|------------|
| 0 | Immediate Addressing | `mov #-1,r2` | The operand starts with the # character, followed immediately by a decimal integer. Can also be replaced by a defined name in the program | `A,R,E=00` |
| 1 | Direct Addressing | `dec x` | The operand is a label that has already been declared or will be declared later in the file, or a .extern directive. | `A,R,E=01/10` |
| 2 | Indexed Addressing with Constant Index | `mov x[2],r2` | The operand consists of a label that represents the starting address of a 0-indexed array, followed by an index in square brackets specifying the element to access. The index can be a numerical constant or a .define | Each word seperately (Immediate & Direct Addressing) |
| 3 | Direct Register Addressing | `mov r1,r2` | The operand is a name of a register. | `A,R,E=00` |

### Encryption Details 

The assembler project implements a custom encryption method for converting the original Base 4 representation of commands and data into an encrypted Base 4 format. The encryption process involves the following structure:

| **Original Base 4**     |    0    |    1    |    2    |    3    |
|-------------------------|---------|---------|---------|---------|
| **Encrypted Base 4**    |    *    |    #    |    %    |    !    |

### Purpose of Encryption
The purpose of this encryption method is to enhance the security of the output files by obscuring the actual machine code representation. This makes it more challenging for unauthorized users to interpret the assembly instructions and data stored within the generated files.

## File Generation

### Object Files
- **File Extension**: `.o`
- **Description**: Object files contain the machine code generated from the assembly source code. They are binary files that represent the compiled version of the assembly instructions, ready for linking with other object files or libraries to create an executable program.

### Entry Files
- **File Extension**: `.entry`
- **Description**: Entry files contain information about entry points in the assembly code. These files list the symbols defined in the assembly source that are designated as entry points, allowing other modules or programs to reference them. This is essential for linking and creating callable routines in larger applications.

### External Files
- **File Extension**: `.external`
- **Description**: External files include definitions for external symbols referenced in the assembly code but defined elsewhere (in different files or modules). They allow the assembler to keep track of these symbols, ensuring that links to external code are correctly resolved during the linking process.


## Examples
Save the code displayed below in a file named `basic_example.as`, then run the assembler as follows:
   `MainAssembler basic_example`

### Example Macro:
```c
mcr m_mcr
    inc r2
    mov A, r1
endmcr 
```

### Basic Example
To demonstrate a simple assembly file, consider the following example:
```assembly
; This is a simple assembly program
.define sz = 2
MAIN: mov r3,LIST[sz]
LOOP: jmp L1
        prn #-5
        mov STR[5], STR[2]
        sub r1,r4
        cmp r3, #sz
        bne END
L1:     inc K
        bne LOOP
END:    hlt
.define len = 4
STR:    .string "abcdef"
LIST:   .data 6,-9,len
K:      .data 22
```

### Advanced Example
```assembly
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
```

| Decimal Address | Source Code | Binary Machine Code |
|----------------|-------------|----------------------|
| 0100 | MAIN: mov r3, LIST[sz] | 00000000111000 |
| 0101 |  | 00000001100000 |
| 0102 |  | 00001000010010 |
| 0103 |  | 00000000001000 |
| 0104 | LOOP: jmp W | 00001001000100 |
| 0105 |  | 00000000000001 |
| 0106 | prn #-5 | 00001100000000 |
| 0107 |  | 11111111101100 |
| 0108 | mov STR[5], STR[2] | 00000000101000 |
| 0109 |  | 00000111110110 |
| 0110 |  | 00000000010100 |
| 0111 |  | 00000111110110 |
| 0112 |  | 00000000001000 |
| 0113 | sub r1, r4 | 00000011111100 |
| 0114 |  | 00000000110000 |
| 0115 | cmp K, #sz | 00000001010000 |
| 0116 |  | 00001000011110 |
| 0117 |  | 00000000001000 |
| 0118 | bne W | 00001010000100 |
| 0119 |  | 00000000000001 |
| 0120 | L1: inc L3 | 00000111000100 |
| 0121 |  | 00000000000001 |
| 0122 | bne LOOP | 00001010000100 |
| 0123 |  | 00000110100010 |
| 0124 | END: hlt | 00001111000000 |
| 0125 | STR: .string "abcdef" | 00000001100001 |
| 0126 |  | 00000001100010 |
| 0127 |  | 00000001100011 |
| 0128 |  | 00000001100100 |
| 0129 |  | 00000001100101 |
| 0130 |  | 00000001100110 |
| 0131 |  | 00000000000000 |
| 0132 |  |  |
| 0133 |  |  |
| 0134 | LIST: .data 6, -9, len | 00000000000110 |
| 0135 |  | 11111111110111 |
| 0136 |  | 00000000000100 |
| 0137 | K: .data 22 | 00000000010110 |

**Example .obj File**
```	
   25	11
100	****!%*
101	***#%**
102	**%*#*%
103	*****%*
104	**%#*#*
105	******#
106	**!****
107	!!!!%!*
108	****%%*
109	**#!!#%
110	****##*
111	**#!!#%
112	*****%*
113	***!!!*
114	****!**
115	***##**
116	**%*#!%
117	*****%*
118	**%%*#*
119	******#
120	**#!*#*
121	******#
122	**%%*#*
123	**#%%*%
124	**!!***
125	***#%*#
126	***#%*%
127	***#%*!
128	***#%#*
129	***#%##
130	***#%#%
131	*******
132	*****#%
133	!!!!!#!
134	*****#*
135	****##%
```

**Example .ext File**
```
W	105
W	119
L3	121
```

**Example .ent File**
```
LIST	132
LOOP	104
```
## Error Handling

The assembler includes robust error handling to help identify and resolve issues that may arise during the assembly process. The error manager function logs and describes errors found in the assembly file, along with the specific line number where the issue occurred.

### Common Errors

Here are some common errors that users might face when using the assembler:

| **Error**                     | **Description**                                                        | **Error Message**                                      |
|-------------------------------|------------------------------------------------------------------------|--------------------------------------------------------|
| **Undefined Register Name**    | Occurs when a register used in the code is not recognized.              | `Error: Undefined register name in line X`             |
| **Undefined Register Type**    | Invalid or unknown register type is used.                              | `Error: Undefined register type in line X`             |
| **Undefined Command**          | An invalid or unrecognized command is encountered.                     | `Error: Undefined command name in line X`              |
| **Extraneous Text**            | Unnecessary text after the command or argument.                        | `Error: Extraneous text after end of command in line X`|
| **Missing Argument**           | A command lacks required arguments.                                    | `Error: Missing argument in line X`                    |
| **Undefined Argument**         | An argument used is not defined.                                       | `Error: Undefined argument in line X`                  |
| **Illegal Comma**              | Comma placed in an invalid position.                                   | `Error: Illegal comma in line X`                       |
| **Missing Comma**              | A required comma is missing.                                           | `Error: Missing comma in line X`                       |
| **Multiple Consecutive Commas**| Consecutive commas are found between arguments.                        | `Error: Multiple consecutive commas in line X`         |
| **Missing Parentheses**        | Required parentheses are not provided.                                 | `Error: Missing parentheses in line X`                 |
| **Segmentation Fault**         | An issue with memory access during execution.                          | `Error: Segmentation fault in line X`                  |
| **Immediate Overflow**         | A number exceeds the allowable range for immediate values.             | `Error: Immediate overflow in line X`                  |
| **Line Length Exceeded**       | A line exceeds the 80-character limit.                                 | `Error: Line [X] is over 80 characters long`           |
| **Duplicate Label**            | The same label is redefined more than once in the file.                | `Error: Label in line [X] is a duplicate!`             |
| **Invalid Label Name**         | A label uses an invalid format or characters.                          | `Error: Invalid Label name in line X`                  |
| **No Such File**               | The specified file cannot be opened.                                   | `Error: No such file %s to open!`                      |
| **File Argument Missing**      | The program is missing input file arguments.                           | `Error: Please input file names to read from!`         |
| **Entry Declaration Missing**  | Entry symbol used without declaration in the file.                     | `Error: Entry in line X does not have a declaration!`   |
| **Undefined Label Usage**      | A label is used without being defined in the file.                     | `Error: Undefined Label is used in line X`             |
| **Illegal Brackets**           | Invalid usage of brackets in the assembly code.                        | `Error: Illegal brackets in line X`                    |
| **Folder Creation Error**      | An error occurs while creating a folder.                               | `Error creating folder!`                               |


### Debugging Tips

When encountering errors, follow these debugging tips to resolve the issues:

1. **Check Line Numbers**: The error messages include the line number where the error occurred. Always start debugging from the indicated line and check for syntax issues.
   
2. **Validate Register and Command Names**: Ensure that the registers and commands used in your assembly file are valid and supported. Refer to the list of supported commands in the documentation.

3. **Review Label Definitions**: Make sure that labels are unique and properly defined. Avoid reusing labels, and follow the valid label format (starting with an alphabetic character, max 31 characters).

4. **Verify Argument Formatting**: For commands that require arguments, ensure that they are properly formatted and all required arguments are provided.

5. **Ensure Correct Comma Placement**: Pay attention to the correct placement of commas. Multiple consecutive commas or missing commas between arguments will trigger errors.

6. **Check Parentheses**: Commands like `lea` or macro calls may require parentheses. Double-check that all necessary parentheses are in place.

7. **Keep Line Length Under 80 Characters**: If any line exceeds the 80-character limit, split it into multiple lines or adjust the formatting to stay within the limit.

8. **Look for Undefined Symbols**: Ensure all external symbols, labels, or macros are properly defined before they are used. Missing definitions will raise errors.

By following these tips and paying close attention to the error messages, you can efficiently debug your assembly file and ensure it is correctly processed by the assembler.

## FAQ

1. **What happens if an error occurs in one of the files?**
   If an error is found during assembly, the `.o`, `.ext`, and `.ent` files for that specific input file will not be created.

2. **Can I pass a file with lines over 80 characters?**
   No, lines exceeding 80 characters will trigger an error and halt the file processing.

3. **What files are produced after successful assembly?**
   If there are no errors, the assembler produces `.o`, `.ext`, and `.ent` files. If no external or entry labels exist, `.ext` and `.ent` files are not created.

4. **How are macros handled?**
   In the pre-assembly phase, macros are expanded, and the resulting code is written to a `.as` file for further processing.

## Project Flow

The project consists of three main phases:

1. **Phase One**: Pre-processing the input assembly files. Macros are expanded, and `.am` files are converted to `.as` files.
   
2. **Phase Two**: The `.as` files are scanned, and essential structures like the Data Segment, Instruction Segment, and Label Table are built. Each line is checked for errors such as exceeding 80 characters. During this phase, we identify commands, labels, and data for further processing.

3. **Phase Three**: Final processing of labels, registers, and external references. The data is translated into machine code, and output files (`.ext`, `.entry`, `.o`) are created if no errors are encountered.

For a detailed visual flow of the project, visit [this Figma graph](https://www.figma.com/file/vnuBfEvpOJfwIhQHCc1v6Q/Assembler?type=whiteboard&node-id=0%3A1&t=YEHNED1LrLzBxb0X-1).


