#pragma once
#include "core/common.h"
#include "core/utility/utility.h"
#include "block.h"
#include "allocators.h"

namespace Ant {
namespace core {
namespace memory {

namespace {
template<typename T>
struct unique_ptr_impl {
	template<typename U, typename = void>
	struct _Ptr { using type = U*; };

	template<typename U>
	struct _Ptr<U, void_t<remove_reference_t<T>*>> {
		using type = remove_reference_t<T>*;
	};

	public:
		using pointer = typename _Ptr<T>::type;

		unique_ptr_impl() = default;
		unique_ptr_impl(Block blk) : m_t(blk) {}

	private:
		Block m_t;
};
}

template<typename T, typename = _Require<__not_<is_array<T>>>>
struct unique_ptr {
	private:
		unique_ptr_impl<T> m_impl;

	public:
		using pointer = typename unique_ptr_impl<T>::pointer;
		using element_type = T;

	private:
		template<typename U>
		using __safe_conversion_u = __and_<is_convertible<typename unique_ptr_impl<U>::pointer, pointer>, __not_<is_array<U>>>;

	public:
		constexpr unique_ptr() : m_impl() {}
		explicit unique_ptr(Block blk) : m_impl(blk) {}
		constexpr unique_ptr(nullptr_t) : m_impl() {}

		unique_ptr(unique_ptr<T>&& other) : m_impl(other.release()) {}
		template<typename U, typename = _Require<__safe_conversion_u<U>>>
		unique_ptr(unique_ptr<U>&& other) : m_impl(other.release()) {}

		~unique_ptr() {
			reset();
		}

		pointer get () const {
			return reinterpret_cast<pointer>(m_impl.m_t.ptr);
		}

		Block release() {
			Block blk = this->m_impl.m_t;
			this->m_impl.m_t = nullblock_t;
			return blk;
		}

		void reset(Block blk = nullblock_t) {
			swap(m_impl.m_t, blk);
			if (!blk)
				deallocate(blk);
		}

		void swap(unique_ptr& _uptr) {
			swap(m_impl, _uptr.m_impl);
		}
	
		
		unique_ptr& operator = (unique_ptr&& other) {
			reset(other.release());
			return *this;
		}

		template<typename U>
		typename enable_if<__safe_conversion_u<U>::value, unique_ptr&>::type
		operator = (unique_ptr<U>&& other) {
			reset(other.release());
			return *this;
		}

		unique_ptr& operator = (nullptr_t) {
			reset();
			return *this;
		}

		typename add_lvalue_reference<element_type>::type
		operator *() const {
			ASSERT(get() != pointer, "Get() called on empry unique_ptr");
			return *get();
		}

		pointer operator -> () const {
			ASSERT(get() != pointer, "Get() called on empry unique_ptr");
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
