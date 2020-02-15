.pos 0x1000
code:   ld $v0, r0
        ld $i, r1
        ld (r1), r1
        ld $s, r2               #addr of s_x
        ld (r2), r3             # addr of x
        ld (r3,r1,4), r4        # value of x[i]
        st r4, (r0)             # v0

        ld $v1, r0
        ld $0x2020,r5             # pointer to s_y
        ld (r5), r5             # addr of s_y
        ld (r5), r5             # addr of data
        ld (r5,r1,4), r6        # value s_y[i]
        st r6, (r0)             # v1

        ld $v2, r0
        ld $0x2024, r5          # pointer to s_z
        ld (r5), r5             # addr of struct S z
        ld (r5), r5
        ld (r5), r5
        ld (r5,r1,4), r5             # value of s_z_x[i]
        st r5, (r0)             #v2 
        
        ld $v3, r0
        ld $0x2024, r5          # addr s_z
        ld (r5), r5             
        ld (r5), r5
        ld $2, r6              
        ld (r5,r6,4), r5        # addr s_z_z
        ld $1, r6 
        ld (r5,r6,4), r5        # value s_z_z_y
        ld (r5), r5
        ld (r5,r1,4), r5             # value of szzy[1]
        st r5, (r0)
        halt
        
        
.pos 0x2000
static: 
i:  .long 0             #0x2000
        v0: .long 0
        v1: .long 0
        v2: .long 0
        v3: .long 0
        x: .long 5
           .long 90
        s: .long x
           .long s_y    #0x2020
           .long s_z
.pos 0x3000
heap:   
s_y: .long data
     .long 0
s_z: .long z 
z: .long s_z_x
    .long s_z_y
    .long s_z_z
s_z_x: .long 999
       .long 998
s_z_y: .long 0
s_z_z: .long s_z_z_x
       .long s_z_z_y
       .long s_z_z_z
s_z_z_x: .long 0
s_z_z_y: .long data2
s_z_z_z: .long 0


.pos 0x4000
data: .long 3912
      .long 134
      .long 321
      .long 3493
      .long 590
      .long 23

data1: .long 312
       .long 39
       .long 93
       .long 1
       .long 40

data2: .long 6
       .long 7