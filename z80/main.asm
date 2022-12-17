.z80
.area TEST(ABS)

.org	0x0000
main:
  ld a,#0xc3
  ld hl,#0x8000
  ld (hl),a
  ld a,#0x00
  ld hl,#0x8001
  ld (hl),a
  ld a,#0x00
  ld hl,#0x8002
  ld (hl),a
  jp 0x8000