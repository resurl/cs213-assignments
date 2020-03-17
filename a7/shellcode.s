ld $0x2f62696e, r6
ld $0x2f736800, r7
ld $0x24, r0
st r6, (r0)
st r7, 4(r0)
ld $8, r1
sys $2
