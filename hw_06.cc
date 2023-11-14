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
    const Zebra morgana("Morgana");
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
    const ZebraKid james("James");
    // Use the getName() form Zebra class
    cout << james.getName() << '\n';
    cout << Zebra::getCount() << '\n';

    cout << "====[ end ]====" << '\n';
    cout << "               " << '\n';

    cout << " #### This is after the program is completely done #####" << '\n';
    cout << " ### The last remaining object should get destructed ###" << '\n';

    return 0;
}
