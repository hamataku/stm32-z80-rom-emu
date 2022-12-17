.z80
.area TEST(ABS)

.org	0x0000
start:
  ld a,#0b00001111 ; PIOモード0
  out (0x1d),a
  jp main

; http://www.apony.com/elec/elec10/elec10.html

.org	0x100
main:
  ld a,#0x00
  out (0x1c),a
  ld a,#0x01
  out (0x1c),a
  jp main