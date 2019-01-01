#pragma once

#include "core/common.h"

namespace {
	constexpr size_t npos = static_cast<size_t>(~0);
}

namespace {
	constexpr inline int __strlength_impl (const char* str, int count = 0) {
		for (;; ++count) {
			if (count >= (~(1 << (8 * sizeof(int) - 1))) - 1) {
				count = -1;
				break;
			}

			else if (str[count] == '\0') break;
		}

		return count;
	}

	constexpr size_t __find_impl(const char* str, const char target, size_t pos = npos) {
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

	constexpr size_t __find_last_of_impl(const char* str, const char target, size_t pos = npos) {
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

constexpr inline int strlength (const char* str) {
	return __strlength_impl(str);
}

constexpr size_t find(const char* str, const char target) {
	return __find_impl(str, target);
}


constexpr size_t find_first_of(const char* str, const char target) {
	return __find_impl(str, target);
}


constexpr size_t find_last_of(const char* str, const char target) {
	return __find_last_of_impl(str, target);
}
