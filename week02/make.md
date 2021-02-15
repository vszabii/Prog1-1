# GNU Make

**Ez a rész nem lesz számonkérve semmilyen formában, csupán "érdekességnek" szánom**

A feladatok megoldásánál láttuk, hogy a programjaink fordításához hosszú parancsokat kell begépelnünk, sok paraméterrel, olyan részeket is beleszőve, amiket vagy próbálunk memorizálni vagy visszakeressük mindig amikor ki akarjuk adni a parancsot.

Megnézzük, hogy hogyan kell eljutni ettől:

```bash
g++ first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp -o first `fltk-config --ldflags --use-images` -std=c++14
```

Eddig:

```
make first
```

Ehhez egy GNU Make nevű eszközt fogunk használni, ami a legtobb Linux disztribúción alapértelmezetten megtalálható. Ha valakinek mégis hiányozna a rendszeréről, akkor (Ubuntun) ezzel a paranccsal telepítheti:

```bash
sudo apt install build-essential
```

A Maket leginkább fájlok előállítására szokás használni, jelentősen szűkebb a feladatköre, mint egy olyan build rendszernek, mint például a CMake vagy a Maven.

A programnak szánt információkat egy Makefile nevű fájlban kell összefoglalnunk, ha kiadjuk a make parancsot a parancssorban, akkor a program automatikusan ezt a fájlt fogja keresni az adott mappában.

A Makefile egy sajátos formátumot követnek:

```make
filenév: függőségek
   előállítás-parancs
```

A *filenév* lesz annak a fájlnak a neve, amit szeretnénk, hogy létrejöjjön. A *függőségek* az azoknak a fájloknak a listája, amik szükségesek ahhoz, hogy létre tudjuk hozni a *filenév* állományt. Az *előállítás-parancs* pedig egy shell parancs, vagy parancsok, amiket hogyha egymás után lefuttatunk az adott *függőségek* fájlokkal, akkor a folyamat végén létrejön a *filenév* fájl.

A függőségeknek is meghatározhatunk egy ehhez hasonló bejegyzést, amennyiben a függőséget elő kell állítani valamilyen fájlból. Függőség lehet forráskód, objectkód (tárgykódnak is hívják, .o kiterjesztés), de igazából bármilyen fájl, amire szükségünk lesz a célfájl (így hívjuk általában azt, ami az előző példában *filenév* volt) előállításához. A Make nemcsak előállítani tudja a fájlokat, de nyomonköveti azt, hogy mely függőség-fájlok módosultak az előző előállítás óta. Ha a célfájl már makeltük és nem változtak a függőségeg, akkor a program nem fogja újra elvégezni az előállítási folyamatot. Ha pedig van módosult függőség, akkor a fájl új verziójával újra előállítja a célfájlt. Ha a módosult fájlt is elő kell állítani, akkor előbb ezt állítja elő, majd a célfájlt, azaz ha egy célfájlt akarunk makelni, akkor minden függőségét makeljük rekurzívan.

Kezdjük is el átírni a fentebbi shell parancsot Makebe.

A példához a PPP. könyv "grafikus Hello World" programját fogjuk használni.

```c++
#include "Point.h"
#include "Simple_window.h"
#include "Graph.h"

int main(){
    using namespace Graph_lib;
    Graph_lib::Point tl{100, 100};
    Simple_window win {tl, 600, 400, "Canvas"};

    win.wait_for_button();

    return 0;
}
```

*A példákban feltételezem, hogy már átvettük a heti fejezet anyagát, azaz ismerősek a PPP könyvhöt tartozó fájlok.*

Ezt a forráskódot elmentjük *first.cpp* néven. Fontos, hogy egy mappába helyezzük a PPP könyv header- és forrásfájljaival.

Ahhoz hogy le tudjuk fordítani ezt a programot, a *first.cpp* forráson kívül még szükségünk lesz azokra a forrásfájlokra, amik az includeolt headerökhöz tartoznak. Ezek a *Graph.cpp Window.cpp GUI.cpp Simple_window.cpp* fájlok lesznek.

Ha mindezt megtettük, akkor ha lefuttatjuk az alábbi parancsot a terminálban, akkor létrejön a *first* nevű futtatható állomány.

```bash
g++ first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp -o first `fltk-config --ldflags --use-images` -std=c++14
```

Ha ezzel a paranccsal előáll a fájl amit szeretnénk, akkor ezt egy az egyben áthelyezhetjük egy Makefileba.

```make
first: first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp
	g++ first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp -o first `fltk-config --ldflags --use-images` -std=c++14
```

*first* néven fog előállni a célfájl, a célfájl függőségei a first.cpp Graph.cpp ... források, azaz ha bármyelyik is módosul, akkor újra elő kell állítani a célfájlt, és a második sorban lévő paranccsal állítjuk elő a célfájlt.

Ha a parancssorban lefuttatjuk a `make` vagy a `make first` parancsot akkor elő fog állni a futtatható állományunk. A két parancs között az a különbség, hogy a `make` a Makefileban lévő legelső célfájlt fogja előállítani, még a `make first` a *first* nevűt.

Az előállítási folyamat sikeres, így elkezdhetjük kicsit fejleszteni a szkriptünket. Szükségünk lenne egy módra, hogy automatikusan törölni tudjuk azokat a fájlokat, amik nem források és esetleg a makelés után nincs rájuk szükségünk, vagy vissza akarjuk állítani a mappa tartalmát a makelés előttre (esetünkben ez csak a *first* fájl törlését jelenti).

Adjunk hozzá a Makefilehoz egy *clean* bejegyzést. Mivel a clean nem lesz egy valós fájl ami létrejön, így jeleznünk kell majd a Make felé hogy ez nem egy konkrét fájl a projectünkben, hanem egy speciális utasítás.

```make
.PHONY: clean

first: first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp
	g++ first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp -o first `fltk-config --ldflags --use-images` -std=c++14

clean:
	rm -f first
```

Ha lefuttatjuk a `make clean` parancsot, akkor el fog tűnni a *first* fájl a mappából. Hogy a Make lássa, hogy a *clean* nem egy fájl ezért a *.PHONY* bejegyzés után helyeztük.

Mi a helyzet ha valaki nem g++-t akar használni, hanem valamilyen más fordítóprogramot, például clang++-t. A Makefileban létrejozhatunk változókat, amikre hivatkozhatunk a bejegyzésekben.

Változót létrehozni `NÉV=érték` alakban lehet, hivatkozni egy változó értékére pedig `$(NÉV)` alakkal. Tároljuk el egy változóban a C++ compiler nevét, és hivatkozzunk rá a fordítási parancsban; így aki más compilert akar használni, annak csak a változó értékét kell átírni a Makefileban. Íratlan szabály, hogy a C++ compilerekre *CXX* hivatkozunk. Így a Makefile a követkző formát ölti:

```make
.PHONY: clean

CXX=g++

first: first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp
	$(CXX) first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp -o first `fltk-config --ldflags --use-images` -std=c++14

clean:
	rm -f first
```

Mivel csak kisebb projecteket szertnénk makelni, így jó lenne valamennyire könnyen személyre (vagy inkább projectre) szabhatóvá tenni a Makefilet. A függőség fájlok fognak változni prjectről projectre, így azokat is érdemes lenne egy változóba kihelyezni.

```make
.PHONY: clean

CXX=g++
SOURCES=first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp

first: $(SOURCES)
	$(CXX) $(SOURCES) -o first `fltk-config --ldflags --use-images` -std=c++14

clean:
	rm -f first
```

Láthatjuk, hogy egy változóban el tudtuk tárolni több fájl nevét is úgy, hogy nem kellett tömböt, vagy valamilyen más tárolót létrehoznunk, mivel minden Make változó string típusú, és amikor hivatkozunk egy változóra, a program egyszerűen behelyettesíti a stringet a hivatkozás helyére.

Haladjunk tovább: szükségünk lenne arra, hogy rugalmasan tudjuk változtatni a compilernek adott flageket/kapcsolókat/paramétereket. Ha például valaki be szeretné kapcsolni a fordítási figyelmeztetéseket, akkor ezt meg kéne tudni oldania egy helyen... helyezzük ki a flageket egy változóba.

```make
.PHONY: clean

CXX=g++
SOURCES=first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp
CXXFLAGS=-o first -std=c++14

first: $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) `fltk-config --ldflags --use-images`

clean:
	rm -f first
```

Mivel nem tenném rá egyik páros szervemet sem, hogy minden projectünk a first nevet fogja viselni, így modularizáljuk ezt is.

```make
.PHONY: clean

CXX=g++
TARGET=first
SOURCES=first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp
CXXFLAGS=-o $(TARGET) -std=c++14

first: $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) `fltk-config --ldflags --use-images`

clean:
	rm -f first
```

Ha azt szeretnénk, hogy egy változó egy bejegyzésben más értéket vegyen fel, mint mindenhol máshol, akkor azt is meg tudjuk tenni `bejegyzés-neve: VÁLTOZÓ=érték` alakban. Érdemes minden olyan fájlnál, ami célspecifikus (források, futtatható fájl neve) ezzel a módszerrel meghatározni.

```make
.PHONY: clean

CXX=g++
CXXFLAGS=-o $(TARGET) -std=c++14

first: TARGET=first
first: SOURCES=first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp

second: TARGET=sec
second: SOURCES=sec.cpp

first: $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) `fltk-config --ldflags --use-images`

second: $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS)

clean:
	rm -f first
```

Ez egy elég fapados megoldás, de hatásos. Ha egy változót egy bejegyzésre specializálva deklaráltunk, akkor az a bejegyzés függőségeinek bejegyzéseiben is (ha vannak) ugyanazzal az értékkel fognak megjelenni. *(A second bejegyzést csak a példa kedvéért helyeztem el, nincs jelentősége.)*

Már csak két dolog van hátra.

Az FLTK library flagjeit helyezzük ki egy változóba, mivel már csak ez nincs paraméteresítve. 

```make
.PHONY: clean

CXX=g++
CXXFLAGS=-o $(TARGET) -std=c++14

first: TARGET=first
first: SOURCES=first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp
first: FLTKFLAGS=`fltk-config --ldflags --use-images`

second: TARGET=sec
second: SOURCES=sec.cpp

first: $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) $(FLTKFLAGS)

second: $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS)

clean:
	rm -f first
```

Ez lenne az egyik. Végül szeretnénkm ha az összes célfájlt egyetlen paranccsal tudnánk előállítani; ennek érdekében hozzunk létre egy speciális bejegyzést, az *all*-t.

```make
.PHONY: clean all

CXX=g++
CXXFLAGS=-o $(TARGET) -std=c++14

first: TARGET=first
first: SOURCES=first.cpp Graph.cpp Window.cpp GUI.cpp Simple_window.cpp
first: FLTKFLAGS=`fltk-config --ldflags --use-images`

second: TARGET=second
second: SOURCES=sec.cpp

BINS=first second
all: $(BINS)

first: $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) $(FLTKFLAGS)

second: $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS)

clean:
	rm -f $(BINS)
```

Hogy tudjunk hivatkozni később is az összes célfájl nevére, így egy *BINS* nevű segédváltozóban eltároljuk ezeket, és az *all* "álbejegyzés" függőségévé tesszük az értékét. Az *all*-t hozzáadjuk a *.PHONY* bejegyzésekhez, mivel ez sem egy valódi fájl, csak úgy kezeljük. A *clean* metódusban is átírjuk a törlendő fájlok nevét a *BINS* tartalmára.

Ha valaki egy új célfájlt szeretne hozzáadni a Makefilehoz a következőket kell tennie:

* A bejegyzésre vonatkozó változók deklarálása a látott módon
* A célfájl hozzáadása a *BINS*-hez
* A bejegyzés elhelyezése (célfájlnév, függőségek, előállítási parancs, változónevekkel, ha lehet)

Minden szerencsére CTRL-c és CTRL-v, illetve egy kis gépelés kérdése.

**Disclaimer: ez a Makefile semmilyen módon sem mondható támadhatatlannak, tökéletesnek, vagy komolyan vehetőnek. Csupán érdekességnek szántam, gondolatébresztőnek, csendes, esős délutánokra. Ismerkedjünk vele, próbáljuk ki, vonjunk le következtetéseket.**
