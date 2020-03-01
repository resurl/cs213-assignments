.pos 0x1000
ld $s, r0           # r0 = &s 
ld (r0), r1         # r1 = s 
ld $n, r2
ld (r2), r2         # r2 = n
ld $0, r3           # i for increment

avgloop: mov r3, r4 # temp i
not r4
inc r4
add r2, r4          # n-i
bgt r4, avg         # if i<n goto avg
mov r2, r3          # reset i (i = n)
dec r3              # i = n-1 (for sort)
br sort             # else goto sort

avg: ld $0, r6      # r6 = 0
     ld 4(r1), r5   # s[i].grade[0]
     add r5, r6
     ld 8(r1), r5   # s[i].grade[1]
     add r5, r6
     ld 12(r1), r5  # s[i].grade[2]
     add r5, r6
     ld 16(r1), r5  # s[i].grade[3]
     add r5, r6
     shr $2, r6     # r6 = grade sum / 4
     st r6, 20(r1)  # s[i].average = grade >> 2
     ld $24, r6     # r6 = 24 (byte offset)
     add r6, r1     # next student
     inc r3         # i++ 
     br avgloop     # calculate avg of s[i]

sort: ld (r0), r2   # &s[0]
      ld $1, r5     # j
      bgt r3, inner # if i > 0, goto inner (loop)
      br median     # else get student id w median avg

inner: mov r5, r4   # j for comparison
       not r4
       inc r4       # -j
       add r3, r4    # i-j
       ld $24, r1   # r1 = 24
       add r1, r2   # &s[j]
       bgt r4, sortif 
       beq r4, sortif #if (i-j) >= 0 goto sortif
       dec r3
       br sort #else go to sort

sortif: ld 20(r2), r6   # r6 = s[j].average
        not r6
        inc r6          # r6 = -s[j].average
        ld $24, r7      # r7 = 24 for offset
        not r7
        inc r7
        add r7, r2      # r2 = &s[j-1]
        ld 20(r2), r7   # r7 = s[j-1].average
        add r7, r6      # r6 = s[j-1].average - s[j].average
        ld $24, r7      
        add r7, r2      # move s[j-1] pointer up
        bgt r6, body    # if (s[j-1] average > s[j].average) goto body (inside if)
        inc r5          # else j++ 
        br inner        # goto inner (loop, beginning)

body: ld $24, r7        
      not r7
      inc r7
      add r7, r2
      ld 24(r2), r1     #r1 = s[j].sid
      ld 0(r2), r7      #r7 = s[j-1].sid
      st r1, 0(r2)      #s[j-1].sid = s[j].sid
      st r7, 24(r2)     #s[j].sid = s[j-1].sid
      ld 28(r2), r1     #r1 = s[j].sid
      ld 4(r2), r7      #r7 = s[j-1].sid
      st r1, 4(r2)      #s[j-1].sid = s[j].sid
      st r7, 28(r2)     #s[j].sid = s[j-1].sid
      ld 32(r2), r1     #r1 = s[j].sid
      ld 8(r2), r7      #r7 = s[j-1].sid
      st r1, 8(r2)      #s[j-1].sid = s[j].sid
      st r7, 32(r2)     #s[j].sid = s[j-1].sid
      ld 36(r2), r1     #r1 = s[j].sid
      ld 12(r2), r7     #r7 = s[j-1].sid
      st r1, 12(r2)     #s[j-1].sid = s[j].sid
      st r7, 36(r2)     #s[j].sid = s[j-1].sid
      ld 40(r2), r1     #r1 = s[j].sid
      ld 16(r2), r7     #r7 = s[j-1].sid
      st r1, 16(r2)     #s[j-1].sid = s[j].sid
      st r7, 40(r2)     #s[j].sid = s[j-1].sid
      ld 44(r2), r1     #r1 = s[j].sid
      ld 20(r2), r7     # r7 = s[j-1].sid
      st r1, 20(r2)     #s[j-1].sid = s[j].sid
      st r7, 44(r2)     #s[j].sid = s[j-1].sid
      ld $24, r7
      add r7, r2
      inc r5
      br inner

median: ld $n, r2       
        ld (r2), r2     # r2 = n
        inc r2          
        shr $1, r2      # r2 = (n + 1) / 2
        dec r2          # ((n + 1) / 2) - 1
        mov r2, r6      # r6 = middle 
        mov r2, r7      # r7 = middle
        shl $3, r6      # r6 = middle*8
        shl $4, r7      # r7 = middle*16
        ld $0, r2
        add r6, r2
        add r7, r2      # r2 *= 24
        shr $2, r2
        ld (r0), r5     # &s[0]
        ld (r5, r2, 4), r4 #s[middle]
        ld $m, r3
        st r4, (r3)     # m = s[middle].sid

halt



.pos 0x2000
n:    .long 7
m:    .long 0
s:    .long base
base: .long 1111
      .long 99
      .long 99
      .long 99
      .long 99
      .long 0
      .long 2222
      .long 88
      .long 88
      .long 88
      .long 88
      .long 0
      .long 3333
      .long 77
      .long 77
      .long 77
      .long 77
      .long 0
      .long 4444
      .long 66
      .long 66
      .long 66
      .long 66
      .long 0
      .long 5555
      .long 55
      .long 55
      .long 55
      .long 55
      .long 0
      .long 6666
      .long 44
      .long 44
      .long 44
      .long 44
      .long 0
      .long 7777
      .long 33
      .long 33
      .long 33
      .long 33
      .long 0