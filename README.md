Safe
====
Header only C++11 safe integer and utilities.

Provides:

-   `Safe::Integer<T>`, a class template which wraps an integer of any type, providing safe arithmetic, conversions, and comparisons
-   `Safe::Make<T>`, a function template which wraps an integer in a `Safe::Integer<T>`
-   `Safe::Cast<B,A>`, a function template which safely casts an integer of type `A` to type `B`
-   Convenience `typedef`s:
    -   `Safe::SizeType` and `Safe::size_t` which provide a safe wrapper for `std::size_t` (i.e. they are `typedef`'d to `Safe::Integer<std::size_t>`)
    -   `Safe::PointerDifferenceType`, `Safe::SignedSizeType`, `Safe::ptrdiff_t`, and `Safe::ssize_t` which provide a safe wrapper for `std::ptrdiff_t` (i.e. they are `typedef`'d to `Safe::Integer<std::ptrdiff_t>`)

Any unsafe (i.e. lossy) operation causes a `std::overflow_error` to be thrown.

Installation
------------
If you just want to use Safe and aren't concerned with building/running the unit tests, just [`#include <safe/safe.hpp>`](./include/safe/safe.hpp).

Dependencies
------------
-   C++11 standard library
-   [Catch](https://github.com/philsquared/Catch) (for unit tests only)

Compiler Support
----------------
-   Clang 3.4 (perhaps earlier, untested)
-   GCC 4.8.1 (definitely uses language features not supported in GCC 4.8)

MSVC++ untested.  I wouldn't hold your breath.

Examples
-------
Safe addition:
```C++
#include <safe/safe.hpp>
#include <iostream>
#include <limits>

int main (void) {

    auto i=std::numeric_limits<int>::max();
    i=Safe::Make(i)+1;  //  Throws std::overflow_error
    std::cout << i << std::endl;

}
```
Safe conversion:
```C++
#include <safe/safe.hpp>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

//  Poorly chosen parameter type
void print (std::uint8_t s) {

    std::cout << static_cast<std::uint16_t>(s) << std::endl;

}

int main (void) {

    std::vector<std::size_t> vec;
    
    std::size_t i=0;
    for (;i<100;++i) vec.push_back(i);
    
    print(Safe::Make(vec.size()));   //  Fine
    
    for (;i<1024;++i) vec.push_back(i);
    
    print(Safe::Make(vec.size()));   //  Throws std::overflow_error

}
```