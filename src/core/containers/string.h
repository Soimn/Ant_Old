#pragma once

#include "core/common.h"
#include "core/debug/assert.h"
#include "core/memory.h"
#include "core/utility/utility.h"

namespace Ant {
namespace core {

struct StringArrayTag {};
struct StringRefTag {};

template<int N, typename Tag>
struct fixed_string {};

template<int N, typename Tag>
int strlength (const fixed_string<N, Tag>& fstr);
int strlength (const char* str);


// ***************************************************
// | Fixed size string implemented with a char array |
// ***************************************************

template<int N>
struct fixed_string<N, StringArrayTag> {
	private:
		unsigned int capacity;
		char _data[N + 1];

	public:
		static constexpr size_t npos = -1;

		constexpr fixed_string() = default;

		explicit constexpr fixed_string(const char* str) {
			static_assert(str[N] == '\0');
			strcpy(_data, str);
			_data[strlength(str)] = '\0';
			capacity = N - strlength(_data);
		}

		template<int N1>
		constexpr fixed_string(const fixed_string<N1, StringRefTag>& s1, const fixed_string<N - N1, StringRefTag>& s2) {
			for (unsigned int i = 0; i < N1; ++i)
				_data[i] = s1[i];

			for (unsigned int j = 0; j < N - N1; ++j)
				_data[N1 + j] = s2[j];

			_data[N] = '\0';
			capacity = N - strlength(_data);
		}

		constexpr char operator [] (const int i) const {
			ASSERT(0 <= i && i <= N, "Index out of range");
			return _data[i];
		}

		constexpr char& operator [] (const int i) {
			ASSERT(0 <= i && i <= N, "Index out of range");
			return _data[i];
		}

		template<int N1, typename Tag>
		constexpr void append (const fixed_string<N1, Tag>& fstr) {
			unsigned int length = strlength(fstr);
			ASSERT(length <= capacity, "Cannot append string, reason: the string to be appended is larger than the main string's capacity");
			strcpy(_data + (N - capacity), fstr.get_data());
			capacity -= length;
			_data[N - capacity] = '\0';
		}
		
		constexpr const char* get_data() const { return _data; }
		constexpr unsigned int max_capacity () const { return static_cast<unsigned int>(N); }
};



// *****************************************************************************
// | Fixed size non-owning string implemented with a reference to a char array |
// *****************************************************************************

template<int N>
struct fixed_string<N, StringRefTag> {
	private:
		const char (&_data)[N + 1];

	public:
		static constexpr size_t npos = -1;

		constexpr fixed_string(const char (&lit) [N + 1]) : _data(ASSERT(lit[N] == '\0', "No null-terminator found"), lit) {}

		constexpr char operator [] (const int i) const {
			ASSERT(0 <= i && i <= N, "Index out of range");
			return _data[i];
		}

		constexpr char& operator [] (const int i) = delete;

		constexpr const char* get_data() const { return _data; }
		constexpr unsigned int max_capacity () const { return static_cast<unsigned int>(N); }
};

template<int N>
using fixed_array_string= fixed_string<N, StringArrayTag>;

template<int N>
using fixed_string_view = fixed_string<N, StringRefTag>;



template<int N1, int N2, typename Tag1, typename Tag2>
constexpr auto operator + (const fixed_string<N1, Tag1>& s1, const fixed_string<N2, Tag2>& s2)
	-> fixed_array_string<N1 + N2> {
	
	return fixed_array_string<N1 + N2>(s1, s2);
};

template<int N_P1>
fixed_string(const char (&lit)[N_P1]) -> fixed_string<N_P1 - 1, StringRefTag>;


// *********************
// | Utility functions |
// *********************

// *** find, find_*_off
template<int N, typename Tag>
constexpr size_t find(const fixed_string<N, Tag>& str, const char target) {
	static_assert(0 < N);
	static_assert(str != "");

	size_t pos = fixed_string<N, Tag>::npos;

	for (unsigned int i = 0; i < N; ++i) {
		if (str[i] == target) {
			pos = i;
			break;
		}
	}

	return pos;
}

template<int N, typename Tag>
constexpr size_t find_first_of(const fixed_string<N, Tag>& str, const char target) {
	static_assert(0 < N);
	static_assert(str != "");

	return find(str, target);
}

template<int N, typename Tag>
constexpr size_t find_last_of(const fixed_string<N, Tag>& str, const char target) {
	static_assert(0 < N);
	static_assert(str != "");

	size_t pos = fixed_string<N, Tag>::npos;

	for (signed int i = N - 1; i >= 0; --i) {
		if (str[i] == target) {
			pos = i;
			break;
		}
	}

	return pos;
}
// ***

// *** reverse
template<int N, typename Tag>
constexpr auto reverse (const fixed_string<N, Tag>& fstr) -> fixed_array_string<N> {
	fixed_array_string<N> buffer;

	for (unsigned int i = 0; i < N; ++i) {
		buffer[i] = fstr[N - 1 - i];
	}

	return buffer;
}
// ***

// *** To string implementation
static constexpr fu16 MAX_TO_STRING_LENGTH = 200;

namespace {
	auto __uint_to_string (u64 i, u8 base) -> fixed_array_string<MAX_TO_STRING_LENGTH> {
		fixed_array_string<MAX_TO_STRING_LENGTH> fstr;
		unsigned int j = 0;
		
		if (i == 0) {
			fstr[0] = '0';
			fstr[1] = '\0';
			return fstr;
		}

		int rem;
		while (i != 0) {
			rem = i % base;
			fstr[j++] = (rem > 9 ? (static_cast<char>((rem - 10) + 'a')) : static_cast<char>(rem + '0'));
			i /= base;
		}

		fstr[i] = '\0';
		reverse(fstr);
		return fstr;
	}

	auto __int_to_string (i64 i, u8 base) -> fixed_array_string<MAX_TO_STRING_LENGTH> {
		fixed_array_string<MAX_TO_STRING_LENGTH> fstr;
		bool isNegative = false;

		if (i == 0) {
			fstr[0] = '0';
			fstr[1] = '\0';
			return fstr;
		}
		
		else if (i < 0 && base == 10) {
			isNegative = true;
			i *= -1;
		}

		fstr[0] = '-';
		fstr.append(__uint_to_string(static_cast<u64>(i), base));
		return fstr;
	}
}

template<typename T, typename = void>
auto to_string (const T& item) -> fixed_array_string<MAX_TO_STRING_LENGTH>;

template<typename T, typename = _Require<is_integral<T>>>
auto to_string (const T& item) -> fixed_array_string<MAX_TO_STRING_LENGTH> {
	if (is_same<T, bool>::value)
		return fixed_array_string<MAX_TO_STRING_LENGTH>(static_cast<bool>(item) ? "true" : "false");
	else if constexpr (is_unsigned<T>::value)
		return __uint_to_string(static_cast<u64>(item), 10);
	else
		return __int_to_string(static_cast<i64>(item), 10);
}
// ***


// *** strlen implementation
template<int N, typename Tag>
int strlength (const fixed_string<N, Tag>& str) {
	return strlength(str.get_data());
}

int strlength (const char* str) {
	unsigned int count = 0;
	size_t cutoff = -1;

	for (;; ++count) {
		if (count >= cutoff) return -1;
		else if (str[count] == '\0') break;
	}

	return count;
}
// ***


struct string {
	
};

}
}
