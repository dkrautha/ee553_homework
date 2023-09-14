#include <cmath> // double sin(double), double cos(double)
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>

using namespace std;

// const for pi
constexpr auto pi = 3.14159265358979323846;

auto is_prime(int n) -> bool {
  for (auto i = 2; i < n; i += 1) {
    if (n % i == 0) {
      return false;
    }
  }
  return true;
}

int main() {
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
  const auto default_precision = cout.precision();
  while (file >> r >> theta_degrees) {
    cout << "r: " << r << " theta degrees: " << theta_degrees << '\n';
    const auto theta_radian = theta_degrees * (pi / 180.0);
    cout << "theta radians: " << theta_radian << '\n';
    const auto x = r * cos(theta_radian);
    const auto y = r * sin(theta_radian);
    // set precision just for x and y, then set it back afterwards as the
    // question doesn't specify that anything else should use the increased
    // precision
    cout << setprecision(8) << "x: " << x << " y: " << y << '\n';
    cout << setprecision(default_precision);
    const auto calculated_radians = atan2(y, x);
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
  if (temperature > 50) {
    can_run = false;
  }
  // check if all condition is met and use canRun to start calculation of wind
  // chill print out the value
  if (can_run) {
    cout << "Wind chill: "
         << 35.74 + 0.6215 * temperature - 35.75 * pow(wind_speed, 0.16) +
                0.4275 * temperature * pow(wind_speed, 0.16)
         << '\n';
  } else {
    cout << "Wind chill: N/A\n";
  }

  cout << "====[ end ]====" << endl;

  return 0;
}
