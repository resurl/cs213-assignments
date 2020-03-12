.pos 0x100
start:
    ld $sb, r5          # r5 = &sb
    inca    r5          # r5 = &sb + 4 
    gpc $6, r6          # r6 = pc + 6
    j main              # goto main
    halt

f:
    deca r5                 # sb -=4 allocate frame 
    ld $0, r0               # r0 = 0 (j)
    ld 4(r5), r1            # r1 = x[i]
    ld $0x80000000, r2      # r2 = 0x80000000
f_loop:
    beq r1, f_end           # if x[i] == 0 goto f_end
    mov r1, r3              # r3 = x[i]
    and r2, r3              # r3 = x[i] & 0x80000000
    beq r3, f_if1           # if x[i] == 0 (x[i] > 0) goto f_if1
    inc r0                  # r0 = j++ 
f_if1:
    shl $1, r1              # x[i] * 2
    br f_loop               # goto f_loop
f_end:
    inca r5                 # sb += 4 deallocate frame 
    j(r6)                   # return 

main:                      
    deca r5                 # sb -= 4
    deca r5                 # sb -= 4 allocate frame of 8 bytes
    st r6, 4(r5)            # *sb = ra
    ld $8, r4               # r4 = 8 = i
main_loop:
    beq r4, main_end        # if i == 0, goto main_end
    dec r4                  # i--
    ld $x, r0               # r0 = &x
    ld (r0,r4,4), r0        # r0 = x[i]
    deca r5                 # sb -= 4 allocate frame of 4 bytes
    st r0, (r5)             # *sb = x[i]
    gpc $6, r6              # r6 = pc + 6    
    j f                     # goto f
    inca r5                 # sb += 4 deallocate frame of 4 bytes 
    ld $y, r1               # r1 = &y
    st r0, (r1,r4,4)        # y[i] = j
    br main_loop            # goto main_loop
main_end:
    ld 4(r5), r6            # r6 = callee ra
    inca r5                 # r5 += 4
    inca r5                 # r5 += 4 deallocate frame of 8 bytes
    j (r6)                  # return

.pos 0x2000
x:
    .long 1
    .long 2
    .long 3
    .long 0xffffffff
    .long 0xfffffffe
    .long 0
    .long 184
    .long 340057058

y:
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

.pos 0x8000
# These are here so you can see (some of) the stack contents.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0

