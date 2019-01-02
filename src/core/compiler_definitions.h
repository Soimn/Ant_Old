#pragma once

// ***************
// |     GCC     |
// ***************
#ifdef ANT_COMPILER_GCC

#define ANT_PURE __attribute__((pure))



// ***************
// |   CLANG     |
// ***************
#elif ANT_COMPILER_CLANG

#define ANT_PURE __attribute__((pure))



// ***************
// |    MSVC     |
// ***************
#elif ANT_COMPILER_MSVC

#define ANT_PURE

#else
#error "Compiler not supported"

#endif
