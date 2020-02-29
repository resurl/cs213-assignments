.pos 0x1000
    ld $0, r1
    ld $n, r2
    ld (r2), r2
    ld $a, r3
    ld $b, r4
loop: mov r1, r5    # iterating i
      not r5
      inc r5
      add r2, r5
      bgt r5, if
      br end
if:   ld (r3, r1, 4), r5    # a[i]
      ld (r4, r1, 4), r0    # b[i]
      not r0
      inc r0
      add r0, r5
      inc r1
      bgt r5, cont
      br loop
cont: inc r7
      br loop
end:  ld $i, r0
      ld $c, r2
      st r7, (r2)
      st r1, (r0)
      halt
      


.pos 0x2000
i: .long 0xffffffff
n: .long 5
a: .long 10
   .long 20
   .long 30
   .long 40
   .long 50
b: .long 11
    .long 20
    .long 28
    .long 44
    .long 48
c: .long 0