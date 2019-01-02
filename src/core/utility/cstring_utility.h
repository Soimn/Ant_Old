#pragma once

#include "core/common.h"

namespace Ant {
namespace core {

constexpr size_t npos = static_cast<size_t>(~0);

namespace {
	
	ANT_PURE
	constexpr inline int __strlength_impl (const char* str, int count = 0) {
		for (;; ++count) {
			
			// 1 << (8*sizeof(int) - 1) will create a int with the sign bit set,
			// the not operator is used to flip this.
			// Subtracting 1 yields the value: 2,147,483,646 for 32 bit integers
			if (count >= (~(1 << (8 * sizeof(int) - 1))) - 1) { 
				count = -1;
				break;
			}

			else if (str[count] == '\0') break;
		}

		return count;
	}

	ANT_PURE
	constexpr inline size_t __find_impl(const char* str, const char target, size_t pos = npos) {
		for (size_t i = 0; i < npos; ++i) {
			if (str[i] == target) {
				pos = i;
				break;
			}

			else if (str[i] == '\0')
				break;
		}

		return pos;
	}

	ANT_PURE
	constexpr inline size_t __find_last_of_impl(const char* str, const char target, size_t pos = npos) {
		for (size_t i = 0; i < npos; ++i) {
			if (str[i] == target) {
				pos = i;
			}

			else if (str[i] == '\0')
				break;
		}

		return pos;
	}
}

ANT_PURE
constexpr inline int strlength (const char* str) {
	return __strlength_impl(str);
}

ANT_PURE
constexpr size_t find(const char* str, const char target) {
	return __find_impl(str, target);
}

ANT_PURE
constexpr size_t find_first_of(const char* str, const char target) {
	return __find_impl(str, target);
}

ANT_PURE
constexpr size_t find_last_of(const char* str, const char target) {
	return __find_last_of_impl(str, target);
}

}
}
