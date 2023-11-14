#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

constexpr auto Pi = 3.14159265358979323846;

// -------------- Design your Shape class here ---------------
// -----------------------------------------------------------
// Shape abstract class with private double variable x and y that indicate shape position
// make the following functions
//  1. Constructor shape (x, y)
//  2. function area take no parameters return double const  equal zero
//  3. function draw take ostream file return void  equal zero
//  4. make deconstruct ~Shape
class Shape {
  private:
    // I don't like doing this, I'd prefer a pure interface instead of slapping state in there.
    double x_;
    double y_;

  public:
    Shape(double x_loc, double y_loc) : x_(x_loc), y_(y_loc) {}

    [[nodiscard]] auto x() const -> double {
        return x_;
    }

    [[nodiscard]] auto y() const -> double {
        return y_;
    }

    [[nodiscard]] virtual auto area() const -> double = 0;
    virtual void draw(ostream& out_stream) const = 0;
    virtual ~Shape() = default;
};

// -------------- Design your Circle class here ---------------
// -----------------------------------------------------------
// Class Circle inherit Class Shape
// private variable x, y, radius
// Make the following functions:
// Circle constructor that take x, y, radius, Note that x and y are assigned to Shape(x,y)
//  for example constructor_name(double x, double y) : Shape(x,y)
// draw that take ostream file to write postscript instruction the function return void
//   postscript:
//    x y radius  0  360  arc \n
//    stroke \n
// for example for x = 200 pixels, y = 3 pixels, and radius 100 pixels. then your function should write this in test.ps
// stroke
// 200 300 100 0 360 arc
// area take no parameters and return double
// ~Circle deconstruction
class Circle: public Shape {
  private:
    double radius;

  public:
    Circle(double x, double y, double radius) : Shape(x, y), radius(radius) {}

    [[nodiscard]] auto area() const -> double override {
        return Pi * radius * radius;
    }

    void draw(ostream& out_stream) const override {
        out_stream << x() << " " << y() << " " << radius << " 0 360 arc\nstroke\n";
    }
};

// -------------- Design your Rect class here ---------------
// -----------------------------------------------------------
// Class Rect inherit Class Shape
// private variable x, y, width, and height
// Make the following functions:
// Rect constructor that take x, y,  width,  height
// draw that take ostream file to write postscript instruction the function return void
// draw that take ostream file to write postscript instruction the function return void
//   postscript:
//    x y moveto \n
//    x+w y lineto \n
//    x+w y+h lineto \n
//    x y+h lineto \n
//    closepath \n
//    stroke \n
// area take no parameters and return double
// ~Rect deconstruction
class Rect: public Shape {
  private:
    double width;
    double height;

  protected:
    void common_draw(ostream& out_stream) const {
        out_stream << x() << " " << y() << " moveto\n";
        out_stream << x() + width << " " << y() << " lineto\n";
        out_stream << x() + width << " " << y() + height << " lineto\n";
        out_stream << x() << " " << y() + height << " lineto\n";
        out_stream << "closepath\n";
    }

  public:
    Rect(double x, double y, double width, double height) :
        Shape(x, y),
        width(width),
        height(height) {}

    [[nodiscard]] auto area() const -> double override {
        return width * height;
    }

    void draw(ostream& out_stream) const override {
        common_draw(out_stream);
        out_stream << "stroke\n";
    }
};

// -------------- Design your FilledRect class here ---------------
// -----------------------------------------------------------
// Class FilledRect inherit Class Shape
// private variable x, y, width, and height
// Make the following functions:
// FilledRect constructor that take x, y,  width,  height
// draw that take ostream file to write postscript instruction the function return void
//   postscript:
//    x y moveto \n
//    x+w y lineto \n
//    x+w y+h lineto \n
//    x y+h lineto \n
//    closepath \n
//    fill \n
// area take no parameters and return double
// ~FilledRect deconstruction
class FilledRect: public Rect {
  public:
    FilledRect(double x, double y, double width, double height) : Rect(x, y, width, height) {}

    void draw(ostream& out_stream) const override {
        common_draw(out_stream);
        out_stream << "fill\n";
    }
};

// -------------- Design your Drawing class here ---------------
// -----------------------------------------------------------
// private variables ofstream and vector<shape*>
// Make the following functions:
// Drawing constructor as following (is given):
//  Drawing(string filename) : f(filename.c_str()), shapes() {} // where f is ofstream
// void add that take shape and push_back to vector
// void draw take no parameters and loop inside vector to rum draw function for each shape class
// void showArea take no parameters and loop inside vector to run area function for each shape class
// void setrgb take three double for red, green, and blue and write postscript line to change color
//   postscript:
//    r_value g_value b_value setrgbcolor \n

class Drawing {
  private:
    ofstream f;
    vector<shared_ptr<Shape>> shapes;

  public:
    explicit Drawing(string const& filename) : f(filename.c_str()) {}  // where f is ofstream

    void add(shared_ptr<Shape> shape) {
        shapes.push_back(std::move(shape));
    }

    void draw() {
        for (auto const& shape : shapes) {
            shape->draw(f);
        }
    }

    void showArea() {
        for (auto const& shape : shapes) {
            cout << shape->area() << endl;
        }
    }

    void setrgb(double red, double green, double blue) {
        f << red << " " << green << " " << blue << " setrgbcolor\n";
    }
};

auto main() -> int {
    cout << "########" << endl;
    cout << "Main Problem" << endl;
    cout << "########" << endl;

    // set path to create .ps file to write postscript instructions
    Drawing drawing("./test.ps");
    // set color
    drawing.setrgb(1, 0, 0);  // set drawing color to be bright red:  1 0 0 setrgbcolor
    // add FilledRect
    drawing.add(shared_ptr<Shape>(new FilledRect(100.0, 550.0, 200.0, 50))
    );  // x y moveto x y lineto ... fill
    // add Rect
    drawing.add(shared_ptr<Shape>(new Rect(200.0, 650.0, 200.0, 50))
    );  // x y moveto x y lineto ... stroke
    // add Circle
    drawing.add(shared_ptr<Shape>(new Circle(300, 300, 100))
    );  // centered in 300 , 300 pixel with radius
    // draw all shapes added to d
    drawing.draw();
    // print out all shapes area
    drawing.showArea();

    cout << "====[ end ]====" << endl;
    cout << "               " << endl;

    return 0;
}
