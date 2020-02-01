.pos 0x1000
swap: ld $0, r0       # r0 = 0
ld $x, r1       # r1 = addr of t   
st r0, (r1)     # t = 0
ld $data, r2    # r2 = array[0]
ld (r2), r4     # r4 = value of array[0]
st r4, (r1)     # t = array[0] (0x2000)
ld (r1), r5     # r5 = value of t
ld 4(r2), r3    # r3 = array[1] (0x2004)
st r3, (r2)     # array[0] = array[1]
inca r2         # r2 = value of array[1] + 4
st r5, (r2)     # array[1] = t 
halt
quiz: st $3 ($0x2000, $4,4)
halt

calc: ld $array, r0        # r0 = address of data array
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
math: ld $x, r0
ld $b, r1
ld (r1), r4        # r4 = value of b (25)
ld $0x05, r2
add r4, r2
shr $1, r2
and r4, r2
shl $2, r2
st r2, (r0)
halt
li:   ld  $data, r1
      ld  $0,    r2
      ld  $1,    r3
      ld  (r1,r2,4), r4     # r4 <= m[data + 0]
      ld  (r1,r3,4), r5     # r5 <= m[data + 4]
      halt
si:   ld  $data, r1
      ld  $0, r2
      ld  $3, r3
      ld  $1, r4
      st  r3, (r1, r2, 4)   # m[data + 0] <= 3
      st  r4, (r1, r4, 4)   # m[data + 4] <= 1
      halt
rmov: ld  $1, r1
      mov r1, r2            # r2 <= 1
      ld  $4, r1
      mov r1, r2
      halt
add:  ld  $1, r1
      ld  $2, r2
      add r1, r2            # r2 <= 1 + 2
      halt
and:  ld  $10, r1
      ld  $6, r2
      and r1, r2            # r2 <= 10 & 6 == 0b1010 & 0b0110 == 0b0010 == 2
      halt
inc:  ld  $1, r1
      inc r1                # r1 <= 1 + 1
      halt
inca: ld  $1, r1
      inca r1               # r1 <= 1 + 4
      halt
dec:  ld  $2, r1
      dec r1                # r1 <= 2 - 1
      halt
deca: ld  $5, r1
      deca r1               # r1 <= 5 - 4
      halt
not:  ld  $0xffff0000, r1
      not r1                # r1 <= ! 0xffff0000 == 0x0000ffff
      halt
shl:  ld  $0x12345678, r1
      shl $17, r1            # r1 <= 0x1234567 << 17 == 0xacf0000
      halt
shr:  ld  $0x100, r1
      shr $1, r1
      halt

.pos 0x2000
data: .long 12884
      .long 3424

.pos 0x6000
array: .long 0
.long 0
.long 254
.long 1

.pos 0x5000
i: .long 2

.pos 0x3000
x: .long 0

.pos 0x4000
y: .long 0

.pos 0x7000
b: .long 25

.pos 0x8000
a:  .long 0
b:  .long 1
    .long 2
    .long 3
    .long 4