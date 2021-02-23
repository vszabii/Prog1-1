#include "Graph.h"
#include "Point.h"
#include "Simple_window.h"
#include "std_lib_facilities.h"
#include <FL/fl_draw.H>
#include <string>
#include <vector>

#undef vector

namespace düssz {
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
        int get_width() const {
            return m_width;
        }
        int get_height() const {
            return m_height;
        }

    private:
        int m_width;
        int m_height;
    };

    template<class T>
    class Vector_ref {
    public:
        Vector_ref() {}
        Vector_ref(T *elem) {
            if (elem)
                push_back(elem);
        }

        ~Vector_ref() {
            for (int i = 0; i < owned.size(); ++i)
                delete owned[i];
        }

        void push_back(T *elem) {
            elements.push_back(elem);
            owned.push_back(elem);
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
        void set_size(int size) {
            m_size = size;
        }

    private:
        std::string     m_label;
        Graph_lib::Font m_font{fl_font()};
        int             m_size{(fl_size() < 14) ? 14 : fl_size()};
    };

    struct Ellipse : Shape {
        Ellipse(Point center, int width, int height)
          : m_height(height), m_width(width) {
            add({center.x - width, center.y - m_height});
        }
        void draw_lines() const {
            if (fill_color().visibility()) {
                fl_color(fill_color().as_int());
                fl_pie(point(0).x, point(0).y, m_width * 2 - 1,
                       m_height * 2 - 1, 0, 360);
                fl_color(color().as_int());
            }

            if (color().visibility()) {
                fl_arc(point(0).x, point(0).y, m_width * 2, m_height * 2, 0,
                       360);
            }
        }

    private:
        int m_height;
        int m_width;
    };
} // namespace düssz

int main() {
    Simple_window    win{{0, 0}, 1200, 800, "valama"};
    düssz::Rectangle rect{{0, 0}, 400, 600};
    düssz::Text      txt{{400, 400}, "orrával a sárba döf"};
    düssz::Ellipse   eli{{600, 400}, 200, 120};

    txt.set_size(30);

    rect.set_fill_color(Color::dark_magenta);
    rect.move(100, 20);

    win.attach(rect);
    win.attach(txt);
    win.attach(eli);

    win.wait_for_button();
}