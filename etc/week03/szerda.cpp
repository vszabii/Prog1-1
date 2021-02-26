#include "GUI.h"
#include "Point.h"
#include "Simple_window.h"
#include "Graph.h"
#include <FL/fl_draw.H>
#include <string>

#undef vector

namespace vuk {
    struct Rectangle : Shape {
        Rectangle(Point topleft, int width, int height)
          : m_width(width), m_height(height) {
            add(topleft);
        }

        Rectangle(Point topleft, Point bottomright)
          : m_width(bottomright.x - topleft.x),
            m_height(bottomright.y - topleft.y) {
            add(topleft);
        }

        void draw_lines() const {
            if (fill_color().visibility()) {
                fl_color(fill_color().as_int());
                fl_rectf(point(0).x, point(0).y, m_width, m_height);
                fl_color(color().as_int());
            }

            if (color().visibility()) {
                fl_color(color().as_int());
                fl_rect(point(0).x, point(0).y, m_width, m_height);
            }
        }

        int height() const {
            return m_height;
        }
        int width() const {
            return m_width;
        }

    private:
        int m_width;
        int m_height;
    };

    template<class T>
    class Vector_ref {
    public:
        Vector_ref(){};
        Vector_ref(T *elem) {
            if (elem)
                push_back(elem);
        }

        ~Vector_ref() {
            for (int i = 0; i < owned.size(); ++i)
                delete owned[i];
        };

        void push_back(T *elem) {
            owned.push_back(elem);
            elements.push_back(elem);
        }
        void push_back(T &elem) {
            elements.push_back(&elem);
        }

        T &operator[](int i) {
            return *elements[i];
        }
        const T &operator[](int i) const {
            return *elements[i];
        }

        int size() const {
            return elements.size();
        }

    private:
        std::vector<T *> elements;
        std::vector<T *> owned;
    };

    struct Text : Shape {
        Text(Point bottomleft, std::string const &label) : m_label(label) {
            add(bottomleft);
        }
        void draw_lines() const {
            fl_draw(m_label.c_str(), point(0).x, point(0).y);
        }

        void set_label(std::string label) {
            m_label = label;
        }
        void set_font(Graph_lib::Font font) {
            m_font = font;
        }
        void set_fontSize(int fontSize) {
            m_fontSize = fontSize;
        }

    private:
        std::string     m_label;
        Graph_lib::Font m_font{fl_font()};
        int             m_fontSize{fl_size() < 14 ? 14 : fl_size()};
    };

    struct Ellipse : Shape {
        Ellipse(Point center, int height, int width)
          : m_height(height), m_width(width) {
            add({center.x - width, center.y - height});
        }

        int width() const {
            return m_width;
        }
        int height() const {
            return m_height;
        }
        Point center() const {
            return Point{point(0).x + m_width, point(0).y + m_height};
        }

        void draw_lines() const {
            if (fill_color().visibility()) {
                fl_color(fill_color().as_int());
                fl_pie(point(0).x, point(0).y, 2 * m_width - 1,
                       2 * m_height - 1, 0, 360);
                fl_color(color().as_int());
            }

            if (color().visibility()) {
                fl_arc(point(0).x, point(0).y, m_width * 2, m_height * 2, 0,
                       360);
            }
        }

    private:
        int m_width;
        int m_height;
    };
} // namespace vuk

int main() {
    Simple_window  win{{0, 0}, 1200, 800, "Window"};
    vuk::Rectangle rect{{0, 0}, 400, 234};

    rect.move(100, 50);
    rect.set_color(Color::dark_cyan);
    rect.set_fill_color(Color::dark_cyan);

    vuk::Text txt{{300, 299}, "Frakk a macskák réme"};
    txt.set_fontSize(727);

    win.attach(rect);
    win.attach(txt);

    vuk::Ellipse elli{{600, 400}, 100, 120};

    Mark mk1{{600, 400}, 'x'};
    Mark mk2{elli.center(), 'o'};

    elli.set_color(Color::magenta);
    elli.set_fill_color(Color::dark_magenta);

    win.attach(elli);
    win.attach(mk1);
    win.attach(mk2);

    win.wait_for_button();
}