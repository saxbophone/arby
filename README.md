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

### What will be provided in future?

### Getting it

### Using it

Docs: [saxbophone.com/arby/](saxbophone.com/arby/)
