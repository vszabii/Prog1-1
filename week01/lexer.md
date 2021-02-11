# A lexerekről és a Flexről pár szóban

**Ez a rész nem lesz számonkérve semmilyen formában, csupán "érdekességnek" szánom**

# Mik is lennének ezek?
A lexikális elemzők (későbbiekben elvétve: lexer(ek)) feladatát úgy tudnám összefoglalni, hogy a bemetükről beolvasott stringet feldolgozza valamilyen módon; elemeire bontja az eredeti szöveget általunk meghatározott szabályok szerint, és a kapott eredményt (ezeket tokeneknek vagy szimbólumoknak is hívjuk) rendelkezésünkre bocsájtja; hogy ezekkel mit teszünk, már rajtunk múlik. Ignorálhatjuk a tokent, kiírathatjuk fájlba/stdout-ra, továbbadhatjuk egy parsernek, eltárolhatjuk egy adatszerkezetben... Gyakorlatilag akármit amit egy programozási nyelvben adattal tehetünk.

A Flex (illetve szellemi elődje, a Lex) egy program amivel lexikális elemzőket tudunk **generálni** úgy, hogy helyenként sajátos szintaxisra és C/C++ szintaxisra épülő forrást fordít C, illetve C++ kódra. Beállításokban és funkciókban gazdag programról van szó. Lehetőségünk van úgy is megírni a flex forrásunkat, hogy az egy önálló programra forduljon legvégül; de van lehetőségünk arra is, hogy a lexerünket például egy C++ osztállyá fordítsa, amit később ugyanúgy használhatunk más programokban, mint bármely osztályt.

A Lex/Flex (illetve a hozzájuk szorosan kötődő Yacc/GNU Bison parserek) több évtizedes múltra tekintenek vissza, nagyjából a C nyelvvel egy korban mozognak. Népszerűségét növelte, hogy egy akkoriban elég népszerűnek számító operációs rendszer (UNIX) alapértelmezetten tartalmazta. Népszerűsége révén számos más programozási nyelvben írt lexer átvette "megjelenését" (pl.: PLY).

# Mire használjuk a lexikális elemzőket?
A teljesség igénye nélkül:
- Fordítóprogramokban (compilerek, interpreterek)
- Olyan (szöveg alapú) formátumok feldolgozására, amiknek jól definiált a struktórája (XML, HTML, JSON, CSS)
- Saját magunk által definiált "nyelvek" feldolgozására; ezek lehetnek:
  - Config fájlok, amik valamilyen, általunk meghatározott szerkezetet/szabályrendszert követnek
  - Egy házi fejlesztési Discord botnak adott parancsok feldologzása

**Az előbbi példákra általában igaz, hogy lexikális elemzőket IS használunk hozzájuk, de sokszor szükségünk van még egy parserre legalább**

# Mit nem tudnak a lexerek?
Az alábbiakbak a compilerekben használt lexereket taglaljuk, azonban minden lexer ezekkel a határokkal rendelkezik.

---

Nézzük az alábbi "C++" "kódot":

```c++
num int ; 3 =
```

A ***tetszőleges C++ compiler*** lexere számára ez egy hibátlan sor. Miden ami szerepel benne lexikálisan helyes:
  - a *num* egy azonosítónév
  - az *int* egy beépített típus neve
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

Ez a sor már nyelvtanilag helyes, hiszen ez egy deklaráció, inicializációval egybekötve. Nézzük meg:

*típus* *azonosító* = *érték*;

Ha ezt megkíséreljük lefordítani, a ***tetszőleges C++ compiler*** fordítási hibát fog dobni, mivel bár nyelvtanilag jó, a C++-ban ez a művelet nem értelmezett.

*Nem tud a nyelv szemantikai szabályairól*

# Flex

### Egy Flex forrás felépítése
Minden Flex forrás a következő mintát követi:
```
Definíciók
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
--- | ---
x | Maga az **x** karakter
. | Bármilyen karakter (kivéve ha az újsor karakter az inputról)
[xyz] | Egy karakter, ami vagy **x**, vagy **y**, vagy **z**
[a-z] | Egy karakter, ami **a** és **z** közé esik
[ab-lZ] | Egy karakter, ami lehet **a**, **b** és **l** között akármi, vagy **Z**
[^a] | Egy karakter, ami nem **a** (úgyis mondhatnánk, hogy **a** komplementerét képzi, ahol az ASCII karakterek az alaphalmaz)
[^bl-zZ] | Egy karakter, ami **nem** **b**, nem **Z** és nem **l** és **z** közé esik (tehát a ^ mindemre vonatkozik, ami utánna áll)

Ezekkel a kifejezésekkel tudunk karaktereket, illetve karakter csoportokat definiálni. Karaktereken, illetve karakter csoportokon tudjuk az alábbi műveleteket alkalmazni:

| Művelet | Példa | Jelentése |
| --- | --- | --- |
| * | r* | Akármennyi (tehát 0, 1, 2, ...) **r** |
| + | r+ | Akármennyi, de legalább egy **r** |
| ? | r? | Vagy 0 vagy 1 darab **r** (mondhatnánk hogy **r** opcionális) |

[A szintaxisról minden kíváncsiságot kimerítő terjedelemben](http://web.mit.edu/gnu/doc/html/flex_2.html)

A szabályok definiálásánál  meg kell határoznunk, hogy abban az esetben, ha találtunk valamit ami illeszkedik a mintára, akkor milyen utasításokat hajtson végre a program. Az utasítások meghatározásakor a yytext változó (string típusú) tárolja azt, ami illeszkedett a szabályra.

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

## real.l

```c
%{
#include <stdio.h>	
int realnum = 0;
%}
digit [0-9]
```

Az include utasítással "importáljuk" a C standard I/O köynvtárát, hogy tudjunk a kimenetre írni.

Továbbá deklarálunk egy int típusú **realnum** változót és nulla értékkel inicializáljuk. Ebben a változóban fogjuk eltárolni, hogy hány valós számot talált a programunk.

Definiálunk továbbá egy karakter csoportot **digit** néven, ami a tízes számrendszerben lévő számjegyeket tartalmazza. Ezt később használhatjuk és hsználni is fogjuk a szabályok definiálására.

```c
%%
{digit}*(\.{digit}+)? 
{
    realnum++;
	  printf("[realnum: %s %f]", yytext, atof(yytext));
}
%%
```

Ez a szabály fogja meghatározni, hogy a lexerünkben mi számít valós számnak. A szabályt, a könnyebb megértés érdekében bontsuk két részre:
`{digit}*` és `{(\.{digit}+)?}`. Ha ezeket részenként sikerül megértenünk, akkor szinte készen vagyunk a szabály "visszafejtésével", mivel ha ezt a két részt egymás után írjuk a szabályban, az azt jelenti, hogy ha egy string első része illeszkedik az első mintára és a második része a másodikra, akkor az egész szabályra illeszkedik (ezt hívjuk majd konkatenációnak).

A `{digit}*` szabály nem jelent mást, mint hogy adott a digit karaktercsoport, és ebből a karaktercsoportból bármennyi áll a vizsgált string elején (akár nulla is!) illeszkedni fog erre a részmintára.

A `(\.{digit}+)?` már egy kicsivel nehezebb dió. Látjuk, hogy a zárójel közötti részt ellátunk egy `?` operátorral, azaz akármilyen mintát is határoztunk meg a zárójeleken belül, az a minta egyszer fordulhat elő maximum (mondhatni opcionális). 

A zárójelben lévő rész egy `\.`-tal kezd. Mivel a `.` karakternek speciális jelentése van a Flexben, így ha szószerint a pont ASCII karakterre akarunk hivatkozni, akkor azt úgy tudjuk megtenni, ha egy \ jelet írunk elé (ezt hívjuk [escape characternek](https://en.wikipedia.org/wiki/Escape_character)). Tehát a második részminta egy ponttal kezdődik. Mi van még? `{digit}+`, Azaz legalább egy számjegy. A második részminta tehát így néz ki: egy pontot követ legalább 1, de ezen kívül akárhány számjegy; ez az egész minta pedig opcionális lesz a valós számainkban.

Ha egy fedél alá hozzuk a két mintát a szabályunk: bármennyi számjegyet (akár nullát is) opcionálisan követ egy pont és ha van pont akkor azt követnie kell legalább 1 számjegynek.

Valid valós számok a lexerünk számára:

```
.314
1221
6.022
07.27
0.0
```

Ezek után rendelkezünk arról, hogy mit szeretnénk tenni azzal a stringgel, ami illeszkedik a mintára.
```c
{
    realnum++;
	  printf("[realnum: %s %f]", yytext, atof(yytext));
}
```

Megnöveljük a realnum változó értékét, majd kiíratjuk a stringet ami illeszkedett, illetve a lebegőpontos ábrázolását.

```
Megjegyzés:

Az illeszkedő valós szám és a lebegőpontos változata között lehetnek eltérések, mivel több valós szám van (matematikában végtelen sok, a programunkban... memória kérdése, hogy mennyire nagyon nagyon sok), de 64 biten (az atof double típusú értékre konvertálja a stringet, amit 64 biten ábrázolunk azon a rendszereken, amiken mi dolgozunk) csak véges számú számot tudunk ábrázolni, így ha olyan stringet akarunk doublelá alakítani, aminek nincsen pontos megfelelője, akkor egy kerekített értéket kapunk (erről bővebben majd Alkalmazott matematikán).
```

```c
int main(){
	yylex();
	printf("Number of real numbers: %d\n", realnum);
	return 0;
}
```

Végül megírjuk a programunk többi részét. Lefuttatjuk a lexerünket, amit majd a Flex fog generálni a `yylex()` utasítással, majd kiírjuk a kimenetre, hogy hány valós számot találtunk. Ezután a program terminál.

Parancsok a fordításhoz

```bash
flex real.l
gcc lex.yy.c -o real -lfle
```

Előtte érdemes telepíteni a flex packaget. Ubuntun ez:
```
sudo apt install flex
```

## leet.l
A forrás egyetlen szabályt tartalmaz, ami nem egetrengetően nehéz, és a többi része a programnak pedig C black magic (legalábbis még számunkra az), így nem részletezem a programot nagyon.

A `.` karakter tetszőleges karaktert jelöl a Flexben, így ez a szabály annyit csinál, hogy bármilyen karakter illeszkedik rá, így minden karakternél, amit kap a porgam a bemenetről, megnézzük hogy tartalmazza-e a LEET szótárunk azt a karaktert, ha igen kiíratjuk valamelyik alternatáv alakját. Ha nem találta meg a szótárban  akkor pedig simán kiírja a karaktert.

Parancsok a fordításhoz

```bash
flex leet.l
gcc lex.yy.c -o leet -lfle
```