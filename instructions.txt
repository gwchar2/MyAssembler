HARDWARE:
The computer in the project is build from a CPU, registers, and RAM. some of the ram is used as a stack.

The processor has 9 registers
r0 r1 r2 r3 r4 r5 r6 r7 PSW
every register is 14 bits.
msb - #13 lsb - #0 [13 12 11 10...0]
the registers are always with a small letter - r0, r1..r7
PSW (program status word) includes a number of flags that represent the work of the processor at every moment. (explained later)
PC - program counter - holds the memory address of the CURRENT COMMAND

Size of the RAM is 4096 cells, in the addresses 0-4095
every cell is 14 bits (same numbering as a register)
every cell is reffered to as 'word'


This computer works only with Z type numbers, positive & negative (and 0)
There is no ability to support R or Q numbers.
This computer supports characters, represented as ASCII value.
in the output file that includes the machine code that the assembler assembels, every word will be coded by ENCRYPTED BASE 4 as such:
0 = *, 1=# , 2 = %, 3 = !

Structure of machine code:
every machine code is structured to a few memory words in a row. starting from 1 word, up to 5.
A - aboslute - 00 - function uses immidiete value
R - relocatable - 10 - function uses label from file
E - external - 01 - function uses label from DIFFERENT file

שיטות מיעון:
0 - מיעון של ערך מיידי
Different types of words:
Command word:
	bits 13-12-11-10 = NOT IN USE ALWAYS 0!!
	bits 9-8-7-6 = OPCODE
	bits 5-4 = מיעון אופרנד מקור RS
	bits 3-2 = מיעון אופרנד יעד RT
	bits 1-0 = A,R,E => 00 (always 00 for command)
Label word:
	bits 13-12-11-10-9-8-7-6-5-4-3-2 = Address of label 12 bit
	bits 1-0 = ARE (10 if internal address, 00 if external address)

קוד הפעולה תמיד אותיות קטנות
0 mov
1 cmp
2 add
3 sub
4 not
5 clr
6 lea
7 inc
8 dec
9 jmp
10 bne
11 red
12 prn
13 jsr
14 rts
15 hlt

The commands are split into 3 groups:

First group : These get 2 operands.
0 mov A, r1 - Copies A to r1
1 cmp A, r1 - If A = r1, than PSW = Z (Turned on 1) else Turned off (0)
2 add A, r0 - r0 = r0 + A
3 sub #3, r1 - r1 = r1 - #3
6 lea HELLO,r1 - r1 = HELLO

Second group: 1 operand only! RS = 00
not r2 - r2 = not r2
clr r2 - r2 = 0
inc r2 - r2 = r2 + 1
dec C - C = C – 1
jmp LINE - PC = LINE 
bne LINE - PC = LINE IF Z flag in PSW = 0, else (1) DOESNT JUMP
red r1 - stdin (user input) into r1
prn r1 - print r1
jsr FUNC - JUMP AND LINK (jump to FUNC and keep last PC in PC stack! push(PC) )

Third group: 0 Operands! RS & RT = 00
rts - return to last PC from PC Stack (pop() from PC stack!)
hlt - exit


MACROS:
Whenever we meet m_mcr in text, we paste the 2 following lines that are in the macro (can be more, user dependent)
mcr m_mcr
	inc r2
	mov A,r1
endmcr
ניתן להניח שלכל שורת מאקרו בקוד המקור קיימת סגירה עם שורת 
endmcr
אין צורך לבדוק זאת
צריך לייצר "קובץ מקור מורחב" שהוא העתק של הקובץ מקור של היוזר אבל לאחר פירוש כלללללל המקרואים


length of line in file is up to 80 characters not including \n
משפט ריק empty sentence - full of whitespaces \t or ' ' or nothing except \n
משפטה הערה note sentence - starts with ; - to be ignored completely!
משפט הנחיה labled sentences - initializes variables such as int x = ?
משפת הוראה command sentences - one of the commands above
משפט הגדרת קבוע define sentences - #define A 5 משפט הגדרת קבוע הנותן שם סימבולי המייצג קבוע מספרי

LABLED SENTENCES:
.data :
מופרדים בפסיקים, לאו דווקא צמודים
XYZ is name of the data array (array of ints)
*all data types declared in the beginning have to be one after the other in the memory?? check!
*for example if 2 arrays are declared one after the other, than mem[xyz]+5 = mem[zyx][1]
XYZ: .data 7, –57, +17, 9
ZYX: .data 7, –57, +17, 9
mov XYZ, r1 - r1 = 7

.string:
characters in the string are coded according to ASCII but 14 bits
STR: .string "abcdef" 
every letter gets its 14 bits ascii code
STR is a character array

.entry: 
Initializes a lable that represents a line of code as a global line of code (can be used in other files)
ONLLLYYYY if the other file has .extern HELLO
Example:
.entry HELLO
HELLO: add #1, r1
NOTICE: 
    -same file can not have .extern & .entry of same lable!!
    -CODE IGNORES THINGS LIKE: HELLO: .entry  (can not lable an entry or extern)

.extern:
an extern is a parameter that isnt labled in the file before hand !!! (Not as a macro, or as a different lable at ALL!!!)
extern alerts that assembler that the line of code represented by the lable is in a different file !
for example, if we want to use HELLO: add #1, r1 in our file, we must start by adding 
.extern HELLO
to the top of the file.
NOTICE: 
    -same file can not have .extern & .entry of same lable!!
    -CODE IGNORES THINGS LIKE: HELLO: .external (can not lable an entry or extern)



משפת הוראה COMMAND SENTENCES:
התווית אינה חובה
1. label: opcode source-operand, target-operand
	HELLO: add r7, B
2. label: opcode target-operand
	HELLO: bne XYZ
3. label: opcode
	END: hlt

משפט הגדרת קבוע DEFINE SENTENCES:
.define קבוע-מספרי = שם-הקבוע
אסור להגדיר את אותו שם קבוע יותר מפעם אחד
אותו סמל לא יכול לשמש הן כשם של קבוע והן כתווית באותה תוכנית.(.extern / .entry)
מילים שמורות של שפת האסמבלי לא יכולות לשמש שם של קבוע.
הקבוע המספרי הוא שלם בבסיס עשרוני.
בין שם קבוע לבין קבוע מפריד = ומותר תווים לבנים בין שני הצדדים


Variables in the assembly language:
Label : 
	- starts with an alphabetic letter
	- after it , alphabetic characters (small or big)
	- maximum number is 31 characters
	- after the 31, or at the end, must include :
	- does not appear more than once
	- can not be a register / other name already in the program
hEllo: .string ~~ OR .data ~~ OR command
לתשומת לב - מותר במשפט הוראה להשתמש באופרנד שהוא סמל שאינו מוגדר כתווית בקובץ הנוכחי, כל עוד הסמל מאופיין כחיצוני באמצעות 
extern

number:
either +76, -76 or 76

string:
printable ascii numbers


Input/output:
When running the program, receives ARGV names of files (1 or more)
These are text files including the "source code"
These text files finish with the ending of .as, for example: Hello.as or x.as
The program DOES NOT RECEIVE THE .as !!! Must input it alone!!!


The assembler works on every file seperatly and makes the following output files:
(If the input file was Hello.as)
am file Hello.am - includes the source code after translation of all the macros if there are no macros - will be identical to the .as file!!!
object file Hello.ob - includes the final machine code
external file Hello.ext - Includes all the .external - if no externals, doesnt make the file.
entries file Hello.ent- Includes all the .entries - if no entries, doesnt make the file

Object file format:
First line of the file (HEADER) includes 2 numbers in base 10: 
	The total length of the the instruction segment (after seperating to words)
	The total length of the data segment 
Rest of the lines are the instruction segment and than afterwords the data segment, every line as the following:
ADDRESS(base10) | encrypted base 4 (in 7 digits)
All addresses according to there FINAL location in the object file


Entry file format:
NAME_OF_LABEL | (base 10) ADDRESS_OF_LABEL_INITIATION 

Externals file format:
NAME_OF_LABEL | (base 10) ADDRESS OF EVERY SINGLE PLACE IN THE CODE IT IS USED!! EVERY PLACE -> NEW LINE (with NAME OF LABEL)


https://www.figma.com/file/vnuBfEvpOJfwIhQHCc1v6Q/Assembler?type=whiteboard&node-id=0-1&t=YVzJSuFsveAoX8L8-0
