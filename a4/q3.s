# void foo() {
#  v0 = s.x[i]; line 1
#  v1 = s.y[i]; line 2
#  v2 = s.z->x[i];
#  v3 = s.z->z->y[i];
# }

.pos 0x1000
code: ld $s, r0             # r0 = addr of s 
      ld $i, r3             # r3 = addr i
      ld (r3), r3           # r3 = val i

      ld $v0, r1            # r1 = addr of v0 
      ld (r0,r3,4), r2      # r2 = val s.x[i]        4 accesses
      st r2, (r1)           # v0 = s.x[i]

      ld $v1, r1            # r1 = addr v1
      ld 8(r0), r2          # r2 = s[2] (addr y)
      ld (r2,r3,4),r2       # r2 = val s.y[i]        5 accesses
      st r2, (r1)           # v1 = s.y[i]

      ld $v2, r1            # r1 = addr v2
      ld 12(r0), r2         # r2 = s[3] (addr s.z)
      ld (r2,r3,4), r2      # r2 = s.z->x[i] (ie. s.z->x[0] or s.z->x[1])
      st r2, (r1)           # v2                     5 accesses
      
      ld $v3, r1
      ld 12(r0), r2         # s.z = &s.z->x[0]
      ld 12(r2), r2         # s.z->z = &s.j->z->x[0]
      ld 8(r2), r2          # s.z->z->y = &s.z->z->y[0]
      ld (r2, r3, 4), r2    # r2 = s.z->z->y[i]      7 accesses
      st r2, (r1)           # v3
      halt

.pos 0x2000
static:
i:  .long 0
v0: .long 0
v1: .long 0
v2: .long 0
v3: .long 0
s:  .long 56    # x[0]
    .long 64    # x[1]
    .long s_y
    .long s_z

.pos 0x3000
heap:
s_y:   .long 31
       .long 522
s_z:   .long 30  
       .long 403
       .long 0
       .long s_z_z
s_z_z:  .long 0
       .long 0 
       .long s_z_z_y
       .long 0
s_z_z_y: .long 442 
         .long 329