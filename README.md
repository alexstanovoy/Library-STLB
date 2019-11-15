![Library::STLB](assets/logo.svg)

## About ##

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://travis-ci.org/AjReme/Library-STLB.svg?branch=master)](https://travis-ci.org/AjReme/Library-STLB)
[![Codecov status](https://codecov.io/github/AjReme/Library-STLB/coverage.svg?branch=master)](https://codecov.io/gh/Ajreme/Library-STLB)
[![Documentation](https://img.shields.io/badge/Documentation-latest-blue.svg)](http://www.library-stlb.io)

***Library::STLB*** (*Standard Template Library B*) - the tiny library that contains implementations of powerful algorithms in **C++** language for different purposes: projects, as a small «*gear*» in big products, competitive programming solutions.
Also, all algorithms and classes designed for intuitive use just like in **STD C++** library.

Features:

- *All sources use only `.hpp` files so library doesn't require any changes in your compilation line arguments*.
- *Library was tested manually, generating random tests, in different competitive programming tasks and even in contests*.
- *Some additional functionality for existing code, new algorithms and data structures will be added later*.
- *Exceptions **is not** used by the library. Therefore if you see any function without `noexcept` - open an issue*.

## Documentation ##

Documentation of the project is available on [GitHub Pages](http://www.library-stlb.io).
***Note that not all sources are currently documented. If you can help - you're welcome!***

## Testing & Creating documentation ##

Just run `CMakeLists.txt` file in the main folder, but I recommend building the project out-of-source. On Linux:

- `mkdir build`
- `cd build`
- `cmake ../CMakeLists.txt -B ./`
- `make`
- `ctest`

Requirements:

- `GNU C++ compiler`
- `CMake`
- `Doxygen`
