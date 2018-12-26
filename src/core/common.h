#pragma once

#include "dll_definitions.h"

// Standard Library Headers
#include <cstdlib>
#include <cstddef>
#include <cmath>

// GLEW & GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// EASTL
// #include <EASTL/algorithm.h>
// #include <EASTL/sort.h>
// 
// #include <EASTL/vector.h>
// #include <EASTL/bitvector.h>
// #include <EASTL/fixed_vector.h>
// #include <EASTL/array.h>
// #include <EASTL/tuple.h>
// #include <EASTL/hash_map.h>
// #include <EASTL/fixed_hash_map.h>
// #include <EASTL/map.h>
// #include <EASTL/fixed_map.h>
// #include <EASTL/vector_map.h>
// #include <EASTL/unordered_map.h>
// 
// #include <EASTL/string.h>
// #include <EASTL/fixed_string.h>
// 
// #include <EASTL/chrono.h>
// #include <EASTL/numeric.h>
// #include <EASTL/numeric_limits.h>
// #include <EASTL/optional.h>
// #include <EASTL/type_traits.h>
// #include <EASTL/utility.h>
// #include <EASTL/variant.h>

// TEMPORARY(Soimn): temporary implementation of operator new[] required by EASTL
// [[maybe_unused]]
// void* operator new[] (size_t size, [[maybe_unused]] const char* name, [[maybe_unused]] int flags, [[maybe_unused]] unsigned int debugFlags, [[maybe_unused]] const char* file, [[maybe_unused]] int line) {
// 	return malloc(size);
// }

// BLAZE
#define BLAZE_BLAS_MODE 1
#define BLAZE_BLAS_IS_PARALLEL 1
#define BLAZE_USE_BLAS_MATRIX_VECTOR_MULTIPLICATION 1
#define BLAZE_USE_BLAS_MATRIX_MATRIX_MULTIPLICATION 1
#define BLAZE_BLAS_INCLUDE_FILE <mkl_cblas.h> // change <mkl_cblas.h> to BLAS header

#define BLAZE_THROW(EXCEPTION)\
	[] {\
	printf(#EXCEPTION);\
	std::abort();\
	}()

#include <blaze/Blaze.h>


#include "core/fixed_int.h"
