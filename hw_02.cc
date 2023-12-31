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
#include <cmath>  // double sin(double), double cos(double)
#include <fstream>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

// const for pi
constexpr auto PI = 3.14159265358979323846;

auto is_prime(int num) -> bool {
    for (auto i = 2; i < num; i += 1) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

auto main() -> int {
    cout << "###########" << endl;
    cout << "Problem One" << endl;
    cout << "###########" << endl;
    // define isPrime
    cout << "is 19 prime: " << std::boolalpha << is_prime(19);
    cout << '\n';

    // isPrime(81);// trial divide, 81%3 == 0 therefore not prime
    cout << "is 81 prime: " << is_prime(81);
    cout << '\n';

    // isPrime(57) ; // 57 % 2, 57% 3, 57%4
    cout << "is 57 prime: " << is_prime(57) << std::noboolalpha;
    cout << '\n';

    cout << "====[ end ]====" << endl;

    cout << "###########" << endl;
    cout << "Problem Two" << endl;
    cout << "###########" << endl;
    // For problem Two, you will need to write your code here
    // first read file robotpath.dat and name it thefile by using ifstream
    // make sure you use the full path to .dat file, e.g., .././src/robotpath.dat

    auto file = std::ifstream("./robotpath.dat");

    // check if thefile exist or not
    if (file.is_open()) {
        cout << "File is open and ready" << '\n';
        cout << " " << '\n';
    } else {
        cout << "File not found!" << '\n';
        cout << " " << '\n';
        return 1;
    }
    // define your variables each line r and theta from the polar coordinates
    // use defined constant pi value in your calculation
    // start reading file and do calculation
    double r = 0;
    double theta_degrees = 0;
    auto const default_precision = cout.precision();
    while (file >> r >> theta_degrees) {
        cout << "r: " << r << " theta degrees: " << theta_degrees << '\n';
        auto const theta_radian = theta_degrees * (PI / 180.0);
        cout << "theta radians: " << theta_radian << '\n';
        auto const x = r * cos(theta_radian);
        auto const y = r * sin(theta_radian);

        // set precision just for x and y, then set it back afterwards as the
        // question doesn't specify that anything else should use the increased
        // precision
        constexpr auto x_and_y_precision = 8;
        cout.precision(x_and_y_precision);
        cout << "x: " << x << " y: " << y << '\n';
        cout.precision(default_precision);

        auto const calculated_radians = atan2(y, x);
        cout << "calculated radians: " << calculated_radians << "\n\n";
    }
    // while reading print the following for each line:
    //  - print distance and angle
    //  - print angle in radian, 1 degree is equal (pi/180degree)
    //  - print x and y location
    //  - check your conversion and print angle in rad after finding x and y with
    //  cartesian coordinates conversion
    // don't forget to close your opened file

    cout << "====[ end ]====" << endl;

    cout << "###########" << endl;
    cout << "Problem Three" << endl;
    cout << "###########" << endl;
    // Write your code here

    // use Boolean CanRun to decide to do calculation or not based on condition
    bool can_run = true;
    // define your input value and other variable
    double temperature = 0;
    double wind_speed = 0;
    // print out message to enter wind speed and take the input
    // Check if speed meet condition
    cout << "Enter wind speed: ";
    cin >> wind_speed;
    if (wind_speed < 3) {
        can_run = false;
    }
    // print out message to enter temperature and take the input
    // check if temperature  meet condition
    cout << "Enter temperature: ";
    cin >> temperature;
    constexpr auto max_temperature = 50;
    if (temperature > max_temperature) {
        can_run = false;
    }
    // check if all condition is met and use canRun to start calculation of wind
    // chill print out the value
    if (can_run) {
        cout << "Wind chill: "
             << 35.74 + 0.6215 * temperature - 35.75 * pow(wind_speed, 0.16)
                + 0.4275 * temperature * pow(wind_speed, 0.16)
             << '\n';
    } else {
        cout << "Wind chill: N/A\n";
    }

    cout << "====[ end ]====" << endl;

    return 0;
}
