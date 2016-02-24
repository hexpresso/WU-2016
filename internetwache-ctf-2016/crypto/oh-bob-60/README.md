# Internetwache CTF 2016 : Oh Bob!

**Category:** Crypto
**Points:** 60
**Solves:** 142
**Description:**

> Description: Alice wants to send Bob a confidential message. They both remember the crypto lecture about RSA. So Bob uses openssl to create key pairs. Finally, Alice encrypts the message with Bob's public keys and sends it to Bob. Clever Eve was able to intercept it. Can you help Eve to decrypt the message?
> 
> 
> Attachment: [crypto60.zip](./crypto60.zip)


## Write-up
```python
#!/usr/bin/env python

from multiprocessing import Pool
from Crypto.PublicKey import RSA
from Crypto.Util.number import long_to_bytes, bytes_to_long
from sage.all import factor
from string import printable as fuck


def _egcd(a, b):
    u, u1 = 1, 0
    v, v1 = 0, 1
    while b:
        q = a // b
        u, u1 = u1, u - q * u1
        v, v1 = v1, v - q * v1
        a, b = b, a - q * b
    return a, u, v


def _decrypt(p, q, e, n, ctxt):
    phi = (p - 1) * (q - 1)
    gcd, a, b = _egcd(e, phi)
    d = a
    if d < 0:
        d += phi
    pt = pow(ctxt, d, n)
    return long_to_bytes(pt)


def _factor_me(pubkeyyyyyy):
    return factor(pubkeyyyyyy)

f = open('secret.enc', 'r').readlines()
c1 = f[0].decode('base64')
c2 = f[4].decode('base64')
c3 = f[2].decode('base64')

keypub1 = RSA.importKey(open('bob.pub'))
keypub2 = RSA.importKey(open('bob2.pub'))
keypub3 = RSA.importKey(open('bob3.pub'))
print('[+] All keypub has been imported')

e1 = keypub1.e
e2 = keypub2.e
e3 = keypub3.e
print('[+] All public exponent has been imported')

n1 = keypub1.n
n2 = keypub2.n
n3 = keypub3.n
print('[+] All modulus has been imported')

core = Pool(3)  # MULTIPROCESS FOR MULTIGAME \o/
# p1, q1, p2, q2, p3, q3 = core.map(_factor_me, [n1, n2, n3])
result = core.map(_factor_me, [n1, n2, n3])
p1, q1 = str(result[0]).split(' * ')
p2, q2 = str(result[1]).split(' * ')
p3, q3 = str(result[2]).split(' * ')


flag = _decrypt(int(p1), int(q1), e1, n1, bytes_to_long(c1))
flag += _decrypt(int(p2), int(q2), e2, n2, bytes_to_long(c2))
flag += _decrypt(int(p3), int(q3), e3, n3, bytes_to_long(c3))
print(filter(lambda x: x in fuck, flag))
```

Output:

```bash
notfound@OTP:/home/notfound/CTF/2016/WACHE/CRYTO/CRY60
>>> sage cry60.py
[+] All keypub has been imported
[+] All public exponent has been imported
[+] All modulus has been imported
I~z8IW{WEAK_R
aNyjVaSA_K3YS_4R
oba53_SO_BAD!}
```

There are some garbages but the flag is guessable,

__Flag__ : IW{WEAK_RSA_K3YS_4R3_SO_BAD!}

Enjoy,<br>
\- [Notfound](http://www.notfound.ovh)


## Other write-ups and resources

* <https://www.xil.se/post/internetwache-2016-crypto60-kbeckmann/>
* <https://github.com/ernacktob/CTF-Writeups/tree/master/Internetwache-CTF-2016/crypto60>
* <https://eugenekolo.com/blog/internetwache-2016-ctf-writeups/#ohbob>
* <https://github.com/p4-team/ctf/tree/master/2016-02-20-internetwache/crypto_60>
* <http://err0r-451.ru/internetwache-crypto-oh-bob-60-pts/>

