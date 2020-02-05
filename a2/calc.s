ld $data, r0        # r0 = address of data array
ld $i, r1           # r1 = addr of index (i)
ld $x, r2           # r2 = addr of x
ld $y, r3           # r3 = addr of y
ld (r1), r4         # r4 = value of i
ld (r0,r4,4), r5    # r5 = value of data[i]
inc r4              # i + 1
ld (r0,r4,4), r6    # r6 = value of data[i+1]
add r6, r5          # r4 = data[i] + data[i+1]
st r5, (r3)         # y = above
ld $0xff, r7        # r7 = 0xff
and r5, r7          # y & 0xff
st r7, (r2)          # x = above 
halt

.pos 0x1000
data: .long 0
.long 0
.long 0
.long 0

.pos 0x2000
i: .long 2

.pos 0x3000
x: .long 0

.pos 0x4000
y: .long 0