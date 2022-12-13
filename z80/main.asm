.z80
.area TEST(ABS)

.org	0x0000
start:
	jp	main

.org	0x020
main:
	jp  start