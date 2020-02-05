#int  i,j;
#int* p;
#int  a[10];
#  i  = a[3];
#  i  = a[i];
#  p  = &j;
#  *p = 4;
#  p  = &a[a[2]];
#  *p = *p + a[4];

  ld $i, r0  #address of i in r0
  ld $j, r1 #address of j in r1
  ld $a, r2 # base address of a
  ld 12(r2), r3 #load value of a[3] into r3
  st r3, (r0) # store a[3] into in
  ld (r2, r3, 4), r4 #store the value of a[i] into r4
  st r4, (r0) # store the value of a[i] into i  
  ld $p, r5 #load the address of p into r5
  ld $4, r6 #loading 4 into r6
  st r6, (r1) #storing r6 into j
  ld 8(r2), r7 #load a[2] into r7 this is i
  ld 16(r2), r0 # load a[4] into r0, overwrite r0
  add r7, r2
  add r7, r2
  add r7, r2
  add r7, r2 #r2 is address of a[a[2]]
  st r2, (r5)
  ld (r2), r6
  add r6, r0 #sum in r0
  st r0, (r2)
  halt

  
  









  .pos 0x100
  i: .long 0
  j: .long 25
  p: .long j

  .pos 0x1000
  a: .long 1
     .long 2
     .long 3
     .long 4
     .long 5
     .long 6
     .long 7
     .long 8
     .long 9
     .long 0
