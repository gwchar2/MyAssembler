# My Assembler

## Overview
Dear Project Reviewer,

This README file is ment for you.
Over here, you can learn more about our project structure, logic and flow.
Before we kick it off, there are a few things you must know..

1) Empty lines, noted lines with ';' and lines that exceed the project limit (80 characters) DO NOT COUNT!
    Furthermore, they will not be handled within the code (An error will be prompted if you pass the 80 characters).

2) It is stricly prohibited to parse in a string or data with the address of a different label (other than a .define)! 
    for example:

```c
    Illegal:
        add  r2, label[someLabel]
        lea label[someLabel],label[otherLabel]
        
    Legal:
        .define sz = 2
        add label[3], label[sz]
        add #3, label[3]
```
    is illegal FOR EVERY COMMAND! and will be returned as an error.

3) All build files are constructed into the 'build' folder. So dont worry if you cant see them!

4) In the 'tests' folder you can find 4 different files showcasing the output received from this code.
    All outputs were ran on MAC M1 , Ubuntu 16.04 and Windows 10 (x64)

To see our work progress on github, you can click here.
[Github][https://github.com/gwchar2/MyAssembler]

Scroll down more for a detailed project flow.

## Project Structure

### Header File: `header.h`

- Defines and macros
- Global enums
- Structures
- External variables
- Main functions
- Label nodes
- Data nodes
- Command nodes
- Macro nodes
- Row nodes
- Pointer nodes
- Error handler
- Extern handler
- Binary handler
- File handler
- Label handler
- Other functions

### Source Files

- `src -> mainAssembler.c`: Main program logic.
- `src -> main_functions.c`: The three major functions that are called in the main.
- `Data Structures Folder `: Holds the data structure files.
- `lib Folder` : Holds the handlers for the rest of the program.

## Project Flow
For a brief explanation about the project flow, you can take a look at the following figma graph.
[Figma][https://www.figma.com/file/vnuBfEvpOJfwIhQHCc1v6Q/Assembler?type=whiteboard&node-id=0%3A1&t=YEHNED1LrLzBxb0X-1]

The project consists of 3 major phases.
Before we get into the different phases, we 'grab' the user inputted file names from argv.
Throughout the project we will itterate on the files. For each file, we will create the corresponding required files, unless an error emerges in one of the files.
If an error emerges in one of the files received, the corresponding .o .ext and .ent files will not be created!

### Phase One
Main logic of this phase can be found in `main_functions.c -> preAssembler`.
In this phase, we itterate through the .am file opened, and pre-process the macros. 
We swap the macros with the correct text, and create a .as file.

### Phase Two
Main logic of this phase can be found in `main_functions.c -> scan_file`.
In this phase, we itterate through the .as file opened, and pre-process the entire file.
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

## Project Features

- Opens and translates any amount of .as files (via CLI).
- Translates all code received to an encrypted base 4 code as stated in the project requirements. 
- Prompts the user for errors, including the specific line the error was made. 
- This project is constructed from a few linked lists, each linked list represents a different aspect of the program.


