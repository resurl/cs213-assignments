.pos 0x1000
  ld $0, r0   # r0 = fd = 0 (standard in)
  ld $buf, r1 # r1 = buf
  ld $64, r2  # r2 = size
  sys $0      # system call: read

  mov r0, r1  # r1 = size = read return value
  ld $buf, r0 # r0 = buf
  sys $2      # system call: exec
  halt

.pos 0x2000
buf:
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

0000 0000 0000
0100 0000 001e
0200 0000 0008
f101
6001
0000 0000 001e
f102
2f62696e2f736800