# Nuit du Hack Qualifications CTF 2016: Matriochka - Step 3

**Category:** Crack Me |
**Points:** 300 |
**Solves:** 183 |
**Description:**

> _Description is missing, feel free to send a pull request_

## Write-up

This task is a reverse-engineering task.
The binary is obtained from Matriochka - Step 2 once the user provides the good
flag.

This crackme uses signals to give you a hard time debugging it: it sets up an
handler for SIGFPE and SIGSEGV.

SIGFPE is the good boy, SIGSEGV is the next step. Each time you have a valid
character, it replaces the previous SIGSEGV handler, which is called after
returning.

Every handlers have the same structure:
* Get the ith char
* Multiply it with a number
* Shift it right
* Check if the result is 0x3E8 (1000)

```ASM
movzx   eax, byte ptr cs:serial
movsx   eax, al
mov     [rbp+char], eax
mov     eax, [rbp+char]
imul    ecx, eax, 3E8h
mov     edx, 78787879h
mov     eax, ecx
imul    edx
sar     edx, 5
mov     eax, ecx
sar     eax, 1Fh
sub     edx, eax
mov     eax, edx
mov     [rbp+char], eax
cmp     [rbp+char], 3E7h
jle     short loc_400848
cmp     [rbp+char], 3E8h
jle     short loc_40084A
```

At first, we thought it was a software modulo, but it isn't. We didn't want to
"waste" time understanding what's going on, instead we wrote a [C code](bf.c)
that bruteforces a character given the shift operand, and the factor.

**Flag**: Did_you_like_signals?