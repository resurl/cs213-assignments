.pos 0x1000
ld $n, r0
ld $0x1010, r1
ld (r0), r0
bgt r0, change
beq r0, end
j 2(r1)
ld $a, r0
ld $0x1000, r1
j 6(r1)
change: ld $50, r0
j 4(r1)
end: halt

.pos 0x3000
n: .long 1
a: .long 0

