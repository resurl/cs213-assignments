***for the assignment:***

**c language debugging --**
to insert debugging information -> gcc -std=gnu11 __-g__ -o foo foo.c

to debug -> gdb foo (windows); lldb foo (mac os)

b foo.c:14 -> breakpoint at line 14 in foo.c

run [args] -> runs program (w/ potential args if program needs it) until breakpoints

bt -> backtrace (function calls that lead up to breakpoint/crash/error)

p (variable) -> print value of variable 

s, n -> s = step, step into function calls; n = next, steps over function calls, ie lets function run normally

c/continue -> continues program until breakpoint or crash or exit

**makefile --**
specifies:

1) steps to build program (ie how lib & program files are linked) *good for reproducability 

2) "make" command rebuilds only parts of program/parts of source files that change 

make foo -> build foo from foo.c

writing Makefiles:
```
target: input1 input2 input3 
    command_to_build_output_from_its_inputs`
```
target depends on 3 other files (example), command is optional if exists a default rule for building a filetype (.c), otherwise command tells how to rebuild those parts to make target

**variables**
$(var) -> syntax