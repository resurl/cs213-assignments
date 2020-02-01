ld $a, r0
ld $b, r1
ld (r1), r4        # r4 = value of b (25)
ld $0x05, r2
add r4, r2
shr $1, r2
and r4, r2
shl $2, r2
st r2, (r0)
halt

.pos 0x1000
a: .long 0

.pos 0x2000
b: .long 25