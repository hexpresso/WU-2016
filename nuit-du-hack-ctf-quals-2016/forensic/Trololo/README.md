#NDH Quals CTF 2016 : Trololo

**Category:** Forensic **Points:** 100 **Solves:** 83

> Description : A computer belonging to a new company has been infected by a malware. This is a known version of a cryptolocker software, that uses a irc server to received commands. Let's try to grab its password...

##Write-up

We begin this challenge with a [pcap](https://github.com/hexpresso/WU-2016/blob/master/nuit-du-hack-ctf-quals-2016/forensic/Trololo/trololo.pcap). First thing done, we opened this pcap with wireshark and then checked the protocol hierarchy `Wireshark > Statistics > Protocol Hierarchy`. We saw that 99% of the traffic where RTP, there are also some ICMP and HTTP too. So, let's see if we have any HTTP Object inside this pcap `Wireshark > File > Export Objects > HTTP`. Nice, we have a [config.enc](https://github.com/hexpresso/WU-2016/blob/master/nuit-du-hack-ctf-quals-2016/forensic/Trololo/config.enc) file.

```
$ file config.enc 
config.enc: Non-ISO extended-ASCII text, with very long lines, with NEL line terminators
```

With the description, we know the company have been infected with a malware, the config.enc is probably encrypted. We started to analyse it and to look for a key or any other element in some packet. One of our test on the analyse with [xortool](https://github.com/hellman/xortool) where nice.

```
$ xortool config.enc 
The most probable key lengths:
   1:   82.9%
  58:   9.2%
  64:   7.9%
```
Let's try with the most frequent char to help xortool:
```
$ xortool -l 1 -c 20 config.enc 
1 possible key(s) of length 1:
\xff
Found 1 plaintexts with 95.0%+ printable characters
```

Bingo, we got an xml file [O.out](https://github.com/hexpresso/WU-2016/blob/master/nuit-du-hack-ctf-quals-2016/forensic/Trololo/O.out) generated in xortool_out:

**Flag:** orudeujieh6oonge4She

[NotFound](https://twitter.com/Notfound404__) [zbetcheckin](https://twitter.com/zbetcheckin)
