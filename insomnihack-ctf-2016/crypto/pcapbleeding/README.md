#Insomni'hack CTF 2016 : PCAPBleeding

**Category:** Crypto |
**Points:** ?? |
**Solves:** ?? |
**Description:** 

> PCAPBleeding
> Author : julien - Difficulty : medium
> Finally, somebody has read our 2014 log files. We think an attack occurred on our HTTPS server but we don't know if they succeed in stealing valuable
> information. Some confidential data pass through this SSL channels so we hope it's not broken :~
> The flag format is INS={some random string here}.

---

##Write-up

This challenge was quite easy.
We were given 3 files :

```bash
>>> file *
attack_log.pcap:  tcpdump capture file (little-endian) - version 2.4 (Ethernet, capture length 262144)
hb_scrt_ch.crt:   PEM certificate
pcap_flag.pcapng: pcap-ng capture file - version 1.0
```

We can see that the frame number 18 is a `Heartbeat Request[Malformed Packet]` :

```bash
>>> tshark -r attack_log.pcap -Y frame.number==18 
 18   0.044628 192.168.105.1 -> 192.168.105.160 TLSv1 74 Heartbeat Request[Malformed Packet]
 [ notfound @ greenwich:~/CTF/2016/INSOM | 0 : !1232 - 1jobs, 0tmux, +3shlvl ]
 [ /dev/pts/3 - 21:16:50 ]

>>> tshark -r attack_log.pcap -Y frame.number==18 -x
 0000  00 0c 29 7c 33 4f 00 50 56 c0 00 08 08 00 45 00   ..)|3O.PV.....E.
 0010  00 3c 89 26 40 00 40 06 5d a3 c0 a8 69 01 c0 a8   .<.&@.@.]...i...
 0020  69 a0 9e 7c 01 bb 3c 41 a2 9a 45 f7 6e 2b 80 18   i..|..<A..E.n+..
 0030  00 fb 1b 11 00 00 01 01 08 0a 00 74 28 e2 00 06   ...........t(...
 0040  8e 18 18 03 01 00 03 01 ff ff                     ..........
```

Moreover, you can use the `Heartbleed BPF expression` ( source [http://www.riverbed.com/blogs/Retroactively-detecting-a-prior-Heartbleed-exploitation-from-stored-packets-using-a-BPF-expression.html] ) :

```bash
>>> tcpdump -r attack_log.pcap 'tcp src port 443  and (((ip and tcp[((tcp[12] & 0xF0) >> 4 ) * 4] = 0x18) and (tcp[((tcp[12] & 0xF0) >> 4 ) * 4 + 1] = 0x03) and (tcp[((tcp[12] & 0xF0) >> 4 ) * 4 + 2] < 0x04) and ((ip[2:2] - 4 * (ip[0] & 0x0F)  - 4 * ((tcp[12] & 0xF0) >> 4) > 69)) )  or ( (ip6 and ip6[6]=6 and (ip6[40 + ((ip6[40+12] & 0xF0) >> 4) * 4 + 0] = 0x18) and (ip6[40 + ((ip6[40+12] & 0xF0) >> 4) * 4 + 1] = 0x03) and (ip6[40 + ((ip6[40+12] & 0xF0) >> 4) * 4 + 2] > 0x04) and ((ip6[4:2] - 4*( (ip6[40+12] & 0xF0) >> 4) ) > 69))))'
reading from file attack_log.pcap, link-type EN10MB (Ethernet)
16:38:13.785570 IP 192.168.105.160.https > 192.168.105.1.40572: Flags [.], seq 1173843499:1173844947, ack 1010934434, win 235, options [nop,nop,TS val 429596 ecr 7612642], length 1448
16:38:13.785821 IP 192.168.105.160.https > 192.168.105.1.40572: Flags [.], seq 16389:17837, ack 1, win 235, options [nop,nop,TS val 429596 ecr 7612642], length 1448
16:38:13.786029 IP 192.168.105.160.https > 192.168.105.1.40572: Flags [.], seq 32778:34226, ack 1, win 235, options [nop,nop,TS val 429596 ecr 7612642], length 1448
16:38:13.786210 IP 192.168.105.160.https > 192.168.105.1.40572: Flags [.], seq 49167:50615, ack 1, win 235, options [nop,nop,TS val 429596 ecr 7612642], length 1448
```

Well, the attack as been performed.
What to do now ? My first thought was to pick up the public exponent and the modulus, by doing this :

```bash
>>> openssl x509 -in hb_scrt_ch.crt -noout -text -modulus
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number:
            e2:07:be:1e:41:2d:e5:b9
    Signature Algorithm: sha256WithRSAEncryption
        Issuer: CN=hb.scrt.ch
        Validity
            Not Before: Feb 18 15:23:21 2016 GMT
            Not After : Feb 15 15:23:21 2026 GMT
        Subject: CN=hb.scrt.ch
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                Public-Key: (2048 bit)
                Modulus:
                    00:bf:68:3e:d2:cc:8c:1f:25:9b:bf:64:28:90:4c:
                    d5:c3:2a:e9:74:e9:bb:1b:52:e3:9a:31:45:19:23:
                    b4:db:bd:95:02:e3:4f:ec:0a:04:41:da:9c:c8:eb:
                    db:29:70:ee:9f:94:5d:4b:01:f1:97:1e:a3:51:2f:
                    63:c3:7f:69:6c:68:bd:34:f5:52:d9:f2:a9:2f:a0:
                    3a:7e:59:65:f2:f9:d8:01:34:c9:de:a1:aa:94:73:
                    2a:50:6a:11:4b:c6:16:0a:b8:d9:9c:87:59:22:03:
                    7a:0b:f3:93:45:d4:ed:05:02:27:0d:37:74:f1:60:
                    16:37:16:db:5c:3f:39:3c:1d:68:f4:a0:0c:8c:30:
                    1b:38:ff:9b:a0:53:eb:9b:7e:1d:60:96:87:61:e6:
                    d3:13:4a:ba:e2:e0:4a:fc:94:06:9f:ac:d0:56:6d:
                    93:f1:19:98:be:89:ea:4a:c0:3f:16:9c:2f:1d:a8:
                    7c:a1:5e:99:7a:4a:cf:67:c6:40:52:76:8b:a0:08:
                    81:f6:28:8d:db:b2:bb:68:15:93:53:15:ef:52:db:
                    93:9d:d3:e1:cb:39:ea:fb:18:89:85:ca:44:5c:ee:
                    8b:7f:26:a1:79:77:0d:ca:2f:36:d3:d3:5b:b7:fa:
                    ef:62:9d:39:2a:94:3a:5c:ba:d4:1d:2f:fa:bf:4e:
                    1d:61
                Exponent: 65537 (0x10001)

```

Well, a 2048 bit modulus. Not the time to try a factorisation, and it will be stupid (crypto challenge, not "bruteforce" challenge).
I knew the amazing `heartleech` by Robert David Graham [https://github.com/robertdavidgraham/heartleech]; so let's use it.

```bash
>>> /home/notfound/TOOLS/heartleech/heartleech --read attack_log.pcap --cert hb_scrt_ch.crt > privkey.pem && tshark -o "ssl.keys_list:192.168.105.160,443,http,privkey.pem" -r pcap_flag.pcapng -Y frame.number==15 -x

Reassembled SSL (312 bytes):
0000  48 54 54 50 2f 31 2e 31 20 32 30 30 20 4f 4b 0d   HTTP/1.1 200 OK.
0010  0a 44 61 74 65 3a 20 54 68 75 2c 20 31 38 20 46   .Date: Thu, 18 F
0020  65 62 20 32 30 31 36 20 31 35 3a 35 38 3a 31 30   eb 2016 15:58:10
0030  20 47 4d 54 0d 0a 53 65 72 76 65 72 3a 20 41 70    GMT..Server: Ap
0040  61 63 68 65 2f 32 2e 34 2e 37 20 28 55 62 75 6e   ache/2.4.7 (Ubun
0050  74 75 29 0d 0a 4c 61 73 74 2d 4d 6f 64 69 66 69   tu)..Last-Modifi
0060  65 64 3a 20 54 68 75 2c 20 31 38 20 46 65 62 20   ed: Thu, 18 Feb 
0070  32 30 31 36 20 31 35 3a 34 32 3a 31 38 20 47 4d   2016 15:42:18 GM
0080  54 0d 0a 45 54 61 67 3a 20 22 34 33 2d 35 32 63   T..ETag: "43-52c
0090  30 64 33 36 32 66 34 32 35 30 22 0d 0a 41 63 63   0d362f4250"..Acc
00a0  65 70 74 2d 52 61 6e 67 65 73 3a 20 62 79 74 65   ept-Ranges: byte
00b0  73 0d 0a 43 6f 6e 74 65 6e 74 2d 4c 65 6e 67 74   s..Content-Lengt
00c0  68 3a 20 36 37 0d 0a 43 6f 6e 6e 65 63 74 69 6f   h: 67..Connectio
00d0  6e 3a 20 63 6c 6f 73 65 0d 0a 43 6f 6e 74 65 6e   n: close..Conten
00e0  74 2d 54 79 70 65 3a 20 74 65 78 74 2f 68 74 6d   t-Type: text/htm
00f0  6c 0d 0a 0d 0a 3c 68 74 6d 6c 3e 0a 3c 62 6f 64   l....<html>.<bod
0100  79 3e 54 68 65 20 66 6c 61 67 20 69 73 20 49 4e   y>The flag is IN
0110  53 3d 7b 48 42 5f 70 72 31 76 34 74 65 5f 6b 65   S={HB_pr1v4te_ke
0120  79 35 5f 6c 65 33 6b 7d 3c 2f 62 6f 64 79 3e 0a   y5_le3k}</body>.
0130  3c 2f 68 74 6d 6c 3e 0a                           </html>.

```
<br>
Pwned.<br>
__Flag__ : "The flag is INS={HB_pr1v4te_key5_le3k}."

Enjoy,
\- [Notfound](http://www.notfound.ovh)
