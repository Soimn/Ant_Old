#pragma once

#include "core/common.h"
#include "core/debug/assert.h"
#include "core/utility/utility.h"
#include "core/utility/cstring_utility.h"

namespace Ant {
namespace core {

struct StringArrayTag {};
struct StringRefTag {};

template<int N, typename Tag>
struct fixed_string {};

// ***************************************************
// | Fixed size string implemented with a char array |
// ***************************************************

template<int N>
struct fixed_string<N, StringArrayTag> {
	private:
		size_t capacity;
		char _data[static_cast<size_t>(N + 1)];

	public:
		constexpr fixed_string() : capacity(N) {
			static_assert(0 < N, "Illegal size fed to fixed_string");

		};

		explicit constexpr fixed_string(const char* str) {
			static_assert(0 < N, "Illegal size fed to fixed_string");
			
			int len = strlength(str);
			ASSERT(len != -1, "No null terminator found in string passed to constructor of fixed_string");
			
			strcpy(_data, str);
			_data[len] = '\0';
			capacity = static_cast<size_t>(N - len);
		}

		template<int N1>
		explicit constexpr fixed_string(const fixed_string<N1, StringRefTag>& str) {
			strcpy(_data, str.get_data());
		}

		template<int N1>
		constexpr fixed_string(const fixed_string<N1, StringRefTag>& s1, const fixed_string<N - N1, StringRefTag>& s2) {
			static_assert(0 < N, "Illegal size fed to fixed_string");
			
			for (int i = 0; i < N1; ++i)
				_data[i] = s1[i];

			for (unsigned int j = 0; j < N - N1; ++j)
				_data[N1 + j] = s2[j];

			_data[N] = '\0';
			capacity = N - strlength(&(_data[0]));
		}

		constexpr char operator [] (const int i) const {
			ASSERT(0 <= i && i <= N, "Index out of range");
			return _data[i];
		}

		constexpr char& operator [] (const int i) {
			ASSERT(0 <= i && i <= N, "Index out of range");
			return _data[i];
		}

		template<int N1>
		constexpr void append (const fixed_string<N1, StringArrayTag>& fstr) {
			size_t length = fstr.length();
			ASSERT(length <= capacity, "Cannot append string, reason: the string to be appended is larger than the main string's capacity");
			strcpy(&_data[N - capacity], fstr.get_data());
			capacity -= length;
		}

		constexpr void append (const char* str) {
			int length = strlength(str);
			ASSERT(static_cast<size_t>(length) <= capacity, "Cannot append string, reason: the string to be appended is larger than the main string's capacity");
			strcpy(&_data[N - capacity], str);
			capacity -= static_cast<size_t>(length);
		}

		constexpr const char* get_data() const { return _data; }
		constexpr size_t max_capacity () const { return static_cast<size_t>(N); }

		constexpr void update_capacity () {
			capacity = static_cast<size_t>(N - strlength(_data));
		}

		constexpr size_t length () const {
			return N - capacity;
		}
};



// *****************************************************************************
// | Fixed size non-owning string implemented with a reference to a char array |
// *****************************************************************************

template<int N>
struct fixed_string<N, StringRefTag> {
	private:
		const char (&_data)[static_cast<size_t>(N + 1)];

	public:
		constexpr fixed_string(const char (&lit) [static_cast<size_t>(N + 1)]) : _data(lit) {
			static_assert(0 < N, "Illegal size fed to fixed_string");
			ASSERT(lit[N] == '\0', "No null-terminator found");
		}

		constexpr char operator [] (const int i) const {
			ASSERT(0 <= i && i <= N, "Index out of range");
			return _data[i];
		}

		constexpr char& operator [] (const int i) = delete;

		constexpr const char* get_data() const { return _data; }
		constexpr size_t max_capacity () const { return static_cast<size_t>(N); }

		constexpr size_t length () const {
			for (char* it = _data; ; ++it) {
				if (*it == '\0') return it - _data;
			}
		}
};

template<int N>
using fixed_array_string= fixed_string<N, StringArrayTag>;

template<int N>
using fixed_string_view = fixed_string<N, StringRefTag>;



template<int N1, int N2, typename Tag1, typename Tag2>
constexpr auto operator + (const fixed_string<N1, Tag1>& s1, const fixed_string<N2, Tag2>& s2)
	-> fixed_array_string<N1 + N2> {
	
	return fixed_array_string<N1 + N2>(s1, s2);
}

#if ANT_COMPILER_GNU
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif

template<int N_P1>
fixed_string(const char (&lit)[N_P1]) -> fixed_string<N_P1 - 1, StringRefTag>;

#if ANT_COMPILER_GNU
#pragma GCC diagnostic warning "-Wsign-conversion"
#endif


// *********************
// | Utility functions |
// *********************

// *** find, find_*_off
constexpr size_t __find_impl(const char* str, const char target, int N, size_t pos = npos) {
	for (size_t i = 0; i < static_cast<size_t>(N); ++i) {
		if (str[i] == target) {
			pos = i;
			break;
		}
	}

	return pos;
}

template<int N, typename Tag>
constexpr size_t find(const fixed_string<N, Tag>& str, const char target) {
	return __find_impl(str.get_data(), target, N);
}

template<int N, typename Tag>
constexpr size_t find_first_of(const fixed_string<N, Tag>& str, const char target) {
	return __find_impl(str, target, N);
}

constexpr size_t __find_last_of_impl(const char* str, const char target, int N, size_t pos = npos) {
	for (signed int i = N - 1; i >= 0; --i) {
		if (str[i] == target) {
			pos = static_cast<size_t>(i);
			break;
		}
	}

	return pos;
}

template<int N, typename Tag>
constexpr size_t find_last_of(const fixed_string<N, Tag>& str, const char target) {
	return __find_last_of_impl(str.get_data(), target, N);
}
// ***

// *** reverse
template<int N>
constexpr void reverse (fixed_array_string<N>& fstr) {
	char* data = const_cast<char*>(fstr.get_data());
	int start = 0;
	int end = static_cast<int>(fstr.length()) - 1;

	while (start < end) {
		swap(*(data + start), *(data + end));
		++start;
		--end;
	}
}
// ***

// *** To string implementation
static constexpr fu16 MAX_TO_STRING_LENGTH = 200;

namespace {
	auto __uint_to_string (u64 i, u8 base) -> fixed_array_string<MAX_TO_STRING_LENGTH> {
		fixed_array_string<MAX_TO_STRING_LENGTH> fstr;
		int j = 0;
		
		if (i == 0) {
			fstr[++j] = '0';
			fstr[j] = '\0';
			return fstr;
		}

		u64 rem;
		while (i != 0) {
			rem = i % base;
			fstr[j++] = (rem > 9 ? (static_cast<char>((rem - 10) + 'a')) : static_cast<char>(rem + '0'));
			i /= base;
		}

		fstr[j] = '\0';
		fstr.update_capacity();
		reverse(fstr);
		return fstr;
	}

	auto __int_to_string (i64 i, u8 base) -> fixed_array_string<MAX_TO_STRING_LENGTH> {
		fixed_array_string<MAX_TO_STRING_LENGTH> fstr;

		if (i == 0) {
			fstr.append("0");
			fstr.append("\0");
			return fstr;
		}
		
		else if (i < 0 && base == 10) {
			i *= -1;
			fstr.append("-");
		}
		fstr.append(__uint_to_string(static_cast<u64>(i), base));
		return fstr;
	}
}

template<typename T>
auto __to_string_impl (const T& item, true_type) -> fixed_array_string<MAX_TO_STRING_LENGTH> {
	if (is_same<T, bool>::value)
		return fixed_array_string<MAX_TO_STRING_LENGTH>(static_cast<bool>(item) ? "true" : "false");
	else if constexpr (is_unsigned<T>::value)
		return __uint_to_string(static_cast<u64>(item), 10);
	else
		return __int_to_string(static_cast<i64>(item), 10);
}

template<typename T>
auto to_string (const T& item) -> fixed_array_string<MAX_TO_STRING_LENGTH> {
	return __to_string_impl(item, conditional_t<is_integral_v<T>, true_type, false_type>{});
}

template<int N, typename Tag>
auto to_string (const fixed_string<N, Tag>& item) -> fixed_array_string<N> {
	return fixed_array_string<N>(item);
}

auto to_string (const char* item) -> fixed_array_string<MAX_TO_STRING_LENGTH> {
	return fixed_array_string<MAX_TO_STRING_LENGTH>(item);;
}
// ***

struct string {
	
};

}
}
