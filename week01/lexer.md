# A lexerekről és a Flexről pár szóban

**Ez a rész nem lesz számonkérve semmilyen formában, csupán "érdekességnek" szánom**

# Mik is lennének ezek?
A lexikális elemzők (későbbiekben elvétve: lexer(ek)) feladatát út tudnám összefoglalni, hogy a bemetükről beolvasott stringet feldolgozza valamilyen módon; elemeire bontja az eredeti szöveget általunk meghatározott szabályok szerint, és a kapott eredményt (ezeket tokeneknek vagy szimbólumoknak is hívjuk) rendelkezésünkre bocsájtja; hogy ezekkel mit teszünk, már rajtunk múlik. Ignorálhatjuk a tokent, kiírathatjuk fájlba/stdout-ra, továbbadhatjuk egy parsernek, eltárolhatjuk egy adatszerkezetben... Gyakorlatilag akármit amit egy programozási nyelvben adattak tehetünk.

A Flex (illetve szellemi elődje, a Lex) egy program, ami sajátos fordít C, illetve C++ kódra. Beállításokban és funkciókban gazdag programról van szó. Lehetőségünk van úgy is megírni a flex forrásunkat, hogy az egy onálló programra forduljon legvégül; de van lehetőségünk arra is, hogy a lexerünket például egy C++ osztállyá fordítsa, amit később ugyanúgy használhatunk más programokban, mint bármely osztályt.

A Lex/Flex (illetve a hozzájuk szorosan kötődő Yacc/GNU Bison parserek) több évtizedes múltra tekintenek vissza, nagyjából a C nyelvvel egy korban mozognak. Népszerűségét növelte, hogy egy akkoriban elég népszerűnek számító operációs rendszer (UNIX) alapértelmezetten tartalmazta. Népszerűsége révén számos más programozási nyelvben írt lexer átvette karakterét (pl.: PLY).

# Mire használjuk a lexikális elemzőket?
A teljesség igénye nélkül:
- Fordítóprogramokban (compilerek, interpreterek)
- Olyan formátumok feldolgozására, amiknek jól definiált a struktórája (XML, HTML, JSON, CSS)
- Saját magunk által definiált "nyelvek" feldolgozására; ezek lehetnek:
  - Config fájlok, amik valamilyen, általunk meghatározott szerkezetet/szabályrendszert követnek
  - Egy házi fejlesztési Discord botnak adott parancsok feldologzása

**Az előbbi példákra általában igaz, hogy lexikális elemzőket IS használunk hozzájuk; általában egy parserrel társítva**

# Mit nem tudnak a lexerek?
Az alábbiakbak a compilerekben használt lexereket taglaljuk, azonban minden lexer ezekkel a határokkal rendelkezik.

---

Nézzük az alábbi "C++" "kódot":

```c++
num int ; 3 =
```

A ***tetszőleges C++ compiler*** lexere számára ez egy hibátlan sor. Miden ami szerepel benne lexikálisan helyes:
  - a *num* egy azonosítónév
  - az int egy beépített típus
  - a *;* és az *=* szimbólumok is ismerősek számára
  - a *3* pedig egy literál

És mégis, ez az abomináció nem C++ vagy C, vagy akármilyen jelenléttel bíró nyelv *(talán Javascriptnek elmegy)*.

*Nem tud a nyelv nyelvtani szabályairól*

---

Nézzük az alábbi C++ "kódot"

```c++
  int num = "Orrával a sárba döf\n";
```

Itt már azért érezzük Bjarne Stroustrup auráját, de valami mégsem az igazi.

Ez a sor már nyelvtanilag helyes, hiszen ez egy deklaráció, inicializációval egybekötve. Hiszen nézzük meg:

*típus* *azonosító* = *érték*;

Ha ezt megkíséreljül lefordítani, a ***tetszőleges C++ compiler*** fordítási hibát fog dobni, mivel bár nyelvtanilag jó, a C++-ban ez a művelet nem értelmezett.

*Nem tud a nyelv szemantikai szabályairól*

# Flex

## Egy Flex forrás felépítése
Minden Flex forrás a következő mintát követi:
```
definíciók
%%
A lexer műkösését leíró szabályok
%%
A saját programkódunk
```

### Definíciók

A definíciók szekcióban elhelyezhetünk olyan kódot, amit szeretnénk, ha a flex a fordítás során érintetlenül hagyna, pl.:
```c
%{
#include <math.h>

double avg = 0;
%}
```

Továbbá definiálhatunk dolgokat, amire a Flexnek van szüksége, például ha egy karaktercsoportot gyakran használunk (vagy ha egyszerűen olvashatóbbá akarjuk tenni a programunkat), akkor azokat itt kell definiálni. Ilyen lehet például:

```c
%{
LOWER_CHAR [a-z]
UPPER_CHAR [A-Z]
DIGIT [0-9]
%}
```

Ha a szabályok részben használjuk a DIGIT aliast, az ugyanazt eredményezné, mintha a definícióját használtuk volna. Érdemes gyakran használt, nem túl bonyolult karaktercsoportokat definiálni.

### Szabályok

A szabályokat reguláris kifehezésekkel határozzuk meg (a szintaxisa nagyon hasonlít a **grep** reguláris kifejezéseire).

A szintaxis pár sorban:

Reg. kif | Jelentése
x | Maga az **x** karakter
. | Bármilyen karakter (kivéve ha az újsor karakter az inputról)
[xyz] | Egy karakter, ami vagy **x**, vagy **y**, vagy **z**
[a-z] | Egy karakter, ami **a** és **z** közé esik
[ab-lZ] | Egy karakter, ami lehet **a**, **b** és **l** között akármi, vagy **Z**
[^a] | Egy karakter, ami nem **a** (úgyis mondhatnánk, hogy **a** komplementerét képzi, ahol az ASCII karakterek az alaphalmaz)
[^bl-zZ] | Egy karakter, ami **nem** **b**, nem **Z** és nem **l** és **z** közé esik (tehát a ^ mindemre vonatkozik, ami utánna áll)

Ezekkel a kifejezésekkel tudunk karaktereket, illetve karakter csoportokat definiálni. Karaktereken, illetve karakter csoportokon tudunk használni műveleteket

Művelet | Példa | Jelentése 
* | r* | Akármennyi (tehát 0, 1, 2, ...) **r**
+ | r+ | Akármennyi, de legalább egy **r**
? | r? | Vagy 0 vagy 1 darab **r** (mondhatnánk hogy **r** opcionális)

Ezeket a műveleteket alkalmazhatjuk karakterekre, illetve karakter csoportokra.

[A szintaxisról minden kíváncsiságok kimerítő terjedelemben](http://web.mit.edu/gnu/doc/html/flex_2.html)

A szabályok meghatározásakor meg kell határoznunk, hogy abban az esetben, ha találtunk valamit ami illeszkedik a mintára, akkor milyen utasításokat hajtson végre a program. Az utasítások meghatározásakor a yytext változó (string típusú) tárolja azt, ami illeszkedett a szabályra.

Az utasításokban hivatkozhazunk már deklarált változókra:

```c
%{
    int number_of_a = 0;
%}

%%
a {
   ++number_of_a; 
}
//további szabályok / kód
//...
```

Felfoghatjuk  egy függvényként is az utasítások sorát, amiből visszatérítünk valamit:

```c
1 {
    //azaz ha a  karakter '1' (literálisan az egy ASCII karakter),
    //akkor a stringet ami ezt az egyes karaktert tartalmazza konvertáljuk int-té
    //és térjen vissza vele a függvény.
    return atoi(yytext);
}
```

De akár írhatunk a kimenetre. Igazából bármit tehetünk, amit a C/C++ nyelv, a deklarált változóink, illetve a programunk többi része megenged.

### Saját kód

Ebben a részben implementálhatjuk a programunk többi részét, ehhez sem fog nyúlni a flex fordítás közben, tehát itt valid C++ kódot igyekezzünk írni.

# Mindez a gyakorlatban
