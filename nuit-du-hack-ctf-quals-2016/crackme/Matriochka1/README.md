# Nuit du Hack Qualifications CTF 2016: Matriochka - Step 1

**Category:** Crack Me |
**Points:** 50 |
**Solves:** 432 |
**Description:**

> _Description is missing, feel free to send a pull request_

## Write-up

This task is a reverse-engineering task.
The matriochka series is interesting because just like real-life matryoshka
dolls, you start with a huge thing that contains smaller things.

First thing to do with reverse engineering tasks is downloading radare2, trying
to figure out how to disassemble stuff, read the radare2 book, print it, give
up, burn it, uninstall radare2, and start gdb or IDA.

```ASM
mov     esi, offset s2  ; "Much_secure__So_safe__Wow"
mov     rdi, rax        ; s1
call    _strcmp
```

We instantly see that it calls `strcmp(s1, "Much_secure__So_safe__Wow")`

**Flag**: Much_secure__So_safe__Wow