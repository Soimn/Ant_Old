# Installation

The Ant game engine requires a number of libraries, all of which are to be shared.

Ant depends on the following libraries:
* Blaze
* GLFW
* GLEW

## Notes
Blaze requires a BLAS and LAPACK library. Intel mkl is highly encouraged and Ant is by deafult set up to use mkl. In order to use other BLAS and LAPACK libraries,
edit the defines preceeding the Blaze main header include in "src/core/common.h".
