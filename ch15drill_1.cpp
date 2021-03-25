#include "Graph.h"
#include "Simple_window.h"
#include <iostream>

double one(double x){return 1; }
double slope(double x){return x/2; } 
double square(double x){return x*x; }
double sloping_cos(double x) { return cos(x) + slope(x); }

int main()
{
	Simple_window win {Point{100, 100}, 600, 600, "Function graphs"};

    Axis x {Axis::x, Point{100, 300}, 400, 20, "1 == 20 pixels"};
    Axis y {Axis::y, Point{300, 500}, 400, 20, "1 == 20 pixels"};

    x.set_color(Color::red);
    y.set_color(Color::red);

    win.attach(x);
    win.attach(y);

    constexpr int r_min = -10;
    constexpr int r_max = 11;

    constexpr int num_points = 400;

    constexpr int x_scale = 20;
    constexpr int y_scale = 20;

    const Point orig {300, 300};

    Function first {one, r_min, r_max, orig, num_points, x_scale, y_scale};
    Function second {slope, r_min, r_max, orig, num_points, x_scale, y_scale};
    Function third {[](double x) { return x * x; },
            r_min, r_max, orig, num_points, x_scale, y_scale};
    Function fourth {cos, r_min, r_max, orig, num_points, x_scale, y_scale};
    fourth.set_color(Color::blue);
    Function fifth {[](double x) { return cos(x) + slope(x); },
            r_min, r_max, orig, num_points, x_scale, y_scale};

    win.attach(first);
    win.attach(second);
    win.attach(third);
    win.attach(fourth);
    win.attach(fifth);



    win.wait_for_button();
}