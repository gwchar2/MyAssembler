# My Assembler

# Table of Contents
- [Introduction](#introduction)
- [Project Structure](#Project-Structure)
- [Project Features](#Project-Features)
- [Project Flow](#Project-Flow)

## Introduction
Dear Project Reviewer,

This README file is meant for you.
Over here, you can learn more about our project structure, logic and flow.
Before we kick it off, there are a few things you must know..

1) All build files are constructed into the 'build' folder.

2) In the 'tests' folder you can find 4 different files showcasing the output received from this code.
    All outputs were ran on MAC M1 , Ubuntu 16.04 and Windows 10 (x64)


## Project Structure

### Source Files

- `src -> mainAssembler.c`: Main program logic.
- `src -> main_functions.c`: The three major functions that are called in the main.
- `Data Structures Folder `: Holds the data structure files.
- `lib Folder`: Holds the handlers for the rest of the program.
- `header.h`: Briefly defines the Macros, external variables, structures & functions of the program.


## Project Features
### Machine Code Structure

- Machine Code: Encrypted BASE 4 (0 = *, 1 = #, 2 = %, 3 = !)
- Structure: 1 to 5 memory words per code
- Types: A (Absolute), R (Relocatable), E (External)

### Instruction Set

Commands are divided into 3 groups:

1. Group 1 (2 Operands):
   - mov, cmp, add, sub, lea

2. Group 2 (1 Operand):
   - not, clr, inc, dec, jmp, bne, red, prn, jsr

3. Group 3 (0 Operands):
   - rts, hlt

### Macros

- Macro Format: m_mcr
- Example Macro:
```c
mcr m_mcr
inc r2
mov A,r1
endmcr
```

### File Structure

- Line Length: Up to 80 characters
- Sentence Types: Empty, Note, Label, Command, Define
- Labeled Sentences: .data, .string, .entry, .extern

### Variables

- Label Format: Starts with alphabetic letter, max 31 characters
- Data Types: .string, .data, command
  
  Important to note:
  It is stricly prohibited FOR EVERY COMMAND to parse in a string or data with the address of a different label (other than a .define)!

   For example:

```c
    Illegal:
        add  r2, label[someLabel]
        lea label[someLabel],label[otherLabel]
        
    Legal:
        .define sz = 2
        add label[3], label[sz]
        add #3, label[3]
```

### Input/Output

- Input: Text files (source code), file names passed as ARGV
- Output Files: .am (source code after macro translation), .ob (final machine code), .ext (external labels), .ent (entry labels)
  
  Important to note:
  Empty lines, noted lines with ';' and lines that exceed the project limit (80 characters) DO NOT COUNT! An error will be prompted if you pass the 80 characters.

### Object File Format

- Header: Total length of instruction segment, total length of data segment
- Content: Address (base 10) | Encrypted BASE 4 (7 digits)

### Entry and Externals Files

- Entry File Format: Name of Label | Address of Label Initiation
- Externals File Format: Name of Label | Address of Usage in Code
- Prompts the user for errors, including the specific line the error was made. 
- This project is constructed from a few linked lists, each linked list represents a different aspect of the program.


## Project Flow
For a brief explanation about the project flow, you can take a look at the following figma graph.
[https://www.figma.com/file/vnuBfEvpOJfwIhQHCc1v6Q/Assembler?type=whiteboard&node-id=0%3A1&t=YEHNED1LrLzBxb0X-1]

The project consists of 3 major phases.
Before we get into the different phases, we 'grab' the user inputted file names from argv.
Throughout the project we will iterate on the files. For each file, we will create the corresponding required files, unless an error emerges in one of the files.
If an error emerges in one of the files received, the corresponding .o .ext and .ent files will not be created!

### Phase One
Main logic of this phase can be found in `main_functions.c -> preAssembler`.
In this phase, we iterate through the .am file opened, and pre-process the macros. 
We swap the macros with the correct text, and create a .as file.

### Phase Two
Main logic of this phase can be found in `main_functions.c -> scan_file`.
In this phase, we iterate through the .as file opened, and pre-process the entire file.
Each line received goes through a statement checking its length. Lines over 80 characters are considered as errors.
We start by checking for specific characteristics that can help us recognize what the following line may be.
Afterwords, we send the line on its way down the flow, deppending on its type.

In this phase, we start building the Data Segment , Instruction Segment & Label Table of the file.
Each one of them is structured by a nested list.
The Instruction Segment uses the Cmd Nodes to hold the command with all its components, and the Reg Nodes for the different registers received. (List of lists)
The Data Segment is a nested list, inside of the Label Table we have constructed.
The Label Table is built from a list of lists. Each Label Node holds the name, type & other crucial information.
Inside of the label table, we have further nested lists, for each specific type, with the most important one being for the Data Segment.
The Data segment is built from Label Nodes that have a private Data Nodes list that 'holds' its data. (List of lists).
The Externals, as well, are constructed from a list of lists. Each 'row' is represented in a new Row Node.

After we finish constructing the structures, we go on to phase 3.

### Phase Three
Main logic of this phase can be found in `main_functions.c -> makefiles`.
In this phase, we do not have much left to do, but it is the most crucial.
We now match entry labels, fix the instruction segment registers that use label reffrences, and increment external appearances.
Afterwords, we translate the data received in the nodes, to the correct binary form.
If there are no errors in the file, we create .ext .entry .o files. If there are no external or entry labels in the code, the files will not be produced.
We free the lists, reset the pointers & global variables, and go on to the next file.
Important note: Pointer list frees all the dynamicaly allocated strings in the program (that are not free'd with there nodes).
