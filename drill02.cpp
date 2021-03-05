#include "Simple_window.h"
#include "Graph.h"

int main()
{
	using namespace Graph_lib;
	
	int xmax = 800;
	int ymax = 1000;

	Simple_window win = {{100,100},xmax,ymax,"Duranda"};

	int x_size = win.x_max();
	int y_size = 801;

	int xgrid = 100;
	int ygrid = 100;

	Lines grid;
	for (int x = xgrid; x < x_size; x+=xgrid)
	{
		grid.add(Point{x,0},Point{x,y_size});
	}
	for (int y = ygrid; y < y_size; y+=ygrid)
	{
		grid.add(Point{0,y},Point{x_size,y});
	}
	
	Rectangle rect01(Point{0,0},100,100);
	rect01.set_fill_color(Color::red);
	Rectangle rect02(Point{100,100},100,100);
	rect02.set_fill_color(Color::red);
	Rectangle rect03(Point{200,200},100,100);
	rect03.set_fill_color(Color::red);
	Rectangle rect04(Point{300,300},100,100);
	rect04.set_fill_color(Color::red);
	Rectangle rect05(Point{400,400},100,100);
	rect05.set_fill_color(Color::red);
	Rectangle rect06(Point{500,500},100,100);
	rect06.set_fill_color(Color::red);
	Rectangle rect07(Point{600,600},100,100);
	rect07.set_fill_color(Color::red);
	Rectangle rect08(Point{700,700},100,100);
	rect08.set_fill_color(Color::red);

	Image pic1 {Point{0,400},"duranda.jpeg"};
	pic1.set_mask(Point{0,0},200,200);
	Image pic2 {Point{0,600},"duranda.jpeg"};
	pic2.set_mask(Point{0,0},200,200);
	Image pic3 {Point{400,0},"duranda.jpeg"};
	pic3.set_mask(Point{0,0},200,200);
	Image pic4 {Point{600,200},"duranda.jpeg"};
	pic4.set_mask(Point{0,0},200,200);

	
	win.attach(grid);
	win.attach(rect01);
	win.attach(rect02);
	win.attach(rect03);
	win.attach(rect04);
	win.attach(rect05);
	win.attach(rect06);
	win.attach(rect07);
	win.attach(rect08);
	win.attach(pic1);
	win.attach(pic2);
	win.attach(pic3);
	win.attach(pic4);
	win.wait_for_button();

	Image spng {Point{0,0},"spng.jpeg"};
	win.attach(spng);

	win.wait_for_button();

	int x = 0;
    int y = 0;
    int dx = 0;
    int dy = 0;
    while (true) {
        x = randint(7);
        y = randint(7);
        dx = 100*x - spng.point(0).x;
        dy = 100*y - spng.point(0).y;
        spng.move(dx,dy);
        //win.resize(1000,800);
        win.wait_for_button();
    }

}