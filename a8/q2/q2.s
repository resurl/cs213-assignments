.pos 0x0
                 ld   $0x1028, r5             # r5 = below stack       
                 ld   $0xfffffff4, r0         # r0 = -12
                 add  r0, r5                  # alloc 12 bytes
                 ld   $0x200, r0              # r0 = &array[0]
                 ld   0x0(r0), r0             # r0 = array[0]
                 st   r0, 0x0(r5)             # save array[0] as a2  
                 ld   $0x204, r0              # r0 = &array[1]
                 ld   0x0(r0), r0             # r0 = array[1]
                 st   r0, 0x4(r5)             # save array[1] as a1 
                 ld   $0x208, r0              # r0 = &array[2]
                 ld   0x0(r0), r0             # r0 = array[2]
                 st   r0, 0x8(r5)             # save array[2] as a0
                 gpc  $6, r6                  # r6 = &return
                 j    0x300                   #goto fn
                 ld   $0x20c, r1              # r1 = array[3]
                 st   r0, 0x0(r1)             # array[3] = r0
                 halt
.pos 0x200
                 .long 0x0000000a           # array[0]
                 .long 0x0000000a
                 .long 0x0000000a
                 .long 0x00000000
.pos 0x300
                 ld   0x0(r5), r0           # array[0]
                 ld   0x4(r5), r1
                 ld   0x8(r5), r2
                 ld   $0xfffffff6, r3       # r3 = -10
                 add  r3, r0                # r0 = array[0] - 10
                 mov  r0, r3                # r3 = r0
                 not  r3
                 inc  r3                    # r3 = 10 - array[0]
                 bgt  r3, L6                # if array[0] < 10 goto L6
                 mov  r0, r3                # r0 = array[0] - 10
                 ld   $0xfffffff8, r4       # r4 = -8
                 add  r4, r3                # r3 = array[0] - 18
                 bgt  r3, L6                # if array[0] > 18 goto L6
                 ld   $0x400, r3            # r3 = &jumptable
                 ld   (r3, r0, 4), r3       # r3 = jumptable[i-10]
                 j    (r3)                  # goto jumptable[i-10]
.pos 0x330
case10:          add  r1, r2                #r2 = array[1] + array[2]
                 br   L7                    # goto L7
case12:          not  r2                    
                 inc  r2                    # -(array[2])
                 add  r1, r2                # r2 = array[1] - array[2]
                 br   L7                    # goto L7
case14:          not  r2
                 inc  r2                    # r2 = -array[2]
                 add  r1, r2                # r2 = array[1] - array[2]
                 bgt  r2, L0                # if array[1] > array[2] goto L0
                 ld   $0x0, r2              # else r2 = 0
                 br   L1                    # goto L1
L0:              ld   $0x1, r2              # r2 = 1
L1:              br   L7                    # goto L7
case16:          not  r1                    
                 inc  r1                    # r1 = -array[1]
                 add  r2, r1                # r1 = array[2] - array[1]
                 bgt  r1, L2                # if array[2] > array[1] goto L2
                 ld   $0x0, r2              # else r2 = 0
                 br   L3                    # goto L3
L2:              ld   $0x1, r2              # r2 = 1
L3:              br   L7                    # goto L7
case18:          not  r2                    
                 inc  r2                    # r2 = -array[2]
                 add  r1, r2                # r2 = array[1] - array[2]
                 beq  r2, L4                # if array[1] == array[2] goto L4
                 ld   $0x0, r2              # r2 = 0
                 br   L5                    #goto L5
L4:              ld   $0x1, r2              #r2 = 1
L5:              br   L7                    #goto L7
L6:              ld   $0x0, r2              #r2 = 0
                 br   L7                    #goto L7
L7:              mov  r2, r0                #r0 = r2
                 j    0x0(r6)               #return to main
.pos 0x400
                 .long 0x00000330           # jumptable 10 
                 .long 0x00000384           # default 11
                 .long 0x00000334           # 12
                 .long 0x00000384           # default 13
                 .long 0x0000033c           # 14
                 .long 0x00000384           # default 15 
                 .long 0x00000354           # 16
                 .long 0x00000384           # default 17
                 .long 0x0000036c           # 18
.pos 0x1000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000           # start of stack
