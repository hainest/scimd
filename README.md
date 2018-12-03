# scimd: A simple, science-oriented SIMD library 

In scientific software, we are more concerned about floating-point performance. Hence,
scimd only supports floating-point arithmetic. This keeps the library very small while
still providing opportunities for speedups in important computations.

scimd supports the following vector instruction sets: 

* Scalar
* SSE4.2
* AVX2
* AVX-512

Work is ongoing to support

* Altivec
* PTX (CUDA's assembly language)
* OpenCL (TBD)
* NEON

## Examples

TODO

## Build Requirements

C++11 Compiler:

* GCC >= 4.8.1
* clang >= 3.7
* ICC (TBD)
* Cray compiler (TBD)
* IBM XL (TBD)

NOTE: AVX-512 requires gcc >= 5 or clang >= 3.9

## Building

scimd is header-only. Just drop it into your project or include it as a git submodule!

## Testing

TODO

## Documentation

TODO

## License

This project is licensed under the GPL3 License - see the [LICENSE.md](LICENSE.md) file for details