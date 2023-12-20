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
#include <ctime>
#include <iostream>

using std::cout;
using std::endl;

class JulianDate {
    // Make all your class access is public

  public:
    int year;
    int month;
    int day;
    int hour;
    int mins;
    int sec;

    // BAD API: 6 adjacent parameters of similar type could easily be swapped
    // SOLUTION: use strong type wrappers / enum classes when possible
    // Custom constructor that sets the various fields
    JulianDate(
        int const year,
        int const month,
        int const day,
        int const hour,
        int const mins,
        int const sec
    ) :
        year(year),
        month(month),
        day(day),
        hour(hour),
        mins(mins),
        sec(sec) {}

    // POSSIBLE CONCURRENCY PROBLEM: std::localtime() is not thread safe
    // Default constructor that is set to the current date and time
    JulianDate() {
        auto const now = std::time(nullptr);
        auto const* now_date = std::localtime(&now);
        year = now_date->tm_year;
        month = now_date->tm_mon;
        day = now_date->tm_mday;
        hour = now_date->tm_hour;
        mins = now_date->tm_min;
        sec = now_date->tm_sec;
    }

    [[nodiscard]] auto calc_juliandate() const -> double {
        // all integer division is intentional!
        auto const A = (month - 14) / 12;
        auto const B = 1461 * (year + 4800 + A);
        auto const C = 367 * (month - 2 - 12 * A);
        auto const E = (year + 4900 + A) / 100;

        auto const jdn = B / 4 + C / 12 - 3 * E / 4 + day - 32075;

        // subtract 0.5 because julian date is zeroed at 12:00 noon
        auto const julian_hms = jdn + hour / 24.0 + mins / 1440.0 + sec / 86400.0 - 0.5;
        return julian_hms;
    }

    [[nodiscard]] auto diff_calc_juliandate(JulianDate const& other) const -> double {
        auto const this_jd = calc_juliandate();
        auto const other_jd = other.calc_juliandate();
        return this_jd - other_jd;
    }
};

// LINK ONE IS DEAD
// http://www.c-jump.com/bcc/c155c/Homework/a4_Serial_Julian_Date/a4_Serial_Julian_Date.html
// LINK TWO DOESN'T HAVE THE FORMULA
// https://www.aavso.org/jd-calculator

auto main() -> int {
    cout << "########" << endl;
    cout << "Main Problem" << endl;
    cout << "########" << endl;

    cout.precision(20);
    JulianDate const x(2007, 2, 8, 0, 0, 0);
    double const x1 = x.calc_juliandate();
    cout << "x1: " << x1 << endl;

    JulianDate const y(2000, 12, 31, 0, 0, 1);
    double const y1 = y.calc_juliandate();
    cout << "y1: " << y1 << endl;

    // manual calculation of the difference between two dates, e.g., x1 - y1 =
    cout << "x1 - y1 manual: " << x1 - y1 << endl;

    // Now use your class function to find the difference between two dates
    cout << "x1 - y1 class: " << x.diff_calc_juliandate(y) << endl;

    // example of date to Julian date format without hour/min/sec
    // 7/4/1776   2369916
    cout << "7/4/1776: " << JulianDate(1776, 7, 4, 0, 0, 0).calc_juliandate() << endl;
    // 12/31/2000 2451910
    cout << "12/31/2000: " << JulianDate(2000, 12, 31, 0, 0, 0).calc_juliandate() << endl;
    // 2/8/2007   2454140
    cout << "2/8/2007: " << JulianDate(2007, 2, 8, 0, 0, 0).calc_juliandate() << endl;
    // 2/9/2007   2454141
    cout << "2/9/2007: " << JulianDate(2007, 2, 9, 0, 0, 0).calc_juliandate() << endl;

    cout << "====[ end ]====" << endl;

    return 0;
}
