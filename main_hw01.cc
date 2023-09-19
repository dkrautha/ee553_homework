#include <iostream>

using std::cout;

double sumUpTo(int n) {
  double sum = 0.0;
  for (double i = 1; i <= n; i += 1) {
    sum += i;
  }
  return sum;
}

double sumReciprocalNormal(int n) {
  double sum = 0.0;
  for (double i = 1; i <= n; i += 1) {
    sum += 1.0 / i;
  }
  return sum;
}

double sumReciprocalRev(int n) {
  double sum = 0.0;
  for (double i = n; i >= 1; i -= 1) {
    sum += 1.0 / i;
  }
  return sum;
}

double sumMysteryNormal(int n) {
  double sum = 0.0;
  for (double i = 1; i <= n; i += 1) {
    sum += 1.0 / (i * i);
  }
  return sum;
}

double sumMysteryRev(int n) {
  double sum = 0.0;
  for (int i = n; i >= 1; i -= 1) {
    sum += 1.0 / (i * i);
  }
  return sum;
}

int main() {
  // Problem One
  cout << "sumUpTo: " << sumUpTo(100) << '\n';

  // Problem Two
  double sum1 = sumReciprocalNormal(100);
  double sum2 = sumReciprocalRev(100);
  cout << "sumReciprocalNormal: " << sum1 << '\n';
  cout << "sumReciprocalRev: " << sum2 << '\n';
  cout << "reciprocal difference: " << (sum1 - sum2) << '\n';

  // Problem Three
  double sum3 = sumMysteryNormal(100);
  double sum4 = sumMysteryRev(100);
  cout << "sumMysteryNormal: " << sum3 << '\n';
  cout << "sumMysteryRev: " << sum4 << '\n';
  cout << "mystery difference: " << (sum3 - sum4) << '\n';

  return 0;
}
