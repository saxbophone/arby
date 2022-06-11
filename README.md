[![Stand With Ukraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/banner2-direct.svg)](https://stand-with-ukraine.pp.ua)
# arby

![release build](https://github.com/saxbophone/arby/actions/workflows/build-release.yml/badge.svg) ![continous integration](https://github.com/saxbophone/arby/actions/workflows/continuous-integration.yml/badge.svg)

Arbitrary precision arithmetic in C++, even at compile time

### What is arby?

arby is a C++ library that provides arbitrary-precision arithmetic integer types.

It is cross-platform, unit-tested and requires some C++20 support for use.

It's also licensed under a weak copyleft license, the [Mozilla Public License v2.0](LICENSE).

This allows arby to be used freely in open-source _and_ closed-source projects, with no restrictions. The only main condition related to use is that if you modify arby itself (i.e. any of the files in this repository), you are required to make your modified versions of these files publicly available, so that the community benefits from any potential improvements you make.

### What is arby _not_?

arby is not intended at this time to be a serious competitor to other well-established bignum libraries for C/C++ such as GNU's GMP/MPFR, certainly not in terms of portability or performance.

arby is designed with the following goals in mind:

- leverage C++ features for ease of use, such as operator overloading, custom literals, etc...
- provide compile-time computation of arbitrary-size integers when available on the platform (much of this being predicated on C++20 for full effect, compiler support for this is variable)
- usable with ease in new projects without having to worry about strict copyleft licensing concerns
- correctness and simplicity over speed, at least for now while the software's in alpha

### What's currently provided?

- Arbitrary-precision unsigned integers (the [Natural Numbers](https://en.wikipedia.org/wiki/Natural_number)) via class **`Nat`**
  - All basic mathematical operators overloaded
  - Divide and remainder (**`divmod()`**)
  - Raise to power (**`pow()`**)
  - All comparisons
  - cast to/from `uintmax_t` and `long double`
  - conversion to/from decimal, octal and hexadecimal string
  - bitwise operators

### What will be provided in future?

- Additions to **`Nat`**:
  - bit-shift operators
- Arbitrary-precision signed integers (the [Integers](https://en.wikipedia.org/wiki/Integer)) via class **`Int`**
  - All operations supported by **`Nat`** with the exception of bitwise ones
  - sign change
  - absolute value

### What _might_ be provided in future?

- Arbitrary-precision integer fractions (the [Rationals](https://en.wikipedia.org/wiki/Rational_number)) via class **`Rat`**
  - All operations supported by **`Int`**
  - simplify operation to reduce the size of numerator and denominator at the expense of precision

### Performance

arby hasn't been benchmarked yet, but it is not expected to perform as well as other long-established bignum libraries for C/C++.

Much of the code is not expected to perform terribly, however it must be noted that converting `Nat` to strings is particularly slow for very large numbers. This is an area for potential future optimisation efforts.

### Usability

arby is alpha-quality software. Although fairly thoroughly unit-tested, more work can be done to make it robust.

In particular, arby currently stores number digits in a custom-made doubly linked list type, due to lack of cross-platform stdlib support for `constexpr std::vector`. Nonetheless, this type doesn't work fully in `constexpr` contexts on all platforms.

Even with good-quality `constexpr` support, language limitations mean one can't store any of arby's types as compile-time constants, however it _is_ possible to store values of other types calculated from arby's types at compile-time, by casting to one of those types and returning such value from a `constexpr` or `consteval` function. This could be useful if calculation of a value that will fit in a fixed-size variable requires intermediate calculations of arbitrary size. Again, support for this requires good `constexpr` support on the platform to work.

### Getting it

Although mostly a header-only library, there are a few routines that are compiled into binary; it's recommended to use CMake for building and installing arby, and further recommended to use [CPM](https://github.com/cpm-cmake/CPM.cmake) to simplify package-management:

```cmake
CPMFindPackage(
    NAME Arby
    GIT_REPOSITORY git@github.com:saxbophone/arby.git
    GIT_TAG v0.3.0
)

# link your targets against Arby::arby
```

### Using it

Short demo program:

```c++
#include <iostream>

#include <arby/Nat.hpp>


using namespace com::saxbophone;

int main() {
    // powers of 11
    for (unsigned int power = 0; power < 20; power++) {
        std::cout << arby::pow(11, power) << std::endl;
    }
    std::cout << arby::pow(2, 64) << std::endl;
    // demo of custom literals
    using namespace com::saxbophone::arby::literals;
    arby::Nat foo = 1234567891011121314151617181920_nat * 77378921_nat;
    std::cout << foo << std::endl;
}
```

Sample output:

```
1
11
121
1331
14641
161051
1771561
19487171
214358881
2357947691
25937424601
285311670611
3138428376721
34522712143931
379749833583241
4177248169415651
45949729863572161
505447028499293771
5559917313492231481
61159090448414546291
18446744073709551616
95529531307686166289154167942030308320
```

### Documentation

Full API Docs available at: [saxbophone.com/arby/](https://saxbophone.com/arby/)
