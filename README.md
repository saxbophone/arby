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

### What will be provided in future?

- Additions to **`Nat`**:
  - bitwise operators
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

### Getting it

Use CPM

### Using it

Write C++ code

Docs: [saxbophone.com/arby/](saxbophone.com/arby/)
