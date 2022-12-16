.z80
.area TEST(ABS)

.org	0x0000
start:
	jp	main1

.org	0x0100
main1:
	jp  start