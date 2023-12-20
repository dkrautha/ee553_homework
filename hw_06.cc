// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute
// this software, either in source code form or as a compiled binary, for any
// purpose, commercial or non-commercial, and by any means.
//
// In jurisdictions that recognize copyright laws, the author or authors of this
// software dedicate any and all copyright interest in the software to the public
// domain. We make this dedication for the benefit of the public at large and to
// the detriment of our heirs and successors. We intend this dedication to be an
// overt act of relinquishment in perpetuity of all present and future rights to
// this software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <http://unlicense.org/>
#include <iostream>
#include <string>
#include <utility>

using std::cout;
using std::string;

class Zebra {
    static int count;
    string name_ = "unnamed";

  public:
    static auto getCount() -> int {
        return count;
    }

    Zebra() {
        count += 1;
        cout << "Hello, I'm just born! I don't have a name yet!" << '\n';
    }

    explicit Zebra(string name) : name_(std::move(name)) {
        count += 1;
        cout << "Hello I'm just born! My name is: " << name_ << '\n';
    }

    [[nodiscard]] auto getName() const -> string const& {
        return name_;
    }

    ~Zebra() {
        count -= 1;
        cout << "Ugh! I'm dying and the current count is: " << count << '\n';
    }
};

class ZebraKid: public Zebra {
  public:
    explicit ZebraKid(string name) : Zebra(std::move(name)) {}
};

int Zebra::count = 0;

void f() {
    cout << "Current num of Zebras: " << Zebra::getCount() << '\n';
    Zebra const joker("Joker");
    Zebra const alice("Alice");
    cout << "Current num of Zebras: " << Zebra::getCount() << '\n';
}

auto main() -> int {
    cout << "########" << '\n';
    cout << "Main Problem" << '\n';
    cout << "########" << '\n';
    // You should be able to run main() with your classes
    // No need to edit this part unless for debugging
    // print the initial count number
    cout << Zebra::getCount() << '\n';  // zero!
    // Creat object a with name Morgana
    Zebra const morgana("Morgana");
    // Print the current count number of Zerba object
    cout << Zebra::getCount() << '\n';
    // Use the function f() above
    f();
    cout << "####" << '\n';
    // Use the function f() above again
    f();
    cout << "####" << '\n';
    // print out the current number of zebra
    cout << Zebra::getCount() << '\n';
    // Get the name of the zebra object a
    cout << morgana.Zebra::getName() << '\n';
    cout << "####" << '\n';

    // create  ZebraKid object
    ZebraKid const james("James");
    // Use the getName() form Zebra class
    cout << james.getName() << '\n';
    cout << Zebra::getCount() << '\n';

    cout << "====[ end ]====" << '\n';
    cout << "               " << '\n';

    cout << " #### This is after the program is completely done #####" << '\n';
    cout << " ### The last remaining object should get destructed ###" << '\n';

    return 0;
}
