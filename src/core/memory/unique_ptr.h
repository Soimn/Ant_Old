#pragma once
#include "core/common.h"
#include "core/utility/utility.h"
#include "core/debug.h"
#include "block.h"
#include "allocators.h"

namespace Ant {
namespace core {
namespace memory {

template<typename T, typename = _Require<__is_referenceable<T>>>
struct unique_ptr {
	private:
		Block m_block;

	public:
		using pointer = T*;
		using element_type = T;
		
		constexpr unique_ptr() : m_block({nullptr, 0, 0}) {}
		explicit unique_ptr(Block blk) : m_block(blk) {}

		unique_ptr(unique_ptr<T>&& other) : m_block(other.release()) {}
		
		~unique_ptr() {
			reset();
		}

		pointer get () const {
			return reinterpret_cast<pointer>(m_block.ptr);
		}

		Block release() {
			Block blk = this->m_block;
			this->m_block = nullblock_t;
			return blk;
		}

		void reset(Block blk = nullblock_t) {
			m_block.swap(blk);
			if (blk != nullblock_t) {
				ant_delete<T>(blk);
			}
		}

		void swap(unique_ptr& _uptr) {
			m_block.swap(_uptr.m_block);
		}
	
		
		unique_ptr& operator = (unique_ptr&& other) {
			reset(other.release());
			return *this;
		}

		unique_ptr& operator = (nullptr_t) {
			reset();
			return *this;
		}

		element_type& operator *() const {
			ASSERT(get() != pointer(), "get() called on empry unique_ptr");
			return *get();
		}

		pointer operator -> () const {
			ASSERT(get() != pointer(), "get() called on empry unique_ptr");
			return get();
		}

		unique_ptr(const unique_ptr& other) = delete;
		unique_ptr& operator = (const unique_ptr& other) = delete;
		unique_ptr& operator = (pointer ptr) = delete;
		unique_ptr& operator = (const Block& blk) = delete;
};

template<typename T>
inline void swap (unique_ptr<T>& __x, unique_ptr<T>& __y) {
	__x.swap(__y);
}

template<typename T, typename U>
inline bool operator == (const unique_ptr<T>& __x, const unique_ptr<U>& __y) {
	return __x.get() == __y.get();
}

template<typename T>
inline bool operator != (const unique_ptr<T>& __x, nullptr_t) {
	return __x.get() != nullptr;
}

template<typename T>
inline bool operator != (nullptr_t, const unique_ptr<T>& __x) {
	return __x.get() != nullptr;
}

template<typename T, typename U>
inline bool operator < (const unique_ptr<T>& __x, const unique_ptr<U>& __y) {
	return reinterpret_cast<byte*>(__x) < reinterpret_cast<byte*>(__y);
}

template<typename T>
inline bool operator < (const unique_ptr<T>& __x, nullptr_t) {
	return false;
}

template<typename T>
inline bool operator < (nullptr_t, const unique_ptr<T>& __x) {
	return __x != nullptr;
}

template<typename T>
inline bool operator <= (const unique_ptr<T>& __x, nullptr_t) {
	return !(nullptr < __x);
}

template<typename T>
inline bool operator <= (nullptr_t, const unique_ptr<T>& __x) {
	return !(__x < nullptr);
}

template<typename T, typename U>
inline bool operator > (const unique_ptr<T>& __x, const unique_ptr<U>& __y) {
	return (__y < __x);
}

template<typename T>
inline bool operator > (const unique_ptr<T>& __x, nullptr_t) {
	return nullptr < __x;
}

template<typename T>
inline bool operator > (nullptr_t, const unique_ptr<T>& __x) {
	return __x < nullptr;
}

template<typename T, typename U>
inline bool operator >= (const unique_ptr<T>& __x, const unique_ptr<T>& __y) {
	return !(__x < __y);
}

template<typename T>
inline bool operator >= (const unique_ptr<T>& __x, nullptr_t) {
	return !(__x < nullptr);
}

template<typename T>
inline bool operator >= (nullptr_t, const unique_ptr<T>& __x) {
	return !(nullptr < __x);
}

namespace {
	template<typename T>
	struct __MakeUniq { typedef unique_ptr<T> __single_object; };

	template<typename T>
	struct __MakeUniq<T[]> { typedef unique_ptr<T[]> __array; };

	template<typename T, size_t _Bound>
	struct __MakeUniq<T[_Bound]> { struct __invalid_type {}; };
}

template<typename T, typename ...Args>
inline typename __MakeUniq<T>::__single_object make_unique (Args&& ...args) {
	return unique_ptr<T>(ant_new<T>(forward<Args>(args)...));
}

template<typename T, typename ...Args>
inline typename __MakeUniq<T>::__array make_unique (size_t num) = delete;

template<typename T, typename ...Args>
inline typename __MakeUniq<T>::__invalid_type make_unique(Args&& ...) = delete;

}
}
}
