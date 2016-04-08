# 250_Morser
Le challenge consiste à analyser une image.
Il est indiqué que le flag serait une suite de caractères qui ne commencerait pas par la chaine FLAG{ etc...

## Découverte:
```
mitsurugi@mitsu:~/chall/inshack$ file morser_files 
morser_files: PC bitmap, Windows 98/2000 and newer format, 640 x 480 x 16
mitsurugi@mitsu:~/chall/inshack$ mv morser_files morser.bmp 
mitsurugi@mitsu:~/chall/inshack$ identify morser.bmp 
morser.bmp BMP 640x480 640x480+0+0 8-bit sRGB 615KB 0.000u 0:00.000
mitsurugi@mitsu:~/chall/inshack$ ls -l morser.bmp 
-rw-r--r-- 1 mitsurugi mitsurugi 614538 avril  7 21:41 morser.bmp
mitsurugi@mitsu:~/chall/inshack$ 
```

Une image BMP, 8 Bpp. 

L'ouverture de l'image nous montre que ça parle de morse, et un artefact en bas à droite nous amène immédiatement à tester le LSB.

## analyse
De manière étrange, PIL sous python refuse de m'ouvrir l'image. Un tour sur wikipedia (https://fr.wikipedia.org/wiki/Windows_bitmap) nous décrit l'entête BMP, et nous dit que l'adresse de départ du contenu du BMP est donnée à l'offset 0x000A.
Dans notre cas, les données utiles démarrent à 138.

Nous écrivons un premier script pour voir à quoi ressemblent les premiers octets: le fichier BMP se remplit par le bas, l'artefact est donc sur la première ligne:

```python
#! /usr/bin/python

i=open("morser.bmp","ro")
image=i.read()
i.close()

juicy=image[138:138+480]

out=[]
for i in range(len(juicy)):
    out.append(str(ord(juicy[i])))
    if (i+1)%16==0:
        out.append('\n')

print ''.join(out)
```

Output :

```
mitsurugi@mitsu:~/chall/inshack$ ./img.py 
3310003330003330
0031100000000013
3300333000311100
3311330000003133
0033300011300013
3331111300100000
0000001131003330
0011300031000031
1000000000300000
1111001000000000
0011310013110013
0000331000000000
3331110000001311
0011111031110011
3313111130310000
3110001133133300
0033333013100011
1110111330113313
1111301310001113
3011331311310011
3000310000310000
3133000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000
0000000000000000

mitsurugi@mitsu:~/chall/inshack$
```

Uniquement 3 types de caractères.
Et écrire du morse demande 3 types de caractères: le trait, le point et la séparation.

Le 0 semble bien pour correspondre à la séparation, on teste le 1 pour point et 3 pour tiret:
```
for i in range(len(juicy)):
    out.append(" .Y-"[ord(juicy[i])])
```

Et on relance:

```
mitsurugi@mitsu:~/chall/inshack$ ./img.py 
--.   ---   --- 
  -..         .-
--  ---   -...  
--..--      -.--
  ---   ..-   .-
---....-  .     
      ..-.  --- 
  ..-   -.    -.
.         -     
....  .         
  ..-.  .-..  .-
    --.         
---...      .-..
  ..... -...  ..
--.-....- -.    
-..   ..--.---  
  ----- .-.   ..
... ...-- ..--.-
....- .-.   ...-
- ..--.-..-.  ..
-   -.    -.    
-.--            
```
c'est beaucoup mieux, mais certaines séries de traits et points sont beaucoup trop longues:
`--.-....-`

## Finalisation
Après quelques réflexions, nous nous rendons compte que les lettres en morse ne font pas plus de 6 symboles, et que les lettres à moins de symboles sont paddées par des 0. 
Il suffit d'ajouter des espaces tous les 6 caractères (on en profite pour tout mettre sur une ligne):

```python
for i in range(len(juicy)):
    out.append(" .Y-"[ord(juicy[i])])
    if (i+1)%6==0:
        out.append(' ')
```

```
mitsurugi@mitsu:~/chall/inshack$ ./img.py 
--.    ---    ---    -..           .---   ---    -...   --..--        -.--   ---    ..-    .----. ...-   .             ..-.   ---    ..-    -.     -..           -      ....   .             ..-.   .-..   .-     --.           ---...        .-..   .....  -...   ..--.- ....-  -.     -..    ..--.- --     -----  .-.    .....  ...--  ..--.- ....-  .-.    ...--  ..--.- ..-.   ..-    -.     -.     -.--    
```

Résolution:
```
mitsurugi@mitsu:~/chall/inshack$ ./img.py | morse -d
G O O D     J O B ,   Y O U 'V E      F O U N  D     T  H E      F L A  G     :   L 5B x4N  D xM  0R 53x4R 3xF U N  N  Y
```
Il y a des espaces en trop, mais nous comprenons le message :)
Le 'x' minuscule nous indique qu'une lettre n'est pas décodée, avec l'image sous les yeux et le morse, nous voyons qu'il s'agit 
du ..--.- qui correspond à underscore _

Le flag pour valider est donc:
```
L5B_4ND_M0R53_4R3_FUNNY
```
