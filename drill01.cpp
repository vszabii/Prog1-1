#include "Simple_window.h"
#include "Graph.h"

double one(double) { return 1; }
double square(double x) { return x*x; }

int main()
{
	using namespace Graph_lib;

    int xmax = 600;
    int ymax = 400;

    int xorig = xmax/2;
    int yorig = ymax/2;

    int rmin = -10;
    int rmax = 10;

    int n_points = 1000;

    Simple_window win {Point{100,100}, xmax, ymax, "My window"};

    Point origo {xorig, yorig};

    int xlength = xmax - 40;
	int ylength = ymax - 40;

	int xscale = 30, yscale = 30;

	Axis x{Axis::x, Point{20, yorig}, xlength, xlength/xscale, "x "};
	Axis y{Axis::y, Point{xorig, ylength+20}, ylength, ylength/yscale, "y "};
	y.set_color(Color::cyan);
	y.label.set_color(Color::red);
	Function s (one, rmin, rmax, origo, n_points, xscale, yscale);
	Function sq (square, rmin, rmax, origo, n_points, xscale, yscale);
	Function sine{sin,0,100,origo,1000,40,40};
	sine.set_color(Color::yellow);

	Polygon poly;
	poly.add(origo);
	poly.add(Point{400,300});
	poly.add(Point{100,150});
	poly.set_color(Color::dark_green);
	poly.set_style(Line_style::dash);
	poly.set_fill_color(Color::dark_green);

	Text t{Point{30,100},"Hello, graphical world!"};
	t.set_font(Font::times_bold);
	t.set_font_size(20);

	Image ii {Point {0,0},"bjarne.jpg"};

	win.attach(ii);
	win.attach(t);
	win.attach(x);
	win.attach(y);
	win.attach(sine);
	win.attach(poly);
    win.wait_for_button();
}