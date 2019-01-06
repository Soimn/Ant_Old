#pragma once

// Standard Library Headers
#include <cstdlib>
#include <cstddef> // maybe unused
#include <cstdio>
#include <cstring> // maybe unused

#ifdef ANT_PLATFORM_UNIX
#include <unistd.h>
#include <cerrno>
#elif ANT_PLATFORM_WINDOWS
#include <windows.h>
#endif

// GLEW & GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// BLAZE
// #define BLAZE_BLAS_MODE 1
// #define BLAZE_BLAS_IS_PARALLEL 1
// #define BLAZE_USE_BLAS_MATRIX_VECTOR_MULTIPLICATION 1
// #define BLAZE_USE_BLAS_MATRIX_MATRIX_MULTIPLICATION 1
// #define BLAZE_BLAS_INCLUDE_FILE <mkl_cblas.h> // change <mkl_cblas.h> to BLAS header
// 
// #define BLAZE_THROW(EXCEPTION) [] { printf(#EXCEPTION), std::abort(); }()
// 
// #include <blaze/Blaze.h>

#include "dll_definitions.h"
#include "compiler_definitions.h"
#include "core/fixed_int.h"
