#NDH Quals CTF 2016 : Who am I?

**Category:** Forensic **Points:** 150 **Solves:** 16

> Description : Our forensics team has analyzed the cell phone of a bank fraud victim. A hacker managed to steal her money with fraudulent bank transfers. Then bank representatives think that the hacker hijacked SMS containing payment tokens, used to verify customer identify prior to sensitive operations. Apparently, the victim bought a second-hand and already rooted (Cyanongen OS) phone on eBay. During the acquisition stage, our team mad a bit-wise copy of the internal storage, but something went wrong during the process. After taking the victim cell phone, we made a backup of the internal storage but it hasn't been We need your help to recover the data and find how the intrusion occured.

##Write-up

We begin this challenge with a whoami.zip (474,4 Mo). We first needed to fix it to be able to unzip it :
```bash
$ zip -FF whoami.zip --out fixed.zip
$ unzip fixed.zip
```

We checked the md5 sum with the 'nandroid.md5' list given and then listed the content:

```bash
$ cd fixed
$ du -sh * | sort -rh
604M   	system.ext4.tar.a
173M    data.ext4.tar.a
12M     recovery.img
8,0M    boot.img
12K     cache.ext4.tar.a
4,0K    nandroid.md5
0       system.ext4.tar
0       data.ext4.tar
0       cache.ext4.tar
```

Then, the forensic part started and took a while before to found something interesting. We see that some of the folder was empty.

We first analyse all folders and files with a recursive grep and some keywords but nothing interesting, we tried to get data from some DB like in 'data.ext4.a' : `data/com.android.providers.telephony/databases/mmssms.db` but it was empty...

We analyse also the 'boot.img' and the 'recovery.img' with [bootimg-tools](https://github.com/pbatard/bootimg-tools) and [abootimg](https://github.com/codeworkx/abootimg) but again, nothing interesting.

It was not easy to know 'what to search', but after some time focusing on the 'system.ext4.a' and all APK `find . -name "*.apk"`, [XeR](https://twitter.com/xer_0x2a) got the great idea to analyse all APK cert to figure out if some of them have a strange one:

```bash
$ cd system/app/
$ for i in *; do unzip -d ${i%.apk} $i; done
```

Then

```bash
for file in $(find . -type d -mindepth 1 -maxdepth 1); do
        cert="${file}/META-INF/CERT.RSA";
        
        [ -f $cert ] &&
        fgrep -q '@android.com' $cert ||
        fgrep -q 'Google, Inc'  $cert ||
        fgrep -q 'Google Inc'   $cert ||
        echo $file;
done
```

Well done:

```bash
$ bash cert_check.sh
grep: ./Term/META-INF/CERT.RSA: Aucun fichier ou dossier de ce type
grep: ./Term/META-INF/CERT.RSA: Aucun fichier ou dossier de ce type
./Term
./Alarm
```

We then decided to reverse the [Alarm.apk](https://github.com/hexpresso/WU-2016/blob/master/nuit-du-hack-ctf-quals-2016/forensic/WhoAmI/Alarm.apk), which had permission to read SMS.
Using a decompiler, we retrieved its [source code](https://github.com/hexpresso/WU-2016/blob/master/nuit-du-hack-ctf-quals-2016/forensic/WhoAmI/Alarm_apk_source). One of the function caught our intention :

```java
static String b(String paramString)
{
  String str1 = "";
  String str2 = d();
  String str3 = paramString.toUpperCase();
  int j = 0;
  int i = 0;
  paramString = str1;
  if (i < str3.length())
  {
    int m = str3.charAt(i);
    int k = j;
    str1 = paramString;
    if (m >= 65)
    {
      if (m <= 90) {
        break label75;
      }
      str1 = paramString;
    }
    for (k = j;; k = (j + 1) % str2.length())
    {
      i += 1;
      j = k;
      paramString = str1;
      break;
      label75:
      str1 = paramString + (char)((m - str2.charAt(j) + 26) % 26 + 65);
    }
  }
  paramString = e() + paramString;
  c();
  return Base64.encodeToString(paramString.getBytes(), 0);
}
```

We noticed that it applies a Vigenere cipher to the first string from the source code, using the second string as the key.

```
Vigenere("TIFFEHNBMHLGSPTDAUGXWRUBSEUYHVEYBENZTDHYDERERPJDWPVKXHIENZTDHYDXLFNSJGAAZJOUZDAZPSZUU", "ABBATHHTUUIZZIPQPQBEEB")
```

Which gives us

```
THEFLAGISNDHTHENLEFTSQUAREBRACKETFORENSICANDROIDDIORDNAFORENSICTHENRIGHTSQUAREBRACKET
```

**Flag:** FORENSICANDROIDDIORDNAFORENSIC

A great team work challenge [Hexpresso](https://twitter.com/hexpressoctf)
