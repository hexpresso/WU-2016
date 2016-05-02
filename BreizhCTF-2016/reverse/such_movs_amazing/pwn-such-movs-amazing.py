from pwn import *
import operator
import pprint

# Use this before reading: https://en.wikipedia.org/wiki/Eye_drop

context(arch='i386',os='linux')

pp = pprint.PrettyPrinter(indent=2)

def find_where(elf, _addr):
    start = None
    start_func = None
    offsets = sorted({v: k for k, v in elf.symbols.items()}.items(), key=operator.itemgetter(0))
    for addr, func in offsets:
        if start is not None and addr > _addr:
            return start_func
        else:
            if start < addr:
                start = addr
                start_func = func


log.info("Opening ./bin/such_movs_amazing")
elf  = ELF("./bin/such_movs_amazing")

log.info("Load addr: 0x%x" % elf.load_addr)

main           = elf.symbols['main']
verbose_cipher = elf.symbols['verbose_cipher']
silent__cipher = elf.symbols['silent__cipher']

log.info("main:           0x%x" % main)
log.info("verbose_cipher: 0x%x" % verbose_cipher)
log.info("silent__cipher: 0x%x" % silent__cipher)

offset = 0x80000000

for i in elf.search(p32(offset + silent__cipher)):
    where = find_where(elf, i)
    log.info("Found ref to silent__cipher in %s" % where)
    if where == 'main':
        elf.write(i, p32(offset + verbose_cipher))
        log.success("Patching %s to use verbose_cipher" % where)

log.success("Writing such_movs_amazing.new")
elf.save("./bin/such_movs_amazing.patched")
