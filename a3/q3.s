.pos 0x100
    ld $a, r0   # r0 = a
    ld $b, r1   # r1 = b[0]
    ld $p, r2   # r2 = p
    ld $3, r3   # r3 = 3
    st r3, (r0) # a = 3
    st r0, (r2) # p = addr a
    ld (r0), r7 # r7 = val a 
    dec r7      # r7 = val a = 2
    st r7, (r0)
    st r1, (r2) # p = addr b[0]
    ld (r2), r4 # r4 = val of p (addr b[0])
    inca r4     # r4 = addr b[1]
    st r4, (r2) # p = addr b[1]
    ld (r2), r5 # r5 = addr b[1]
    ld $8, r3   # r3 = 8 (2 mem indexes)
    add r3, r5  # b[1 + 2] = addr b[3], 4th index
    ld (r1, r7, 4), r6 # r6 = val b[2]
    st r6, (r5) # p[a] = b[a]
    ld (r1), r1 # r1 = val b[0]
    inca r5
    st r1, (r5) # *(p+3) = b[0]
    halt

.pos 0x200
    a: .long 0 # 0x200
.pos 0x300
    b: .long 5 # 0x300
       .long 4 # 0x308 
       .long 3 # 0x30c
       .long 8
       .long 7
.pos 0x400       
    p: .long 0