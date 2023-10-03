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
  JulianDate(const int year, const int month, const int day, const int hour,
             const int mins, const int sec)
      : year(year), month(month), day(day), hour(hour), mins(mins), sec(sec) {}

  // POSSIBLE CONCURRENCY PROBLEM: std::localtime() is not thread safe
  // Default constructor that is set to the current date and time
  JulianDate() {
    const auto now = std::time(nullptr);
    const auto* now_date = std::localtime(&now);
    year = now_date->tm_year;
    month = now_date->tm_mon;
    day = now_date->tm_mday;
    hour = now_date->tm_hour;
    mins = now_date->tm_min;
    sec = now_date->tm_sec;
  };

  [[nodiscard]] auto calc_juliandate() const -> double {
    // all integer division is intentional!
    const auto A = (month - 14) / 12;
    const auto B = 1461 * (year + 4800 + A);
    const auto C = 367 * (month - 2 - 12 * A);
    const auto E = (year + 4900 + A) / 100;

    const auto jdn = B / 4 + C / 12 - 3 * E / 4 + day - 32075;

    // subtract 0.5 because julian date is zeroed at 12:00 noon
    const auto julian_hms =
        jdn + hour / 24.0 + mins / 1440.0 + sec / 86400.0 - 0.5;
    return julian_hms;
  }

  [[nodiscard]] auto diff_calc_juliandate(const JulianDate& other) const
      -> double {
    auto this_jd = calc_juliandate();
    auto other_jd = other.calc_juliandate();
    return this_jd - other_jd;
  };
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
  JulianDate x(2007, 2, 8, 0, 0, 0);
  double x1 = x.calc_juliandate();
  cout << "x1: " << x1 << endl;

  JulianDate y(2000, 12, 31, 0, 0, 1);
  double y1 = y.calc_juliandate();
  cout << "y1: " << y1 << endl;

  // manual calculation of the difference between two dates, e.g., x1 - y1 =
  cout << "x1 - y1 manual: " << x1 - y1 << endl;

  // Now use your class function to find the difference between two dates
  cout << "x1 - y1 class: " << x.diff_calc_juliandate(y) << endl;

  // example of date to Julian date format without hour/min/sec
  // 7/4/1776   2369916
  cout << "7/4/1776: " << JulianDate(1776, 7, 4, 0, 0, 0).calc_juliandate()
       << endl;
  // 12/31/2000 2451910
  cout << "12/31/2000: " << JulianDate(2000, 12, 31, 0, 0, 0).calc_juliandate()
       << endl;
  // 2/8/2007   2454140
  cout << "2/8/2007: " << JulianDate(2007, 2, 8, 0, 0, 0).calc_juliandate()
       << endl;
  // 2/9/2007   2454141
  cout << "2/9/2007: " << JulianDate(2007, 2, 9, 0, 0, 0).calc_juliandate()
       << endl;

  cout << "====[ end ]====" << endl;

  return 0;
}
