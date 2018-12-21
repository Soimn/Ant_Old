# Installation

The Ant game engine requires a number of libraries, all of which are to be shared.

Ant depends on the following libraries:
* Blaze
* GLFW
* GLEW
* EASTL

## Notes
Blaze requires a BLAS and LAPACK library. Intel mkl is highly encouraged and Ant is by deafult set up to use mkl. In order to use other BLAS and LAPACK libraries,
edit the defines preceeding the Blaze main header include in "src/core/common.h".
EASTL is by default built statically, append SHARED to the add_library command to change this.
```cmake
# Original
add_library(EASTL ${EASTL_SOURCES})

# Edited
add_library(EASTL SHARED ${EASTL_SOURCES})
```
