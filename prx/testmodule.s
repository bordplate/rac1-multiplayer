# assembly source file for mod
# you can define assembly labels, functions and data here 

# the following data is placed in the .text (code) segment
.text

# example asm function
# note the period is required, however it is not used in C
# C declaration: extern void foo( void );
.global .foo 
.foo:
blr

# the following data is placed in the .data (read-write data) segment
.data
# globally accessible ASM variable var
# C declaration: extern int bar;
.global bar
bar:
.int 0xDEADBABE
