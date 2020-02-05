.pos 0x100
ld $a, r0       # r0 = addr. a 
ld $s, r1       # r1 = addr. s
ld $tos, r2     # r2 = addr. tos
ld $tmp, r3     # r3 = addr. tmp
ld $0, r4       # r4 = 0
st r4, (r2)     # tos = 0
st r4, (r3)     # tmp = 0
ld (r2),r2
ld (r3),r5

ld (r0), r0     # r0 = value of a[0]
st r0, (r1)     # s[tos] (s[0]) = a[0]
inc r2          # tos = 1
ld $0x210, r1   # s[tos] = s[1] = 0x210
ld $0x204, r0   # r0 = [a1]
ld (r0), r0     # r0 = val of a[1]
st r0, (r1)     # s[1] = val of a[1]
inc r2          # tos = 2
ld $0x214, r1   # s[tos] = s[2] = 0x214
ld $0x208, r0   # r0 = addr. a[2]
ld (r0), r0     # r0 = val a[2]
inc r2          # tos++
dec r2          # tos-- (tos = 2)
ld (r1), r5     # r5 = val s[2]
st r5, (r3)     # tmp = val s[2]
dec r2          # tos = 1
ld $0x210, r1   # s[1]
ld (r1), r1     # r1 = val s[1]
add r1, r5      # tmp = tmp + s[1]
st r5, (r3)
dec r2          # tos = 0
ld $0x20c, r1   # s[0]
ld (r1), r1     
add r1, r5
st r5, (r3)
halt


.pos 0x200
    a: .long 3 #0x200
       .long 2 #0x204
       .long 4 #0x208
    s: .long 1 #0x20c
       .long 2 #0x210
       .long 3 #0x214
       .long 4 #0x218
       .long 5 #0x21c
    tos: .long 0
    tmp: .long 0