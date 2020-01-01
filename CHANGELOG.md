# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- Linear suffix array construction (`suffix_array`) [by Juha Karkkainen and Peter Sanders](https://www.cs.helsinki.fi/u/tpkarkka/publications/icalp03.pdf).
- Linear LCP array construction (`lcp_array`) [by Toru Kasai, Gunho Lee, Hiroki Arimura, Setsuo Arikawa, and Kunsoo Park](http://web.cs.iastate.edu/~cs548/references/linear_lcp.pdf).
- Improved perfomance Fast Fourier transform (`fast_fourier_transform`).

### Changed
- `radix_sort` signature.

### Removed
- `radix_nth_element`. Will be rewritten soon.

## [0.0.3] - 2019-12-31
### Added
- Draft Earley parser implementation.

## [0.0.2] - 2019-11-15
### Added
- Prefix function (`prefix_function`).
- Z-function (`z_function`).
- Manacher algorithm (`manacher`).
- Shunting yard algorithm (`infix_to_prefix`).
- Draft `bitset` documentation.

### Changed
- All present algorithms were split by namespaces.
- `radix_sort` and `radix_nth_element` signature.

### Deprecated
- `bitvec`.

## [0.0.1] - 2019-11-2
- Initial Release.