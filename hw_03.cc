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
#include <random>  // for std::uniform_int_distribution

using std::cin;
using std::cout;
using std::endl;

auto read_elements(int* arr, int const arr_size) {
    for (auto i = 0; i < arr_size; i += 1) {
        cin >> arr[i];
    }
}

auto print_elements(int const* const arr, int const arr_size) {
    for (auto i = 0; i < arr_size; i += 1) {
        cout << arr[i] << " ";
    }
}

auto print_elements_rev(int const* const arr, int const arr_size) {
    for (auto i = arr_size - 1; i >= 0; i -= 1) {
        cout << arr[i] << " ";
    }
}

auto odds_only(int const* const arr, int const arr_size, int*& output_arr, int& output_size) {
    // you can't resize allocations with new[] very easily, this is a perfect use
    // case of a std::vector instead of trying to use a pointer and a length
    // I've taken the lazy way out by allocating e to be the same size as A, and
    // just lying about it's actual length to the caller
    // there's an additional problem here, if e is not nullptr, then whatever it
    // was pointing to will be leaked when we reassign it in the function
    output_arr = new int[arr_size];
    for (auto i = 0; i < arr_size; i += 1) {
        if (arr[i] % 2 == 1) {
            output_arr[output_size] = arr[i];
            output_size += 1;
        }
    }
}

auto swap_values(int* num1, int* num2) {
    // you should probably use std::swap instead
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

int main() {
    cout << "########" << endl;
    cout << "Problem One" << endl;
    cout << "########" << endl;
    // read how many number of elements you want to create
    cout << "Please enter number of elements: ";
    int n;
    cin >> n;

    // Use function ReadElements() to read array (A) from the user with given
    // number of elements define new empty array dynamically with n number as int
    // *A = new int[n]; write your code here
    int* A = new int[n];

    read_elements(A, n);

    // Use function PrintElements() that can print the entered array
    // write your code here
    cout << "Print Array" << endl;
    print_elements(A, n);
    cout << endl;

    // Use function PrintElementsRev() that can print the array in reverse order
    // write your code here
    cout << "Print Reverse Array" << endl;
    print_elements_rev(A, n);
    cout << endl;

    // Use function OddsOnly that can print only the odd elements of the user
    // array define a null pointer *e to use it with OddsOnly function and int m
    // equal zero write your code here you can use NULL here or nullptr for *e

    int* e = nullptr;  // null pointer
    int m = 0;  // number of odd elements

    // print odds array
    cout << "Print Odds Only Array" << endl;
    // Use function
    odds_only(A, n, e, m);
    print_elements(e, m);
    cout << endl;

    // do not forget
    delete[] A;  // Do not forget to deallocate dynamically allocated memory
    delete[] e;  // same as above

    cout << "====[ end ]====" << endl;
    cout << "               " << endl;

    cout << "########" << endl;
    cout << "Problem Two" << endl;
    cout << "########" << endl;
    // Read only this part of the problem

    // define the size of the array and the array
    constexpr int N = 10;  // Size of the array
    int arr[N];

    std::random_device random_device {};
    auto generator = std::mt19937(random_device());
    auto distribution = std::uniform_int_distribution<>(0, 99);
    // Initialize the array with random values (for demonstration)
    for (auto i = 0; i < N; i++) {
        // write code to generate random number between 0-99 and fill in the array
        arr[i] = distribution(generator);
    }

    // Display the original array
    cout << "Original Array: ";
    print_elements(arr, N);
    cout << endl;

    cout << "Enter a value to search for: ";
    int searchValue;
    cin >> searchValue;

    bool found = false;
    int index = -1;  // To store the index of the found value

    // Linear search algorithm
    // implement linear search algorithms
    for (auto i = 0; i < N; i += 1) {
        if (arr[i] == searchValue) {
            found = true;
            index = i;
            break;
        }
    }

    if (found) {
        cout << "The value " << searchValue << " is found at index " << index << endl;
    } else {
        cout << "The value " << searchValue << " is not found in the array." << endl;
    }

    cout << "########" << endl;
    cout << "Problem Three" << endl;
    cout << "########" << endl;

    auto num1 = 0;
    auto num2 = 0;

    cout << "Enter the first integer: ";
    cin >> num1;
    cout << "Enter the second integer: ";
    cin >> num2;

    cout << "Before swapping: num1 = " << num1 << ", num2 = " << num2 << endl;

    // Call the swapValues function to swap the values of num1 and num2 using
    // pointers
    // define swap value function before main function
    swap_values(&num1, &num2);

    // Display the values after swapping
    cout << "After swapping: num1 = " << num1 << ", num2 = " << num2 << endl;
}
