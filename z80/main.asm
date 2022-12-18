.z80
.area TEST(ABS)

.org	0x0000
start:
  ld a,#0b00001111 ; PIOモード0
  out (0x1d),a
  jp main

.org	0x100
main:
  ld a,#0x00
  out (0x1c),a
  ld bc,#0xffff
  call loop
  ld a,#0x01
  out (0x1c),a
  ld bc,#0xffff
  call loop
  jp main

loop:
  dec bc
  ld a,b
  or c
  jr nz,loop
  ret


  