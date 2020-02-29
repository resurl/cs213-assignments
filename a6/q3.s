.pos 0x1000
ld $s, r0           # r0 = &s 
ld (r0), r1         # r1 = s 
ld $n, r2
ld (r2), r2         # r2 = n
ld $0, r3

avgloop: mov r3, r4          # temp i
not r4
inc r4
add r2, r4          # n-i
bgt r4, avg         # if i<n goto avg
mov r2, r3          # reset i (i = n)
dec r3              # i = n-1 (for sort)
br sort             # else goto sort

avg: ld $0, r6
     ld 4(r1), r5   # s[i].grade[0]
     add r5, r6
     ld 8(r1), r5   # s[i].grade[1]
     add r5, r6
     ld 12(r1), r5  # s[i].grade[2]
     add r5, r6
     ld 16(r1), r5  # s[i].grade[3]
     add r5, r6
     shr $2, r6
     st r6, 20(r1)  # s[i].average = grade >> 2
     ld $24, r6
     add r6, r1     # next student
     inc r3
     br avgloop

sort: ld (r0), r2 # &s[0]
      ld $1, r5    # j
      ld (r0), r1  # reset s 
      bgt r3, inner
      br median

inner: mov r3, r4   # i for comparison
       not r4
       inc r4       # -i
       add r5, r4   # j - i
       ld $24, r1   # r1 = 24
       add r1, r2   # &s[j]
       bgt r4, sort  
       br sortif

sortif: ld 20(r2), r6 # r6 = s[j].average
        ld $24, r7
        not r7
        inc r7
        add r7, r2
        ld 20(r2), r7 #r7 = s[j-1]
        not r7
        inc r7
        add r7, r6 #r6 = a[j-1].average - s[j].average
        ld $24, r7
        add r7, r2
        bgt r6, body
        inc r5
        br inner

body: ld $24, r7
      not r7
      inc r7
      add r7, r2
      ld 24(r2), r1 #r1 = s[j].sid
      ld 0(r2), r7 #r7 = s[j-1].sid
      st r1, 0(r2) #s[j-1].sid = s[j].sid
      st r7, 24(r2)    #s[j].sid = s[j-1].sid
      ld 28(r2), r1 #r1 = s[j].sid
      ld 4(r2), r7 #r7 = s[j-1].sid
      st r1, 4(r2) #s[j-1].sid = s[j].sid
      st r7, 28(r2)    #s[j].sid = s[j-1].sid
      ld 32(r2), r1 #r1 = s[j].sid
      ld 8(r2), r7 #r7 = s[j-1].sid
      st r1, 8(r2) #s[j-1].sid = s[j].sid
      st r7, 32(r2)    #s[j].sid = s[j-1].sid
      ld 36(r2), r1 #r1 = s[j].sid
      ld 12(r2), r7 #r7 = s[j-1].sid
      st r1, 12(r2) #s[j-1].sid = s[j].sid
      st r7, 36(r2)    #s[j].sid = s[j-1].sid
      ld 40(r2), r1 #r1 = s[j].sid
      ld 16(r2), r7 #r7 = s[j-1].sid
      st r1, 16(r2) #s[j-1].sid = s[j].sid
      st r7, 40(r2)    #s[j].sid = s[j-1].sid
      ld 44(r2), r1 #r1 = s[j].sid
      ld 20(r2), r7 #r7 = s[j-1].sid
      st r1, 20(r2) #s[j-1].sid = s[j].sid
      st r7, 44(r2)    #s[j].sid = s[j-1].sid
      ld $24, r7
      add r7, r2
      inc r5
      br inner

median: ld $n, r2
        ld (r2), r2
        inc r2
        shr $1, r2
        dec r2
        mov r2, r6
        mov r2, r7
        shl $3, r6
        shl $4, r7
        add r6, r2
        add r7, r2 #r2 *= 24
        ld (r0), r5
        ld (r5, r2, 4), r4
        ld $m, r3
        st r4, (r3)

halt



.pos 0x2000
n:    .long 3
m:    .long 0
s:    .long base
base: .long 1234
      .long 80
      .long 60
      .long 78
      .long 90
      .long 0
      .long 5678
      .long 65
      .long 90
      .long 95
      .long 95
      .long 0
      .long 8748
      .long 34
      .long 20
      .long 09
      .long 79
      .long 0