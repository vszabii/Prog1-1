# PPP. Chapter 12.

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [PPP. Chapter 12.](#ppp-chapter-12)
  - [Előfeltételek](#előfeltételek)
  - [A képernyő, ahogy a program látja](#a-képernyő-ahogy-a-program-látja)
  - [Hogyan hozzunk létre ablakot?](#hogyan-hozzunk-létre-ablakot)
  - [Hogyan jelenítsünk meg?](#hogyan-jelenítsünk-meg)
  - [Egyszerű alakzatok és megfigyelésük](#egyszerű-alakzatok-és-megfigyelésük)
    - [Axis](#axis)
    - [Function](#function)
    - [Polygon](#polygon)
    - [Rectangle](#rectangle)
    - [Text](#text)
    - [Image](#image)
    - [Circle](#circle)
    - [Ellipse](#ellipse)
    - [Mark](#mark)
  - [Egyéb](#egyéb)
    - [Color](#color)
    - [Line_style](#line_style)
    - [Font](#font)

<!-- /code_chunk_output -->

## Előfeltételek

A fejezetben egy FLTK nevű libaryvel és a PPP könyv előre megírt headerjeivel dolgozunk, így előbb szerezzük be mindkettőt.

FLTK (Ubuntun):

```bash
sudo apt install libfltk1.1-dev
```

Az FLTK 1.1-es varzióját használjuk, mivel a könyv forrásfájljai is ezzel a verzióval íródtak.

Fájlok:

[Előadáson használt fájlok](https://github.com/rbesenczi/Prog1/tree/main/week02-06/GUI)

## A képernyő, ahogy a program látja

A könyv (és a library) a képernyőt egy koordinátarendszerként képezeli el, ahol a képernyő bal felső sarka a (0, 0) koordinátájú pont, jobbra indulva az X koordináták nőnek (ahogy várható volt), lefele haladva pedig nőnek az Y koordináták (ez már kicsit ellentmond annak amire számítottunk). Az általunk létrehozott ablakok ugyanazt a rendszert követik (bal felsp sarok (0, 0) stb..)

## Hogyan hozzunk létre ablakot?

Egyelőre a `Simple_window` osztályt fogjuk erre használni. Ez leveszi az alakzatok kirajzoláást és a vászon léptetését a vállunkról.

```c++
Point topleft{100, 100};

Simple_window window{topleft, 600, 400, "Canvas"};

window.wait_for_button();
```

A konstruktor paraméterei:

1. A képernyőnk mely pontjában legyen az ablak bal felső sarka.
1. Az ablak szélesságe.
1. Az ablak magassága.
1. Az ablak fejlécében megjelenő szöveg.

Az ablakot a `window.wait_for_button()` metódussal rajzoljuk ki.
Az ablak fejlécében lévő szöveget a `window.set_label(...)` metódussal tudjuk megváltoztatni.

```c++
Point topleft{100, 100};

Simple_window window{topleft, 600, 400, "Canvas"};

window.set_label("New Canvas");
```

## Hogyan jelenítsünk meg?

Ha valamilyen Shapet (Axis, Rectangle, ...) ki akarunk rajzolni az ablakra, akkor (miután létrehoztuk az ablakot), meghívjuk az ablak `attach()` metódusát.

```c++
Simple_window window{...};

Rectangle rectangle{...};

window.attach(rectangle);
```

Az alakzat az ablakkal együtt lesz kirajzolva a `wait_for_button()` metódus meghívásakor.

## Egyszerű alakzatok és megfigyelésük

### Axis

Tengelyeket tudunk vele létrehozni.

```c++
Axis x_axis{Axis::x, Point{20, 30}, 280, 10, "x axis"};
```

A konstruktor paraméterei:

1. Milyen tengely (függőleges/vízszintes)
1. A vászon mely pontjában legyen a tengely sarka (Axis::x esetén ez a bal sarok, Axis::y esetén a jobb sarok)
1. Hány pixel széles legyen
1. Hány beosztás legyen rajta
1. A tengely címkéjében megjelenő szöveg

### Function

Matematikai értelemben vett függvényeket (bevesz egy számot, kidob egy számot valamilyen szabály alapján) tudunk vele kirajzolni.

```c++
Function sinus{sin, 0, 100, Point{20, 150}, 1000,  50, 50};
```

A konstruktor paraméterei:
1. Egy függvény (most a programozási értelemben), ami lehet a Standard libraryből, de akár mi magunk is megírhatjuk. (Fontos: csak olyan függvények jók ide, ami double-t ad vissza és egy darab double paramétere van)
1. és
1. egy intervallum alső és felső határa, ahonnan veszi a számokat, amit (ebben a példában a `sin`) függvénybe behelyettesít (értelmezési tartomány)
1. Egy képzeletbeli koordinátarendszerbe fogja kirajzolni a program a függvényt; ez a paraméter megadja, hogy a vászon melyik pontja a képzeletbeli koordinátarendszer (0, 0) pontja
1. Hány pontra ossza fel a 2. és 3. paraméter által meghatározott intervallumot
1. és
1. hányszorosára növelje a (példában `sin`) függvény felvett értéketit

### Polygon

Tetszőleges sokszöget lehet vele meghatározni.

```c++
Polygon polygon;
```

Új csúcsot hozzáadni az alakzathoz az `add()` metódussal lehet.

```c++
poly.add(Point{300, 200});
poly.add(Point{350, 100});
poly.add(...);
...
```

### Rectangle

Téglalapot hozunk vele létre.

Kétfajta konstruktora van.

```c++
Rectangle rectangle{Point{200, 200}, 100, 50};
```

1. Bal felső sarok
1. Szélesség
1. Hosszúság

```c++
Rectangle rectangle{Point{200, 200}, Point{500, 500}};
```

1. Bal felső sarok
1. Jobb alsó sarok

### Text

Szöveget tudunk vele megjeleníteni.

```c++
Text text{Point{150, 150}, "Hello, graphical world!"};
```

A konstruktor paraméterei:

1. A "szövegmező" bal alsó sarka
1. A szöveg

Tudjuk a szöveg nagyságát állítani:

```c++
text.set_font_size(20);
```

Illetve a betütípusát:

```c++
text.set_font_size(Font::times_bold);
```

### Image

Képet tudunk vele megjeleníteni.

```c++
Image image{Point{100, 50}, "image.jpg"};
```

A konstruktor paraméterei:

1. A kép balalsó sarka hova kerüljön a vásznon
1. A kép neve (ahogy a fájlrendszerben megtalálható)

A képet maszkolhatjuk (azaz kivághatunk belőle egy szeletet amit megtartunk)

```c++
image.set_mask(Point{40, 40}, 250, 150);
```

### Circle

Kört lehet vele létrehozni

```c++
Circle circle{Point{200, 200}, 20};
```

A konstruktor paraméterei:

1. A kör középpontja a vásznon
2. A kör sugara

### Ellipse

Ellipszist lehet vele létrehozni

```c++
Ellipse ellipse{Point{32, 32}, 13, 43};
```

A konstruktor paraméterei:

1. Az ellipszis középpontja a vásznon
1. Az ellipszis vízszintes tengelye
1. Az ellipszis függőleges tengelye

### Mark

Megjelölhetünk vele egy pontot

```c++
Mark mark{Point{100, 100}, 'x'};
```

A konstruktor paraméterei:

1. A jelölés helye
1. Egy karakter amivel jelölünk

## Egyéb

Minden Shapenek be lehet állítani kitöltés színt, vonal színt, vonal stílust, tudjuk mozgatni.

```c++
Rectangle rectangle{Point{100, 100}, 200, 300};

rectangle.set_fill_color();
rectangle.set_color(Color::);
rectangle.set_style(Line_style::solid);
rectangle.move(1, 1);
```

### Color

A következő színek érhetőek el:
| Kód |
| --- |
|`Color::red`|
|`Color::blue`|
|`Color::green`|
|`Color::yellow`|
|`Color::white`|
|`Color::black`|
|`Color::magenta`|
|`Color::cyan`|
|`Color::dark_red`|
|`Color::dark_green`|
|`Color::dark_yellow`|
|`Color::dark_blue`|
|`Color::dark_magenta`|
|`Color::dark_cyan`|

### Line_style

A következő vonalstílusok érhetőek el:
| Kód | Minta |
| --- | --- |
|`Line_style::solid`|------|
|`Line_style::dash`|- - - |
|`Line_style::dot`|......|
|`Line_style::dashdot`|- . - .|
|`Line_style::dashdotdot`|-..-..|

### Font

A következő betűtípusok érhetőek el:
| Kód |
| --- |
|`Font::helvetica`|
|`Font::helvetica_bold`|
|`Font::helvetica_italic`|
|`Font::helvetica_bold_italic`|
|`Font::courier`|
|`Font::courier_bold`|
|`Font::courier_italic`|
|`Font::courier_bold_italic`|
|`Font::times`|
|`Font::times_bold`|
|`Font::times_italic`|
|`Font::times_bold_italic`|
|`Font::symbol`|
|`Font::screen`|
|`Font::screen_bold`|
|`Font::zapf_dingbats`|
