# Nuit du Hack Qualifications CTF 2016: Matriochka - Step 2

**Category:** Crack Me |
**Points:** 100 |
**Solves:** 238 |
**Description:**

> _Description is missing, feel free to send a pull request_

## Write-up

This task is a reverse-engineering task.
The binary is obtained from Matriochka - Step 1 once the user provides the good
flag.

This one expects user input in argv. First thing it does it a strlen() which
gives us details about the flag length :

```ASM
mov     rdi, rax        ; s
call    _strlen
lea     rdx, [rax+1]    ; (len + 1)
mov     rax, rdx        ; (len + 1)
shl     rax, 2          ; (len + 1) * 4
add     rax, rdx        ; (len + 1) * 5
shl     rax, 2          ; (len + 1) * 20
add     rax, rdx        ; (len + 1) * 21
add     rax, rax        ; (len + 1) * 42
cmp     rax, 1F8h       ; (len + 1) * 42 == 12 * 42
```

We find that strlen(flag) == 11

Then, there is a bunch of instruction blocks that read a char, makes some
arithmetical operations, and compares it to a value.

Example:
```ASM
mov     rax, [rbp+argv] ; 4 = d
add     rax, 8
mov     rax, [rax]
add     rax, 3
movzx   eax, byte ptr [rax]
movsx   eax, al
add     eax, eax
cmp     eax, 0C8h ; '+'
jz      short loc_4007B0 ; 7 = p
mov     [rbp+bool], 0
```
argv[2][3] * 2 == 0xC8 <=> argv[2][3] == 0x64 ('d')

Most of them are straight forward like the previous one, but some of them were
trickier since they checked against an other char.

No bruteforce was required, 2 passes were required.

Once most of the flag was recovered (Pandi_p??da), we guessed the 2 remaining
characters.

**Flag**: Pandi_panda