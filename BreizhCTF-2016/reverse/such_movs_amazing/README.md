# BreizhCTF 2016 - Such Movs Amazing
**Category:** Reverse |
**Points:** 350 |
**Solves:** 0 |
**Description:**

> _Description is missing, feel free to send a pull request_


```
 ____  _   _  ____ _   _       __  __  _____     ______    _
/ ___|| | | |/ ___| | | |     |  \/  |/ _ \ \   / / ___|  | |
\___ \| | | | |   | |_| |     | |\/| | | | \ \ / /\___ \  | |
 ___) | |_| | |___|  _  |     | |  | | |_| |\ V /  ___) | |_|
|____/ \___/ \____|_| |_|     |_|  |_|\___/  \_/  |____/  (_)
                                    `
                                   :,
           `                      `:,`
          :::,                    ::,`
         `;;;:,                  .;:,`
         ,'';::.                 ;;:,`
         ;';::::.               :;;:,`
         '';;;:;:.             .;;;:,.
         ;;;;;;:;:,          .,::;;;:,
         ;;;';;;;;:,,,,::::::::::,,:;,
         ;;'''';;';;;::::::::::::,,,,:`
         ;;;++';;;';;::,,::::::::,,,,,,`
         ;;;++';;;'':::::::::::::,,,,,,,,`
         ,;;'+#;;;;::::::::::::;:,,..,,,,,,`
         .;:;++#;;::::::::::::::;:,...,,,,,.`
         .';;;++':::::::::,,,:::;:,,,,,,,....`
         .:';;;;::::::::::,,,,:::::,,,.,'#@@@@
         .;;;;::,,,::::::,,,,,,::'+@@@@@@@@@@@.
         `';;;:,,,::::::,,,;+@@@@@@,'+ @@@@@@,
          '';:,,,,,:;+@@@@@@@@@@@@@ .:@ +@@@',`
          ';:,;+@@@@@@:@@@@@@@@@,,@@@ @'@@@@``.
         #@@@@@@#@@ ;+ #@@@@@@@@,,.:@@@@#;..``.
         #+;:,,:,@@+; @ :'@@@@@:,,,..,,,,,,.````
       @@;:::,,,,,@#@@ @+ @@@#::,,,...,,..,.```.
         ,::,,,,,,.:@@;@@@@@@;::,,,.,,,,,..`````
         ::,,,,,,,,..@':,,,:,,,,,,..,,:;;;:.````
         :,,,,,,.,.....,,,:,,,,,,,..,;++++++.`..`
         :,,,,........,,,....,,,,...,+##@###,`..`
         ,:,,,,..............,,,,...,+#@###',...`
         .::,,,,,,....,.......,,,,,.,'#####+:...`
         .::,,,,,,,.......,,..,,,,,,,;'####':,...
          ::,,,,,,,,,,.,,,,,,.,,,,,:,:'++#+';,..`
          :::,,,,,,,,,,,,,,,,,,,,,,,:;'++''';,..`
          :::,,,,,,,,,,,,,,,,:::,::::;'+#@#+:,..`
          `:,,,,..,,,,,,,,,,,:;;:::::+###++',...`
           ::,,,,,,,,,,,,,,,,:;'+++'#+++++':....
           ,:,,,,,,,,,,,,,,,,,:;;''++''''':,....
            :::,,,,,,,,,,,,,,,,::;;:;::;;;,,....
            ::::,,,,,,,,,,,,,,,,,,:::,:,:,,,...`
            .::::,,,,,,,,,,,::,:,:,,,,,,,,,,...
              ::,:,,,,,,,,,,:,:,:,,,,,,,,,,,,`
               ::,,,,:,,,,,,::::::,,,,,,,,,,,
                ,,,,,,,:,:,::::::,:,,,,,,,,.
                 .,:,,,,:::::::::,:,,,,,,,.
                   .,,,,:::::::::::,::::,
                     `.,,,:::::::::::::`
                         `.,,::::::,`
   ____   ___       __  _____   __     _
  / ___| / _ \      \ \/ ( _ ) / /_   | |
  \___ \| | | |      \  // _ \| '_ \  | |
   ___) | |_| |      /  \ (_) | (_) | |_|
  |____/ \___/      /_/\_\___/ \___/  (_)
      _    __  __    _     ________ _   _  ____       _
     / \  |  \/  |  / \   |__  /_ _| \ | |/ ___|     | |
    / _ \ | |\/| | / _ \    / / | ||  \| | |  _      | |
   / ___ \| |  | |/ ___ \  / /_ | || |\  | |_| |     |_|
  /_/   \_\_|  |_/_/   \_\/____|___|_| \_|\____|     (_)
```

## Recon
For this challenge, we're given a 12MiB binary.

Launching *such_movs_amazing* reveals some kind of hidden computation:

```bash
[pix:such_movs_amazing] % ./such_movs_amazing
[......8<......snip......8<......]
a[0] = '0x** | (Encrypted value hidden due to security reasons)'
b[0] = '0x44'
c[0] = '0x44'
a[1] = '0x** | (Encrypted value hidden due to security reasons)'
b[1] = '0x73'
c[1] = '0xcc'
a[2] = '0x** | (Encrypted value hidden due to security reasons)'
b[2] = '0x78'
c[2] = '0x86'
a[3] = '0x** | (Encrypted value hidden due to security reasons)'
b[3] = '0x7e'
c[3] = '0xe'
```

A quick look using objdump is going to reveal the horrendous truth:
```ObjDump
[pix:such_movs_amazing] % gdb -batch -ex 'file ./bin/such_movs_amazing' -ex 'disassemble main'
Dump of assembler code for function main:
   0x0804cdb5 <+0>:	    mov    0x8413b58,%eax
   0x0804cdba <+5>:	    mov    $0x8804cdb5,%edx
   0x0804cdbf <+10>:	mov    %eax,0x82139e0
   0x0804cdc4 <+15>:	mov    %edx,0x82139e4
   0x0804cdca <+21>:	mov    $0x0,%eax
   0x0804cdcf <+26>:	mov    $0x0,%ecx
   0x0804cdd4 <+31>:	mov    $0x0,%edx
   0x0804cdd9 <+36>:	mov    0x82139e0,%al
   0x0804cdde <+41>:	mov    0x806dff0(,%eax,4),%ecx
   0x0804cde5 <+48>:	mov    0x82139e4,%dl
   [......8<......snip......8<......]
```

The source code has been compiled using the excelent [movfuscator](https://github.com/xoreaxeaxeax/movfuscator) compiler. Reversing this kind of assembly is going to be a nightmare.

```ObjDump
[pix:such_movs_amazing] % objdump -Mintel -d bin/such_movs_amazing | grep '<.*>:'
08048210 <printf@plt-0x10>:
08048220 <printf@plt>:
08048230 <dprintf@plt>:
08048240 <perror@plt>:
08048250 <exit@plt>:
08048260 <sigaction@plt>:
08048270 <dispatch>:
0804827c <_start>:
080482f7 <_start0>:
080487a4 <silent__cipher>:
080495bb <verbose_cipher>:
0804aa7a <verify_cipher_is_sane_before>:
0804cdb5 <main>:
```

This is interesting ! We have two functions, `silent__cipher` and `verbose_cipher`. What happens if we patch `such_movs_amazing` to use the later ?


## Pwntools

After a small look at objdump output we can grasp the following informations:

```ObjDump
[pix:such_movs_amazing] % objdump -Mintel -d ./bin/such_movs_amazing | egrep --color -C1 '<.*>:|80487a4|80493d2|80495bb'
--
080487a4 <silent__cipher>:
 80487a4:	a1 58 3b 41 08       	mov    eax,ds:0x8413b58
 80487a9:	ba a4 87 04 88       	mov    edx,0x880487a4
 80487ae:	a3 e0 39 21 08       	mov    ds:0x82139e0,eax
--
080495bb <verbose_cipher>:
 80495bb:	a1 58 3b 41 08       	mov    eax,ds:0x8413b58
 80495c0:	ba bb 95 04 88       	mov    edx,0x880495bb
 80495c5:	a3 e0 39 21 08       	mov    ds:0x82139e0,eax
--
 8049d38:	89 10                	mov    DWORD PTR [eax],edx
 8049d3a:	b8 a4 87 04 88       	mov    eax,0x880487a4
 8049d3f:	a3 f0 3a 21 08       	mov    ds:0x8213af0,eax
--
0804aa7a <verify_cipher_is_sane_before>:
 804aa7a:	a1 58 3b 41 08       	mov    eax,ds:0x8413b58
--
 804b534:	89 10                	mov    DWORD PTR [eax],edx
 804b536:	b8 a4 87 04 88       	mov    eax,0x880487a4
 804b53b:	a3 f0 3a 21 08       	mov    ds:0x8213af0,eax
--
0804cdb5 <main>:
 804cdb5:	a1 58 3b 41 08       	mov    eax,ds:0x8413b58
--
 8053f0e:	89 10                	mov    DWORD PTR [eax],edx
 8053f10:	b8 a4 87 04 88       	mov    eax,0x880487a4
 8053f15:	a3 f0 3a 21 08       	mov    ds:0x8213af0,eax
--
 80542e1:	89 10                	mov    DWORD PTR [eax],edx
 80542e3:	b8 a4 87 04 88       	mov    eax,0x880487a4
 80542e8:	a3 f0 3a 21 08       	mov    ds:0x8213af0,eax
--
 80546b4:	89 10                	mov    DWORD PTR [eax],edx
 80546b6:	b8 a4 87 04 88       	mov    eax,0x880487a4
 80546bb:	a3 f0 3a 21 08       	mov    ds:0x8213af0,eax
--
 805ac53:	89 10                	mov    DWORD PTR [eax],edx
 805ac55:	b8 a4 87 04 88       	mov    eax,0x880487a4
 805ac5a:	a3 f0 3a 21 08       	mov    ds:0x8213af0,eax
--
 805b026:	89 10                	mov    DWORD PTR [eax],edx
 805b028:	b8 a4 87 04 88       	mov    eax,0x880487a4
 805b02d:	a3 f0 3a 21 08       	mov    ds:0x8213af0,eax
--
 805b3f9:	89 10                	mov    DWORD PTR [eax],edx
 805b3fb:	b8 a4 87 04 88       	mov    eax,0x880487a4
 805b400:	a3 f0 3a 21 08       	mov    ds:0x8213af0,eax
--
 806700d:	89 10                	mov    DWORD PTR [eax],edx
 806700f:	b8 a4 87 04 88       	mov    eax,0x880487a4
 8067014:	a3 f0 3a 21 08       	mov    ds:0x8213af0,eax
```

It seems that the call to `silent__cipher` is controlled by this instruction: `mov    eax,0x880487a4`. Can we patch it ? Let's patch it !

```Python
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
```

```Java
[pix:such_movs_amazing] % python pwn-such-movs-amazing.py
[*] Opening ./bin/such_movs_amazing
[*] Load addr: 0x8048000
[*] main: 0x804cdb5
[*] verbose_cipher: 0x80495bb
[*] silent__cipher: 0x80487a4
[*] search:         0x880487a4
[*] Found ref to silent__cipher in silent__cipher
[*] Found ref to silent__cipher in verbose_cipher
[*] Found ref to silent__cipher in verify_cipher_is_sane_before
[*] Found ref to silent__cipher in main
[+] Patching to use verbose_cipher
[*] Found ref to silent__cipher in main
[+] Patching to use verbose_cipher
[*] Found ref to silent__cipher in main
[+] Patching to use verbose_cipher
[*] Found ref to silent__cipher in main
[+] Patching to use verbose_cipher
[*] Found ref to silent__cipher in main
[+] Patching to use verbose_cipher
[*] Found ref to silent__cipher in main
[+] Patching to use verbose_cipher
[*] Found ref to silent__cipher in main
[+] Patching to use verbose_cipher
[+] Writing such_movs_amazing.new
```

Now it's time to run our patched binary !

```Bash
[pix:such_movs_amazing] % ./bin/such_movs_amazing.patched
CIPHER DBG: [20:13] -> 33 | [' ', ''] -> '3'
CIPHER DBG: [33:37] -> 04 | ['3', '7'] -> ''
CIPHER DBG: [04:42] -> 46 | ['', 'B'] -> 'F'
a[0] = '0x** | (Encrypted value hidden due to security reasons)'
CIPHER DBG: [22:13] -> 31 | ['"', ''] -> '1'
CIPHER DBG: [31:37] -> 06 | ['1', '7'] -> ''
CIPHER DBG: [06:42] -> 44 | ['', 'B'] -> 'D'
b[0] = '0x44'
CIPHER DBG: [22:13] -> 31 | ['"', ''] -> '1'
CIPHER DBG: [31:37] -> 06 | ['1', '7'] -> ''
CIPHER DBG: [06:42] -> 44 | ['', 'B'] -> 'D'
c[0] = '0x44'
CIPHER DBG: [20:13] -> 33 | [' ', ''] -> '3'
CIPHER DBG: [33:37] -> 04 | ['3', '7'] -> ''
CIPHER DBG: [04:42] -> 46 | ['', 'B'] -> 'F'
a[1] = '0x** | (Encrypted value hidden due to security reasons)'
CIPHER DBG: [15:13] -> 06 | ['', ''] -> ''
CIPHER DBG: [06:37] -> 31 | ['', '7'] -> '1'
CIPHER DBG: [31:42] -> 73 | ['1', 'B'] -> 's'
b[1] = '0x73'
CIPHER DBG: [aa:13] -> b9 | ['�', ''] -> '�'
CIPHER DBG: [b9:37] -> 8e | ['�', '7'] -> '�'
CIPHER DBG: [8e:42] -> cc | ['�', 'B'] -> '�'
c[1] = '0xcc'
CIPHER DBG: [20:13] -> 33 | [' ', ''] -> '3'
CIPHER DBG: [33:37] -> 04 | ['3', '7'] -> ''
CIPHER DBG: [04:42] -> 46 | ['', 'B'] -> 'F'
a[2] = '0x** | (Encrypted value hidden due to security reasons)'
```

Foccusing on a[x] calculations:

```Bash
[pix:such_movs_amazing] % ./bin/such_movs_amazing.patched |grep -a -B3 "0x\*\*"
CIPHER DBG: [42:13] -> 51 | ['B', ''] -> 'Q'
CIPHER DBG: [51:37] -> 66 | ['Q', '7'] -> 'f'
CIPHER DBG: [66:42] -> 24 | ['f', 'B'] -> '$'
a[3] = '0x** | (Encrypted value hidden due to security reasons)'
--
CIPHER DBG: [5a:13] -> 49 | ['Z', ''] -> 'I'
CIPHER DBG: [49:37] -> 7e | ['I', '7'] -> '~'
CIPHER DBG: [7e:42] -> 3c | ['~', 'B'] -> '<'
a[4] = '0x** | (Encrypted value hidden due to security reasons)'
--
CIPHER DBG: [48:13] -> 5b | ['H', ''] -> '['
CIPHER DBG: [5b:37] -> 6c | ['[', '7'] -> 'l'
CIPHER DBG: [6c:42] -> 2e | ['l', 'B'] -> '.'
a[5] = '0x** | (Encrypted value hidden due to security reasons)'
--
CIPHER DBG: [43:13] -> 50 | ['C', ''] -> 'P'
CIPHER DBG: [50:37] -> 67 | ['P', '7'] -> 'g'
CIPHER DBG: [67:42] -> 25 | ['g', 'B'] -> '%'
a[6] = '0x** | (Encrypted value hidden due to security reasons)'
--
CIPHER DBG: [54:13] -> 47 | ['T', ''] -> 'G'
CIPHER DBG: [47:37] -> 70 | ['G', '7'] -> 'p'
CIPHER DBG: [70:42] -> 32 | ['p', 'B'] -> '2'
a[7] = '0x** | (Encrypted value hidden due to security reasons)'
--
CIPHER DBG: [46:13] -> 55 | ['F', ''] -> 'U'
CIPHER DBG: [55:37] -> 62 | ['U', '7'] -> 'b'
CIPHER DBG: [62:42] -> 20 | ['b', 'B'] -> ' '
a[8] = '0x** | (Encrypted value hidden due to security reasons)'
```

This looks like advanced Triple-XOR (c)(r)(tm), and you can spot BZHCTF !

```Bash
[pix:such_movs_amazing] % ./bin/such_movs_amazing.patched |grep -a -B3 "0x\*\*" |grep ':13\]' |cut -d"'" -f2 |sed -e :a -e 'N;s/\n//;ba'
   BZHCTF{I_ike_t0_m0v_it_m0v_it_!}
 ```

Nice typo :-)

By [@PiX](https://twitter.com/pix)
