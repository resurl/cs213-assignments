.pos 0x1000
start:
  ld $stacktop, r5    # after end of memory
  gpc $6, r6          
  j main
  halt

main:
  deca r5             # alloc 4 bytes
  st r6, (r5)         # ra
  ld $-128, r0        
  add r0, r5          # alloc 128 bytes 
  deca r5             # alloc 4 bytes
  ld $str1, r0        # r0 = &str1
  st r0, (r5)         # a0 = &str1
  gpc $6, r6          
  j print             # goto print
  inca r5             # dealloc 4 bytes
  ld $0, r0           # r0 = 0
  mov r5, r1          # r1 = stack pointer
  ld $256, r2         # r2 = 256 bytes
  sys $0              # system call: read(0,stack,256)
  mov r0, r4          # r4 = result of read (name)
  deca r5             # alloc 4 bytes
  ld $str2, r0        # r0 = &str2
  st r0, (r5)         # save &str2 to stack
  gpc $6, r6          
  j print             # goto print
  inca r5             # dealloc 4 bytes
  ld $1, r0           # r0 = 1
  mov r5, r1          # r1 = stack pointer
  mov r4, r2          # r2 = name/input
  sys $1              # system call: write
  ld $128, r0         
  add r0, r5          # dealloc 128 bytes
  ld (r5), r6         # r6 = ra
  inca r5             # empty stack
  j (r6)              # exit main

print:
  ld (r5), r0         # r0 = &str1 
  ld 4(r0), r1        # r1 = &_str1
  ld (r0), r2         # r2 = 30
  ld $1, r0           # r0 = 1
  sys $1              # system call: write(1,_str1,30)
  j (r6)              # goto main

.pos 0x1800
proof:
  deca r5
  ld $str3, r0
  st r0, (r5)
  gpc $6, r6
  j print
  halt

.pos 0x2000
str1:
  .long 30
  .long _str1
_str1:
  .long 0x57656c63      # Welc
  .long 0x6f6d6521      # ome!
  .long 0x20506c65      #  Ple
  .long 0x61736520      # ase 
  .long 0x656e7465      # ente
  .long 0x72206120      # r a 
  .long 0x6e616d65      # name
  .long 0x3a0a0000      # :\n

str2:
  .long 11
  .long _str2
_str2:
  .long 0x476f6f64      # Good
  .long 0x206c7563      #  luc
  .long 0x6b2c2000      # k,  

str3:
  .long 43
  .long _str3
_str3:
  .long 0x54686520      # The 
  .long 0x73656372      # secr
  .long 0x65742070      # et p
  .long 0x68726173      # hras
  .long 0x65206973      # e is
  .long 0x20227371      #  "sq
  .long 0x7565616d      # ueam
  .long 0x69736820      # ish 
  .long 0x6f737369      # ossi
  .long 0x66726167      # frag
  .long 0x65220a00      # e"\n

.pos 0x4000
stack:
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
  .long 0
stacktop:
  .long 0
