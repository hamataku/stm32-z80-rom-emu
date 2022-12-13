from intelhex import IntelHex
import subprocess
import os

dirname = os.path.dirname(os.path.abspath(__file__))

# asm build
print("\033[32mz80 asm build\033[0m")
subprocess.run('sdasz80 -l -s -o main.asm', cwd=dirname, shell=True)
subprocess.run('sdldz80 -i main', cwd=dirname, shell=True)

ih = IntelHex() 
ih.loadhex(dirname + '/main.ihx')
f = open(dirname + '/rom.c', 'w')

f.write('#include "rom.h"\n')
f.write('const uint8_t rom_data[32768] = {\n')

for address in range(32768):
    if address in ih.addresses():
        f.write('\t0x%02x, // 0x%02x\n' % (ih[address], address))
    else:
        f.write('\t0x00, // 0x%02x\n' % address)
f.write('};')
f.close()