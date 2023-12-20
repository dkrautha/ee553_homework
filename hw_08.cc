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
#include <algorithm>  // for copy_n
#include <cstring>  // for strcat, strcpy, strlen, strncpy, size_t
#include <iostream>  // for operator<<, basic_ostream, char_traits, cout, endl
#include <utility>  // for swap, move

using usize = std::size_t;

class String {
    usize size_;
    char* data_;

  public:
    String() : size_(0), data_(nullptr) {}

    String(usize length, char const* str) : size_(length), data_(new char[size_ + 1]) {
        std::copy_n(str, length, data_);
    }

    // NOLINTNEXTLINE
    String(char const* str) : String(std::strlen(str) + 1, str) {}

    String(String const& other) : String(other.size_, other.data_) {}

    auto operator=(String const& other) -> String& {
        String tmp(other);
        swap(*this, tmp);
        return *this;
    }

    String(String&& other) noexcept : String() {
        swap(*this, other);
    }

    auto operator=(String&& other) noexcept -> String& {
        swap(*this, other);
        return *this;
    }

    ~String() {
        delete[] data_;
    }

    friend auto swap(String& a, String& b) noexcept -> void {
        using std::swap;
        swap(a.size_, b.size_);
        swap(a.data_, b.data_);
    }

    friend auto operator<<(std::ostream& os, String const& s) -> std::ostream& {
        return os << s.data_;
    }

    friend auto operator+(String const& a, String const& b) -> String {
        String result;
        result.size_ = a.size_ + b.size_;
        result.data_ = new char[result.size_ + 1];
        std::copy_n(a.data_, a.size_ - 1, result.data_);
        std::copy_n(b.data_, b.size_, result.data_ + a.size_ - 1);
        return result;
    }
};

using std::cout;

auto main() -> int {
    cout << "########\n";
    cout << "Main Problem\n";
    cout << "########\n";

    // Create String s = abc
    String s = "abc";
    // cout s
    cout << "s is: " << s << '\n';
    // Create String s = def
    String s2 = "def";
    // cout s2
    cout << "s2 is: " << s2 << '\n';
    // create String s3 equal s2
    String s3 = s2;  // copy constructor
    // cout s3
    cout << "s3 is: " << s3 << " copy constructor " << '\n';
    //    Create String s4 that is assigned to s added to s2
    String s4 = s + s2;  // abcdef // copy constructor
    // cout s4
    cout << "s4 is: " << s4 << " copy constructor of s + s2" << '\n';
    // Create s5 and move s4
    String s5 = std::move(s4);  // move constructor
    // cout s5
    cout << "s5 is: " << s5 << '\n';
    cout << "cout s4 should give you error after move()\n";
    cout << s4 << '\n';
    cout << "====[ end ]====\n";
    cout << "               \n";

    return 0;
}
