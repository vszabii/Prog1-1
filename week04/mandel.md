# [Mandelbrot nagyító](https://github.com/rbesenczi/BHAXBook/tree/master/Mandelbrot/halmaz/mandel_mento)

A feladat megértéseb sokat segíthet ha tisztában vagyunk kicsit a Mandelbrot-halmazzal, illetve a [Qt keretrendszer alapjaival](./week03/qt.md).

A feladat megoldásához ismét szükségünk van a Qt keretrendszerre.

Ubuntun esetében ezt a következő paranccsal tehetjük:

```bash
sudo apt install qt5-default
```
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Mandelbrot nagyító](#mandelbrot-nagyítóhttpsgithubcomrbesenczibhaxbooktreemastermandelbrothalmazmandel_mento)
  - [main.cpp](#maincpp)
  - [frakablak.h](#frakablakh)
    - [Konstruktor](#konstruktor)
    - [Destruktor](#destruktor)
    - [Publikus adattagok](#publikus-adattagok)
    - [`void vissza(...);`](#void-vissza)
    - [Qt eventek kezelése](#qt-eventek-kezelése)
    - [Privát adattagok](#privát-adattagok)
  - [frakablak.cpp](#frakablakcpp)
    - [`FrakAblak()`](#frakablak)
    - [`~FrakAblak()`](#~frakablak)
    - [`void paintEvent(...)`](#void-paintevent)
    - [`void mousePressEvent(...)`](#void-mousepressevent)
    - [`void mouseMoveEvent(...)`](#void-mousemoveevent)
    - [`void mouseReleaseEvent(...)`](#void-mousereleaseevent)
    - [`void keyPressEvent(...)`](#void-keypressevent)
    - [`void vissza(int magassag, int *sor, int meret)`](#void-visszaint-magassag-int-sor-int-meret)
    - [`void vissza(void)`](#void-visszavoid)
  - [frakszal.h](#frakszalh)
    - [Adattagok](#adattagok)
  - [frakszal.cpp](#frakszalcpp)
    - [`FrakSzal(...)`](#frakszal)
    - [`~FrakSzal(...)`](#~frakszal)
    - [`void run()`](#void-run)

<!-- /code_chunk_output -->

## main.cpp

```c++
#include <QApplication>
#include "frakablak.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    FrakAblak w1;
    w1.show();

    return a.exec();
}

```

Létrehozunk egy `QApplication` objektumot, ami szükséges az alkalmazás futtathatóságához.

Ezután létrehozunk egy `FrakAblak` objektumot, amit magáért a vizuális élményért fele. A megjelenítéséhez meghívjuk a `show()` etóduusát. Hogy az alkalmazásunk futása kezdetét vegye, ki kell adnunk a `return a.exec();` utasítást.

## frakablak.h

```c++
#ifndef FRAKABLAK_H
#define FRAKABLAK_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include "frakszal.h"

class FrakSzal;

class FrakAblak : public QMainWindow
{
    Q_OBJECT

public:
    FrakAblak(double a = -2.0, double b = .7, double c = -1.35,
              double d = 1.35, int szelesseg = 800,
              int iteraciosHatar = 255, QWidget *parent = 0);
    ~FrakAblak();
    void vissza(int magassag , int * sor, int meret) ;
    void vissza(void) ;
    // A komplex sík vizsgált tartománya [a,b]x[c,d].
    double a, b, c, d;
    // A komplex sík vizsgált tartományára feszített
    // háló szélessége és magassága.
    int szelesseg, magassag;
    // Max. hány lépésig vizsgáljuk a z_{n+1} = z_n * z_n + c iterációt?
    // (tk. most a nagyítási pontosság)
    int iteraciosHatar;

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void keyPressEvent(QKeyEvent*);

private:
    QImage* fraktal;
    FrakSzal* mandelbrot;
    bool szamitasFut;
    // A nagyítandó kijelölt területet bal felsõ sarka.
    int x, y;
    // A nagyítandó kijelölt terület szélessége és magassága.
    int mx, my;
    int kepszam;
};

#endif // FRAKABLAK_H
```

### Konstruktor

Minden paraméterének adunk alapértelmezett értéket. A paraméterek:

- `double a`: a komplex számok valós részének alsó határa
- `double b`: a komplex számok valós részének felső határa
- `double c`: a komplex számok képzetes részének alsó határa
- `double d`: a komplex számok képzetes részének felső határa
- `int szelesseg`: a kirajzolandó kép szélessége pixelekben
- `int iteraciosHatar`: a macimális iterációk számának határa akkor, amikor egy pixelről/komplex számról el akarjuk dönteni, ogy része-e a halmaznak
- `QWidget *parent`: egy `QWidget` objektumra mutató pointer, amiben szeretnénk hogy a `FrakAblak` megjelenjen; ha ez 0 (vagy elyesebben `nullptr`) akkor önálló ablak lesz.

### Destruktor

Mivel lesznek dinamikusan allokált erőforrásaink, így ezeket majd fel kell szabadítanunk a destruktorban.

### Publikus adattagok

A konstruktornál már beszéltünk róluk, az `int magassag` kivételével, ami a megjelenítendő kép magassága.

### `void vissza(...);`

Ezek a matódusok felelősek azért, hogy a kép egy-egy kiszámított sorát megjelenítse. A `void vissza(void);` felelős azért, hogy a minden sort megjelenítettünk, akkor "nyugalmi" állapotba állítsa a programot.

### Qt eventek kezelése

A protected címke után lévő öt metódus felel minden esemény kezeléséért. Beszédes nevük miatt nem térünk ki ebben a részben ájuk, annyit kell tudnunk, hogy a Qt keretrendszer a háttérben figyeli a bejövő eventeket és ha valamelyikhez talál kezelőt, kkor automatikusan meghívja a kezelő metódust.

### Privát adattagok

- `QImage fraktal`: ebben az objektumban tároljuk a kiszámított képet, így lehetőségünk van rugalmasabban kezelni, irajzolhatjuk a `FrakAblak`-ban, de akár el is menthetjük fájlként
- `FrakSzal mandelbrot`: egy `QThread`-ből származtatott objektum, ami magának a halmaz alguritmusának a számítását
- `bool szamitasfut`: itt tároljuk el az (főleg az eventek kezelése miatt), hogy éppen számítunk-e képet vagy nem
- `int x, y`: a nagyítandó kijelölt területet bal felsõ sarkának koordinátái
- `int mx, my`: a nagyítandó kijelölt terület szélessége és magassága
- `int kepszam`: annak a száma, hogy hány képet mentettünk fájlként

## frakablak.cpp

```c++
// frakablak.cpp
//
// Mandelbrot halmaz nagyító

#include "frakablak.h"

FrakAblak::FrakAblak(double a, double b, double c, double d,
                     int szelesseg, int iteraciosHatar, QWidget *parent)
                         : QMainWindow(parent)
{
    setWindowTitle("Mandelbrot halmaz");

    szamitasFut = true;
    x = y = mx = my = 0;
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->szelesseg = szelesseg;
    this->iteraciosHatar = iteraciosHatar;
    magassag = (int)(szelesseg * ((d-c)/(b-a)));

    setFixedSize(QSize(szelesseg, magassag));
    fraktal= new QImage(szelesseg, magassag, QImage::Format_RGB32);

    mandelbrot = new FrakSzal(a, b, c, d, szelesseg, magassag, iteraciosHatar, this);
    mandelbrot->start();
    
    kepszam=0;
}

FrakAblak::~FrakAblak()
{
    delete fraktal;
    delete mandelbrot;
}

void FrakAblak::paintEvent(QPaintEvent*) {
    QPainter qpainter(this);
    qpainter.drawImage(0, 0, *fraktal);
    if(!szamitasFut) {
        qpainter.setPen(QPen(Qt::white, 1));
        qpainter.drawRect(x, y, mx, my);

    }
    qpainter.end();
}

void FrakAblak::mousePressEvent(QMouseEvent* event) {

    // A nagyítandó kijelölt területet bal felsõ sarka:
    x = event->x();
    y = event->y();
    mx = 0;
    my = 0;

    update();
}

void FrakAblak::mouseMoveEvent(QMouseEvent* event) {

    // A nagyítandó kijelölt terület szélessége és magassága:
    mx = event->x() - x;
    my = mx; // négyzet alakú

    update();
}

void FrakAblak::mouseReleaseEvent(QMouseEvent* event) {

    if(szamitasFut)
        return;

    szamitasFut = true;

    double dx = (b-a)/szelesseg;
    double dy = (d-c)/magassag;

    double a = this->a+x*dx;
    double b = this->a+x*dx+mx*dx;
    double c = this->d-y*dy-my*dy;
    double d = this->d-y*dy;

    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;

    delete mandelbrot;
    mandelbrot = new FrakSzal(a, b, c, d, szelesseg, magassag, iteraciosHatar, this);
    mandelbrot->start();

    update();
}

void FrakAblak::keyPressEvent(QKeyEvent *event)
{

    if(szamitasFut)
        return;

    if (event->key() == Qt::Key_N)
        iteraciosHatar *= 2;
    szamitasFut = true;

    delete mandelbrot;
    mandelbrot = new FrakSzal(a, b, c, d, szelesseg, magassag, iteraciosHatar, this);
    mandelbrot->start();
    if (event->key() == Qt::Key_S)
    {
		fraktal->save("mandel_"+QString::number(kepszam++)+".png");
    }

}


void FrakAblak::vissza(int magassag, int *sor, int meret)
{
    for(int i=0; i<meret; ++i) {
        QRgb szin = qRgb(0, 255-sor[i], 0);
        fraktal->setPixel(i, magassag, szin);
    }
    update();
}

void FrakAblak::vissza(void)
{
    szamitasFut = false;
    x = y = mx = my = 0;
}


```

### `FrakAblak()`

A paraméterrekként átadott értékekkel inicializálunk mindent amit tudunk; a `magassag` adattagot kikövetkeztetjük a szélesség, lletve a határok arányaiból (mert miért ne).

A `setFixedSize(QSize(szelesseg, magassag));` sorral megtiltjuk az ablak méretének megváltoztatását. A `fratkal` adattagra llokálunk egy üres `QImage` objektumot, az adattagok között szereplő méretben és minden pixelt 32 biten ábrázolunk, RGB zíntérben.

Létrehozunk egy `FrakSzal` objektumot a `mandelbrot` adattagra, amit útjára is indítunk, hogy kezdje el számítani a képet.

### `~FrakAblak()`

Mivel a `fraktal` és a `mandelbrot` adattagokat dinamikusan  allokáltuk, így ezeket fel kell szabadítanunk a destruktorban.

### `void paintEvent(...)`

Ez a metódus fog meghívódni, ha valaki meghívja az `update()` függvényt.

Ez a metódus lesz a felelős azért, hogy kirajzolja a "vászonra" az `fraktal`-ban eltárolt képet és amennyiben nem fit számítás,  kijelölésünket jelző fehér téglalapot.

### `void mousePressEvent(...)`

Egérgomb lenyomása esetén a kijelölés bal felső sarkát "lehorgonyozza" és egy üres kijelölést határoz meg; újrarajzoltatja az blakot.

### `void mouseMoveEvent(...)`

Átálítja a kijelölt terület méretét annak függvényében, hogy mennyire mozgattuk el a kurzort. A kijelölést négyzet alakúra ormázza és előidézi az ablak újrarajzolását.

### `void mouseReleaseEvent(...)`

Az egérgomb elengedésével idézhetjük elő egy új kép kiszámítását, a kijelölés által meghatározott új határokkal. Az előző renderelést végző `FrakSzal`-at töröljük, és egy újat hozunk létre az új számítási paraméterekkel.

### `void keyPressEvent(...)`

Itt definiáljuk, hogy bizonyos billentyűk lenyomása milyen változást váltson ki a programban. Amennyiben éppen folyik egy kép kirajzolása/számítása nem teszünk semmit, a függvény visszatér. Amennyiben a megnyomott gomb N, megduplázzuk a maximális iterációk határát, azaz élesítünk a képen. Az új iterációs határral létrehozunk egy új `FrakSzal`-at és újrarajzoljuk a képet. Amennyiben S a lenyomott billentyű, elmentjük a képet.

(*Megj:* az hogy kép mentés előtt miért kell újra kiszámítani a képet, számomra még rejtély.)

### `void vissza(int magassag, int *sor, int meret)`

Egy adott sor pixeljeit fogja kiszínezni a `FrakSzal` által kiszámított értékek alapján

A metódus paraméterei:

- `int magassag`: hanyadik sor pixeleit fogjuk kiszínezni
- `int *sor`: C-szerű tömb (C pointerekkel oldja meg a tömböket), fogadjuk el hogy ez az és soha ne használjuk saját kódban
- `int meret`: a `sor` "tömb" mérete; sajnos a C-szírű tömbök nem tudják hogy hány elemük van, szóval ha nem adjuk át egy mésik paraméterben, good luck

### `void vissza(void)`

Az utolsó sor kiszámítása után "megszünteti" a kijelölést azzal, hogy a kijelölés balfelső sarkát, szélesség és magasságát kinullázza és átállítja a `szamitasFut` tagot hamisra.

## frakszal.h

```c++
#ifndef FRAKSZAL_H
#define FRAKSZAL_H

#include <QThread>
#include <math.h>
#include "frakablak.h"

class FrakAblak;

class FrakSzal : public QThread
{
    Q_OBJECT

public:
    FrakSzal(double a, double b, double c, double d,
             int szelesseg, int magassag, int iteraciosHatar, FrakAblak *frakAblak);
    ~FrakSzal();
    void run();

protected:
    // A komplex sík vizsgált tartománya [a,b]x[c,d].
    double a, b, c, d;
    // A komplex sík vizsgált tartományára feszített
    // háló szélessége és magassága.
    int szelesseg, magassag;
    // Max. hány lépésig vizsgáljuk a z_{n+1} = z_n * z_n + c iterációt?
    // (tk. most a nagyítási pontosság)
    int iteraciosHatar;
    // Kinek számolok?
    FrakAblak* frakAblak;
    // Soronként küldöm is neki vissza a kiszámoltakat.
    int* egySor;

};

#endif // FRAKSZAL_H
```

A konstruktor nagyjából magáért beszél, akárcsak a destruktor, a `run()` metódus meghívásával fogjuk kiszámítani a képet.

### Adattagok

A legtöbb adattag már ismerős lehet a `FrakAblak` osztályból az `egySor` kivételével. Ebben a "tömbben" fogjuk eltárolni annak a sornak az adatait, amit kiszámoltunk, és ezt fogjuk átadni a `vissza(...)` metódusnak.

## frakszal.cpp

```c++
#include "frakszal.h"

FrakSzal::FrakSzal(double a, double b, double c, double d,
                   int szelesseg, int magassag, int iteraciosHatar, FrakAblak *frakAblak)
{
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->szelesseg = szelesseg;
    this->iteraciosHatar = iteraciosHatar;
    this->frakAblak = frakAblak;
    this->magassag = magassag;

    egySor = new int[szelesseg];
}

FrakSzal::~FrakSzal()
{
    delete[] egySor;
}

void FrakSzal::run()
{
    // A [a,b]x[c,d] tartományon milyen sûrû a
    // megadott szélesség, magasság háló:
    double dx = (b-a)/szelesseg;
    double dy = (d-c)/magassag;
    double reC, imC, reZ, imZ, ujreZ, ujimZ;
    // Hány iterációt csináltunk?
    int iteracio = 0;
    // Végigzongorázzuk a szélesség x magasság hálót:
    for(int j=0; j<magassag; ++j) {
        //sor = j;
        for(int k=0; k<szelesseg; ++k) {
            // c = (reC, imC) a háló rácspontjainak
            // megfelelõ komplex szám
            reC = a+k*dx;
            imC = d-j*dy;
            // z_0 = 0 = (reZ, imZ)
            reZ = 0;
            imZ = 0;
            iteracio = 0;
            // z_{n+1} = z_n * z_n + c iterációk
            // számítása, amíg |z_n| < 2 vagy még
            // nem értük el a 255 iterációt, ha
            // viszont elértük, akkor úgy vesszük,
            // hogy a kiinduláci c komplex számra
            // az iteráció konvergens, azaz a c a
            // Mandelbrot halmaz eleme
            while(reZ*reZ + imZ*imZ < 4 && iteracio < iteraciosHatar) {
                // z_{n+1} = z_n * z_n + c

                ujreZ = reZ*reZ - imZ*imZ + reC;
                ujimZ = 2*reZ*imZ + imC;

                reZ = ujreZ;
                imZ = ujimZ;

                ++iteracio;

            }
            // ha a < 4 feltétel nem teljesült és a
            // iteráció < iterációsHatár sérülésével lépett ki, azaz
            // feltesszük a c-rõl, hogy itt a z_{n+1} = z_n * z_n + c
            // sorozat konvergens, azaz iteráció = iterációsHatár
            // ekkor az iteráció %= 256 egyenlõ 255, mert az esetleges
            // nagyítasok során az iteráció = valahány * 256 + 255

            iteracio %= 256;

            //a színezést viszont már majd a FrakAblak osztályban lesz
            egySor[k] = iteracio;
        }
        // Ábrázolásra átadjuk a kiszámolt sort a FrakAblak-nak.
        frakAblak->vissza(j, egySor, szelesseg);
    }
    frakAblak->vissza();

}
```

### `FrakSzal(...)`

A konstruktorban értéket adunk az adattagoknak, és allokálunk egy integer "tömböt", amiben a számításokat fogjuk végezni.

### `~FrakSzal(...)`

A destruktorban felszabadítjuk az allokált tömböt.

### `void run()`

Ebben a metódusban futtatjuk a Mandelbrot-halmazt kirajzoló algoritmust. Az algoritmus minden iterációjában beszínezzük az `egySor` tömb éppen adott pontját és ha egy sorral készen vagyunk, akkor meghívjuk a "kijelölés resetelő" `vissza()` metódust.
