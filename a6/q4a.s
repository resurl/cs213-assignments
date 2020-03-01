.pos 0x0
                 ld   $sb, r5           # r5 = &sb
                 inca r5                # r5 = &sb + 4
                 gpc  $6, r6            # r6 = pc + 6      
                 j    0x300             # goto 0x300 
                 halt                     
.pos 0x100
                 .long 0x00001000         
.pos 0x200
                 ld   (r5), r0          # r0 = arg1 
                 ld   4(r5), r1         # r1 = arg2 
                 ld   $0x100, r2        # r2 = 0x100
                 ld   (r2), r2          # r2 = 0x1000 
                 ld   (r2, r1, 4), r3   # r3 = ptr[arg2]
                 add  r3, r0            # r0 += ptr[arg2]
                 st   r0, (r2, r1, 4)   # ptr[arg2] = r0  
                 j    (r6)              # return 
.pos 0x300
                 ld   $-12, r0          # r0 = -12
                 add  r0, r5            # allocate frame #1 to stack 
                 st   r6, 8(r5)         # save ra to bottom of stack 
                 ld   $1, r0            # r0 = 1 
                 st   r0, (r5)          # save l1 of frame #1 to stack
                 ld   $2, r0            # r0 = 2
                 st   r0, 4(r5)         # save l0 of frame #1 to stack
                 ld   $-8, r0           # r0 = -8
                 add  r0, r5            # allocate another frame #2 to stack 
                 ld   $3, r0            # r0 = 3
                 st   r0, (r5)          # save a0 of frame #2 to stack 
                 ld   $4, r0            # r0 = 4
                 st   r0, 4(r5)         # save a1 of frame #2 to stack
                 gpc  $6, r6            # ra = pc + 6
                 j    0x200             # goto 0x200
                 ld   $8, r0            # r0 = 8
                 add  r0, r5            # deallocate frame #2
                 ld   (r5), r1          # r1 = a1
                 ld   4(r5), r2         # r2 = a0
                 ld   $-8, r0           # r0 = -8
                 add  r0, r5            # allocate frame #2
                 st   r1, (r5)          # save in stack as a0
                 st   r2, 4(r5)         # save in stack as a1 
                 gpc  $6, r6            # ra = pc + 6     
                 j    0x200             # goto 0x200
                 ld   $8, r0            # r0 = 8
                 add  r0, r5            # deallocate frame #2
                 ld   8(r5), r6         # r6 = callee ra 
                 ld   $12, r0           # r0 = 12
                 add  r0, r5            # deallocate frame #1
                 j    (r6)              # return
.pos 0x1000
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
