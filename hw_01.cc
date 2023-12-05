#include <iostream>

using std::cout;

auto sumUpTo(int n) -> double {
  double sum = 0.0;
  for (auto i = 1; i <= n; i += 1) {
    sum += i;
  }
  return sum;
}

auto sumReciprocalNormal(int n) -> double {
  double sum = 0.0;
  for (auto i = 1; i <= n; i += 1) {
    sum += 1.0 / i;
  }
  return sum;
}

auto sumReciprocalRev(int n) -> double {
  double sum = 0.0;
  for (auto i = n; i >= 1; i -= 1) {
    sum += 1.0 / i;
  }
  return sum;
}

auto sumMysteryNormal(int n) -> double {
  double sum = 0.0;
  for (auto i = 1; i <= n; i += 1) {
    sum += 1.0 / (i * i);
  }
  return sum;
}

auto sumMysteryRev(int n) -> double {
  double sum = 0.0;
  for (int i = n; i >= 1; i -= 1) {
    sum += 1.0 / (i * i);
  }
  return sum;
}

auto main() -> int {
  constexpr auto SUM_TO = 100;
  // Problem One
  cout << "sumUpTo: " << sumUpTo(SUM_TO) << '\n';

  // Problem Two
  double sum1 = sumReciprocalNormal(SUM_TO);
  double sum2 = sumReciprocalRev(SUM_TO);
  cout << "sumReciprocalNormal: " << sum1 << '\n';
  cout << "sumReciprocalRev: " << sum2 << '\n';
  cout << "reciprocal difference: " << (sum1 - sum2) << '\n';

  // Problem Three
  double sum3 = sumMysteryNormal(SUM_TO);
  double sum4 = sumMysteryRev(SUM_TO);
  cout << "sumMysteryNormal: " << sum3 << '\n';
  cout << "sumMysteryRev: " << sum4 << '\n';
  cout << "mystery difference: " << (sum3 - sum4) << '\n';

  return 0;
}
