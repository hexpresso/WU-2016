# Nuit du Hack Qualifications CTF 2016: Matriochka - Step 4

**Category:** Crack Me |
**Points:** 500 |
**Solves:** 49 |
**Description:**

> You must solve steps [1](../Matriochka1/), [2](../Matriochka2/) and [3](../Matriochka3/) first.

## Write-up

This task is a reverse-engineering task.
The binary is obtained from Matriochka - Step 3 once the user provides the good
flag.

```
$ file stage4
stage4: DOS/MBR boot sector
```
Wellcome to **hell**.

We are given a MBR for this task. MBR stands for
[Master Boot Record](https://en.wikipedia.org/wiki/Master_boot_record).

When your computer boots, the BIOS runs, and jumps to your MBR. It is the first
sector on your hard drive.

To ensure compatibility, bootstrap code is not in 32 bits or 64 bits. It uses
[Real mode](https://en.wikipedia.org/wiki/Real_mode).

One can debug with qemu + gdb, using the following commands:
```sh
qemu-system-x86_64 stage4 -s -S -monitor stdio

gdb
    target remote localhost:1234
    set architecture i8086
    display /i ($cs * 0x10) + $pc
    b *0x7C00
	c
```
The machine can be reset with system_reset in the monitor console.

### Part 1: MBR

First thing we'll do is extract the first disk sector:
```sh
dd if=../stage4 bs=512 count=1 of=boot.mbr
```

Once imported with base address 0x7C00, we can see three functions at 0x7C02,
0x7C0E, and 0x7C1B. Those functions pop the return address, adds repectively
1, 2, or 4 before pushing it back. Therefore, I called them skipByte, skipShort
and skipLong.
These functions are overused to obfuscate code as much as possible.

The bootloader code doesn't do anything fancy. It justs sets segment registers,
and sends an interuption [13H (DISK)](https://en.wikipedia.org/wiki/INT_13H)
to read 0x32 sectors from the disk at 0x1000. It then jumps to it. (It is
required since BIOS only reads first sector)

### Part 2: Second sector

We first thought that every remaining sectors were treated equally. We were
wrong. In fact, the 2 next sectors are slightly different than the last sectors
(we'll see why later)

Once again, dump it, and load it at 0x1000 in your favourite disassembler:
```sh
dd if=../stage4 bs=512 count=2 skip=1 of=disk2
```

Our friends skipByte, skipShort and skipLong are here again. Get ready for more
obfuscation! Just like the previous part, static analysis is the way to go (you
can use gdb to help). It's a bunch of calls to skipXxx and jmps, but there is
no conditional branching.

There are 2 functions: One that prints a string at 0x107C, and one that reads
user input at 0x1117.

The string is "What's the magic word ?", and the user input is stored at 0x1003

The reason why we decided to analyze those sectors indepently than the other is
that once the input is read, the code sets your processor in protected mode
(= "normal" mode). It then jumps to 0x1400 (the 4th sector)

### Part 3: Last sectors

This part is much more pleasant, because it's ~~not~~ less obfuscated, and it's
what reversers are used to! We can even use gdb to see what's really going
on ! :)

Once again, dump, load at 0x1400, read code.

A pointer of the password is stored at 0x19C7. First thing we noticed is that
there are cross refs to this address. The code compares part of our input with
ascii chars each time.

```ASM
mov     ebx, ds:input
mov     cl, [ebx]
cmp     cl, 47h ; 'G'
jnz     short loc_14C4
inc     ebx
mov     cl, [ebx]
cmp     cl, 6Fh ; 'o'
jnz     short loc_14C4
inc     ebx
mov     cl, [ebx]
cmp     cl, 6Fh ; 'o'
jnz     short loc_14C4
```

**Flag**: Good_Game_!

Wait, no, where are my points? It's a wrong flag! :(

Back to the xrefs, there are a few more that we didn't peek yet. They contain
non-printable chars : our serial is probably altered.

We set a watchpoint with gdb on our serial (at 0x1003) to see what writes
there : code at 0x167E xors it with a key in the memory. It uses the same
routine to decrypt the good, and bad boy.

Our solution was to fill the input with 'a' (0x41), and set a breakpoint on
each block that compared with the right flag.

Each block is composed of sub-blocks like this:
```ASM
add     ebx, 2
mov     cl, [ebx]
cmp     cl, 5Bh ; '['
jnz     short loc_16CD
```

It checks input[k += 2] with 0x5B. Since our input is xored with a string, we
could retrieve the char of each characters with:
`flag[i] = 'a' ^ check[i] ^ input[i]`

**Flag**: Ddr1ml/frf
