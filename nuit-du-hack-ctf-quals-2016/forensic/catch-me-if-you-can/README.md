#NDH Quals CTF 2016 : Catch me if you can

**Category:** Forensic **Points:** 100

> Description : We managed to infect the computer of a target. We recorded all packets transferred over the USB port, but there is something unusual. We need them to be sorted to get the juicy secret.

Attachments
<table>
<tr><td>Filename</td><td>Size</td><td>Hash (SHA-256)</td></tr>
<tr>
<td>[usb.pcap](usb.pcap)</td>
<td>93.99 kB</td><td>107af2dda6097d0f1e446ea375051899c900236279a4ff7fb5aca04708998c71</td>
</tr>
</table>
 
##Write-up

**TL,DR:** 2 intertwined .ods downloads, with a riddle in the first one

I start by looking at the pcap file with [Wireshark](https://www.wireshark.org/).
Since I'm not familiar with USB dumps, I don't really know what's
"unusual", but some bigger packets catch my eye, so I decide to
follow what appears to be a hint ("sorted") and sort by size.
I get 22 packets way larger than the others.

Since Wireshark doesn't know how to deal with the payload, I switch
to scripting with [scapy](http://www.secdev.org/projects/scapy/):

```
$ scapy
> f = rdpcap('usb.pcap')
```

I see the `PK` string hinting for a zip file, plus
`mimetypeapplication/vnd.oasis.opendocument.spreadsheet` hinting for
an Open/LibreOffice Calc spreadsheet document. It seems the payload
starts at offset 27 in each packet, let's assemble all the packets.
```
open('test.zip','w').write(''.join([p.load[27:] for p in f if len(p.load)>58]))
```

`unzip` says it's corrupted, must be some ordering to do, let's try by time:

```
ps=[p for p in f if len(p.load)>58]
ps.sort(lambda a,b: a.time < b.time)
open('test.zip','w').write(''.join([p.load[27:] for p in ps]))
```

Still corrupted, let's attempt to write all chunks for inspection:
```
for p in f: open('test'+str(p.time), 'w').write(p.load)
```

Lots of `.time` are identical, files are overwritten, this rules out
sorting by date. Also there are 248 trailing null bytes in the payload
that need to be removed. Let's extract all packets for manual
inspection, and put them in a single file for manual ordering
within a good, binary-safe editor such as &lt;troll&gt;Emacs&lt;/troll&gt;.
```
for i in range(len(f)):
    if len(f[i].load) > 58:
        open('test'+("%03d"%i), 'w').write(f[i].load[27:-248])
```
```
$ for i in test???; do (cat $i; echo; echo) >> test.all; done
```

OK, there are 2 chunks starting with `PK^C^D`, so let's assume there
are actually TWO zip files in here.  The goal must be to reassemble
the file based on knowledge of the format:

https://en.wikipedia.org/wiki/Zip_%28file_format%29#Structure

I spot two sets of file offsets, allowing a first grouping of the
chunks.  I then spend (too) much time trying to manually reorder the chunks,
since I didn't find the ordering key.  Eventually I figure out that
chunks are actually in order... just one of each file alternatively:
```
for i in range(len(f)):
    if len(f[i].load) > 58:
        open("test-%d-%03d"%((i%2)+1,i), 'w').write(f[i].load[27:-248])
# + manually add the end of file #2, not the same number of trailing null bytes (?)
open('test-2-803', 'w').write(f[803].load[27:])
```
```
$ cat test-1-* > test1.ods
$ cat test-2-* > test2.ods
```

The second file looks like a prank and doesn't seem interesting.

The first one looks like a riddle.  I am perplex for a while but
then the `<->` sign in the picture eventually gets me to look at the side of
the speadsheet (Ctrl + arrow keys).  At the bottom right:
```
g6d5g5f2b6g5d3e4d4b3c5b6k2j5j5g4l2
```

This looks like coordinates on the French keyboard from the spreadsheet:
```
g6 d5 g5 f2 b6 g5 d3 e4 d4 b3 c5 b6 k2 j5 j5 g4 l2
n  d  h  [  w  h  3  r  e  1  s  w  @  l  l  y  ]
ndh[wh3re1sw@lly]
```

And to finish, some flag format guessing to understand that one needs to drop `ndh[]` in the submission form, and we flag!

```
wh3re1sw@lly
```

-- Beuc
