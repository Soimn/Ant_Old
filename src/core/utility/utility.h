#pragma once

#include "core/common.h"
#include "core/utility/type_traits.h"

namespace Ant {
namespace core {
namespace memory {

// Memory align

[[gnu::pure]]
inline byte* align (void* ptr, byte alignment) {
	return reinterpret_cast<byte*>(ptr) + static_cast<byte>(((~reinterpret_cast<uintptr>(ptr)) + 1) & static_cast<byte>(alignment - 1));
}

inline void* align_std (byte alignment, size_t size, void*& ptr, size_t& space) {
	void* result = nullptr;

	if (space >= size) {
		byte* aligned = align(ptr, alignment);
		byte offset = static_cast<byte>(aligned - reinterpret_cast<byte*>(ptr));

		if ((space - size) >= offset) {
			ptr = aligned;
			space -= offset;
			result = aligned;
		}
	}

	return result;
}

//

// Forward

template<typename T>
constexpr T&& forward (typename remove_reference<T>::type& _t) { return static_cast<T&&>(_t); }

template<typename T>
constexpr T&& forward (typename remove_reference<T>::type&& _t) {
	static_assert(!is_lvalue_reference<T>::value, "template argument substituting T is an l-value reference");
	return static_cast<T&&>(_t);
}

template<typename T>
constexpr typename remove_reference<T>::type&&
move (T&& _t) {
	return static_cast<typename remove_reference<T>::type&&>(_t);	
}

template<typename T>
inline typename enable_if<__and_</*__not_<__is_tuple_like<T>>,*/ is_move_constructible<T>, is_move_assignable<T>>::value>::type // NOTE(Soimn): FORWARD DECLARATION IN TYPE_TRAITS.H
swap (T __a, T __b) {
	T tmp = move(__a);
	__a = move(__b);
	__b = move(tmp);
}

template<typename T, size_t N>
inline typename enable_if<is_swappable<T>::value>::type
swap (T (&__a)[N], T (&__b)[N]) {
	for (size_t __n = 0; __n < N; ++__n) {
		swap(__a[__n], __b[__n]);
	}
}

//

}
}
}
