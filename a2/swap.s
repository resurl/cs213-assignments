ld $0, r0       # r0 = 0
ld $t, r1       # r1 = addr of t   
st r0, (r1)     # t = 0
ld $array, r2    # r2 = array[0]
ld (r2), r4     # r4 = value of array[0]
st r4, (r1)     # t = array[0] (0x2000)
ld (r1), r5     # r5 = value of t
ld 4(r2), r3    # r3 = array[1] (0x2004)
st r3, (r2)     # array[0] = array[1]
inca r2         # r2 = value of array[1] + 4
st r5, (r2)     # array[1] = t 
halt

.pos 0x1000
t: 
.long 0

.pos 0x2000
array:
.long 1
.long 2