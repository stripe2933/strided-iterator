# strided-iterator
An random access iterator(until C++17)/contiguous iterator(since C++20) implementation with Modern C++ Style. It can be used with Modern C++ (since C++11).

# How to use
The library has two options: you can specify stride at compile time (using template parameter), or at runtime (using constructor).

```c++
#include <vector>
#include <iostream>
#include <strided_iterator.hpp>
#include <const_strided_iterator.hpp>

int main(){
    std::vector<int> nums { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // Specify stride at compile time:
    int i = -1;
    for (strided_iterator<int, 3> it = nums.begin(); it < nums.end(); ++it, --i){
        *it = i;
    }
    // nums: { -1, 2, 3, -4, 5, 6, -7, 8, 9 }

    // Specify stride at runtime:
    for (const_strided_iterator<int> it { nums.cbegin() + 1, 3 }; it < nums.cend(); ++it){
        std::cout << *it << ' ';
    }
    std::cout << std::endl; // output: 2 5 8

    return 0;
}
```

It has STL compatibility, so it can be used in variety of solutions.

```c++
std::vector<double> coords { // x1 y1 x2 y2 ...
    -1, 1, // (-1, 1)
    1, 1,  // (1, 1)
    1, -1  // (1, -1)
};
double leftmost_x = *std::min_element<strided_iterator<double, 2>>(coords.cbegin(), coords.cend()); // -1
double topmost_y = *std::max_element<strided_iterator<double, 2>>(coords.cbegin() + 1, coords.cend() + 1); // 1
```

Note: for the use of this in STL functions, it must be specified that **the distance the first (`coords.cbegin() + 1` in the above example) and the last (`coords.cend() + 1`) must be the mulitiple of the `Stride`** (2 in the above example), because STL iterates the iterator with != (not eq) condition, i.e.

```c++
for (auto it = first; it != last; ++it){ // <- compare with != operator
    // use it
}
```

## How to install

This is header-only library. Copy the files in `/include` folder to use. If you want to build test,

```cmake
mkdir build
cd build
cmake ..
make
```

and two files `/build/test/strided-iterator-test` and `/build/test/const-strided-iterator-test` will be generated (for macOS). The build was tested with Clang 13.0.1.

You can specify `-NDEBUG` flag to remove stride checking code and to speed up.

## Tests

This library was tested with [googletest](https://github.com/google/googletest). I appreciate you to support the test with more solutions.