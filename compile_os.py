import os

CC_GCC = '/usr/local/x86_64elfgcc/bin/x86_64-elf-gcc'
CC_LD = '/usr/local/x86_64elfgcc/bin/x86_64-elf-ld'

# compile sector 1
os.chdir('BootLoader/sector1')
os.system('nasm {0}.asm -f bin -o ../bin/{0}.bin'.format('ohnos_bootloader'))

# compile sector 2
os.chdir('../sector2')
os.system('nasm {0}.asm -f elf64 -o ../bin/{0}.o'.format('ohnos_sector2'))

# compile kernel
os.chdir('../../Kernel')
os.system(CC_GCC + ' -Ttext 0x8000 -ffreestanding -mno-red-zone -m64 -c ohnos_kernel.c -o bin/ohnos_kernel.o')
os.system(CC_GCC + ' -ffreestanding -mno-red-zone -m64 -c ohnos_kb_scancodes.c -o bin/ohnos_kb_scancodes.o')
os.system(CC_GCC + ' -ffreestanding -mno-red-zone -m64 -c ohnos_idt.c -o bin/ohnos_idt.o')

# compile drivers
os.chdir('../Drivers')
os.system(CC_GCC + ' -ffreestanding -mno-red-zone -m64 -c ohnos_io_driver.c -o bin/ohnos_io_driver.o')
os.system(CC_GCC + ' -ffreestanding -mno-red-zone -m64 -c ohnos_screen_driver.c -o bin/ohnos_screen_driver.o')

# link
os.chdir('..')
os.system(CC_LD + ' -T link.ld')

# create the final bootloader binaries
os.system('cat BootLoader/bin/ohnos_bootloader.bin ohnos_kernel.bin > os-image/OhNOS.bin')

# run the os
os.chdir('os-image')
os.system('qemu-system-x86_64 -drive format=raw,file=OhNOS.bin')
