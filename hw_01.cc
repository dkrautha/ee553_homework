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
