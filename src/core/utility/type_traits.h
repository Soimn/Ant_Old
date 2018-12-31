#pragma once

#include "core/common.h"

namespace Ant {
namespace core {

// *** forward declarations

template<typename T>
struct is_void;

template<typename T>
struct is_object;

template<typename T>
struct __is_referenceable;

template<typename T>
struct add_rvalue_reference;

template<typename T>
struct is_array;

template<typename>
struct is_function;

template<typename T>
struct remove_cv;

template<typename T>
struct remove_reference;

template<typename T>
struct remove_extent;

template<typename T>
struct remove_pointer;

template<typename T>
struct add_pointer;

template<typename T>
struct __is_swappable;
// ***

// *** Types

template<typename T, T v>
struct integral_constant {
	static constexpr T value = v;
	typedef T value_type;
	typedef integral_constant<T, v> type;
	constexpr T operator () () const { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template<bool v>
using bool_constant = integral_constant<bool, v>;

// Success & failure
template<typename T>
struct __success_type { typedef T type; };

template<typename T>
struct __failure_type {};

// ***

// *** Helpers

template<typename...>
using void_t = void;

// Conditional
template<bool Cond, typename true_t, typename false_t>
struct conditional { typedef true_t type; };

template<typename true_t, typename false_t>
struct conditional<false, true_t, false_t> { typedef false_t type; };

template<bool Cond, typename true_t, typename false_t>
using conditional_t = typename conditional<Cond, true_t, false_t>::type;

// SFINAE enable_if
template<bool Cond, typename T = void>
struct enable_if {};

template<typename T>
struct enable_if<true, T> { typedef T type; };

template<bool Cond, typename T = void>
using enable_if_t = typename enable_if<Cond, T>::type;

// Logical or
template<typename...>
struct __or_;

template<>
struct __or_<> : public true_type {};

template<typename T>
struct __or_<T> : public T {};

template<typename T1, typename T2>
struct __or_<T1, T2> : public conditional<T1::value, T1, T2>::type {};

template<typename T1, typename T2, typename T3, typename ...Ts>
struct __or_<T1, T2, T3, Ts...> : public conditional<T1::value, T1, __or_<T2, T3, Ts...>>::type {};

// Logical and
template<typename...>
struct __and_;

template<>
struct __and_<> : public true_type {};

template<typename T>
struct __and_<T> : public T {};

template<typename T1, typename T2>
struct __and_<T1, T2> : public conditional<T1::value, T2, T1>::type {};

template<typename T1, typename T2, typename T3, typename ...Ts>
struct __and_<T1, T2, T3, Ts...> : public conditional<T1::value, __and_<T2, T3, Ts...>, T1>::type {};

// Logical not
template<typename T> struct __not_ : public bool_constant<!bool(T::value)> {};

template<typename ...Cond>
using _Require = enable_if_t<__and_<Cond...>::value>; 

template<typename T>
typename add_rvalue_reference<T>::type declval();

// decay
template<typename U, bool _IsArray = is_array<U>::value, bool _IsFunction = is_function<U>::value>
struct __decay_selector;

template<typename U>
struct __decay_selector<U, false, false> { typedef typename remove_cv<U>::type __type; };

template<typename U>
struct __decay_selector<U, true, false> { typedef typename remove_extent<U>::type* __type; };

template<typename U>
struct __decay_selector<U, false, true> { typedef typename add_pointer<U>::type __type; };

template<typename T>
struct decay {
	private:
		typedef typename remove_reference<T>::type __remove_type;
	
	public:
		typedef typename __decay_selector<__remove_type>::__type type;
};
// ***

// *** add & remove *

// Remove const & volatile
template<typename T>
struct remove_const { typedef T type; };

template<typename T>
struct remove_const<const T> { typedef T type; };

template<typename T>
struct remove_volatile { typedef T type; };

template<typename T>
struct remove_volatile<volatile T> { typedef T type; };

template<typename T>
struct remove_cv {
	typedef typename remove_const<typename remove_volatile<T>::type>::type type;
};

template<typename T>
using remove_cv_t = typename remove_cv<T>::type;

// add & remove references
template<typename T>
struct remove_reference { typedef T type; };

template<typename T>
struct remove_reference<T&> { typedef T type; };

template<typename T>
struct remove_reference<T&&> { typedef T type; };

template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

template<typename T, bool = __is_referenceable<T>::value>
struct __add_rvalue_reference_impl { typedef T type; };

template<typename T>
struct __add_rvalue_reference_impl<T, true> { typedef T&& type; };

template<typename T> struct add_rvalue_reference : public __add_rvalue_reference_impl<T> {};

template<typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

template<typename T, bool = __is_referenceable<T>::value>
struct __add_lvalue_reference_impl { typedef T type; };

template<typename T>
struct __add_lvalue_reference_impl<T, true> { typedef T& type; };

template<typename T> struct add_lvalue_reference : public __add_rvalue_reference_impl<T> {};

template<typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

// remove_extent
template<typename T>
struct remove_extent { typedef T type; };

template<typename T, size_t _Size>
struct remove_extent<T[_Size]> { typedef T type; };

template<typename T>
struct remove_extent<T[]> { typedef T type; };

template<typename T>
using remove_extent_t = typename remove_extent<T>::type;

// add & remove pointer
template<typename T, typename>
struct __remove_pointer_impl { typedef T type; };

template<typename T, typename U>
struct __remove_pointer_impl<T, U*> { typedef U type; };

template<typename T>
struct remove_pointer : public __remove_pointer_impl<T, remove_cv_t<T>> {};

template<typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

template<typename T, bool = __or_<__is_referenceable<T>, is_void<T>>::value>
struct __add_pointer_helper { typedef T type; };

template<typename T>
struct __add_pointer_helper<T, true> { typedef typename remove_reference<T>::type* type; };

template<typename T>
struct add_pointer : public __add_pointer_helper<T> {};

template<typename T>
using add_pointer_t = typename add_pointer<T>::type;

// ***

// *** Is *

// is_*reference
template<typename T>
struct is_lvalue_reference : public false_type {};

template<typename T>
struct is_lvalue_reference<T&> : public true_type {};

template<typename T>
struct is_rvalue_reference : public false_type {};

template<typename T>
struct is_rvalue_reference<T&&> : public true_type {};

template<typename T>
struct is_reference : public __or_<is_lvalue_reference<T>, is_rvalue_reference<T>>::type {};

// __is_referenceable
template<typename T>
struct __is_referenceable : public __or_<is_object<T>, is_reference<T>>::type {};

template<typename _Res, typename ...Args>
struct __is_referenceable<_Res(Args...)> : public true_type {};

template<typename _Res, typename ...Args>
struct __is_referenceable<_Res(Args......)> : public true_type {};

// is_array
template<typename T>
struct is_array : public false_type {};

template<typename T>
struct is_array<T[]> : public true_type {};

template<typename T, size_t size>
struct is_array<T[size]> : public true_type {};

// is_pointer
template<typename T>
struct __is_ptr_impl : public false_type {};

template<typename T>
struct __is_ptr_impl<T*> : public true_type {};

template<typename T>
struct is_pointer : public __is_ptr_impl<typename remove_cv<T>::type>::type {};

// is_void
template<typename>
struct __is_void_impl : public false_type {};

template<>
struct __is_void_impl<void> : public true_type {};

template<typename T>
struct is_void : public __is_void_impl<remove_cv_t<T>>::type {};

// is_same
template<typename, typename>
struct is_same : public false_type {};

template<typename T>
struct is_same<T, T> : public true_type {};

// is_function
template<typename>
struct is_function : false_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...)> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) &> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) &&> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......)> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) &> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) &&> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) const> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) const &> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) const &&> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) const> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) const &> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) const &&> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) volatile> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) volatile &> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) volatile &&> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) volatile> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) volatile &> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) volatile &&> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) const volatile> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) const volatile &> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args...) const volatile &&> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) const volatile> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) const volatile &> : true_type {};

template<typename _Res, typename ...Args>
struct is_function<_Res(Args......) const volatile &&> : true_type {};

// is_object
template<typename T>
struct is_object : public __not_<__or_<is_function<T>, is_reference<T>, is_void<T>>>::type {};

// is_*constructible
template<class, class T, typename ...Args>
struct is_constructible_ : false_type {};

template<class T, typename ...Args>
struct is_constructible_<void_t<decltype(T(declval<Args>()...))>, T, Args...> : public true_type {};

template<class T, typename ...Args>
using is_constructible = is_constructible_<void_t<>, T, Args...>;

template<typename T, bool = __is_referenceable<T>::value>
struct __is_move_constructible_impl;

template<typename T>
struct __is_move_constructible_impl<T, false> : public false_type {};

template<typename T>
struct __is_move_constructible_impl<T, true> : public is_constructible<T, T&&> {};

template<typename T>
struct is_move_constructible : public __is_move_constructible_impl<T> {};

// is_*assignable
template<typename T, typename U, typename = void>
struct is_assignable : public false_type {};

template<typename T, typename U>
struct is_assignable<T, U, decltype(declval<T>() = declval<U>(), void())> : true_type {};

template<typename T, bool = __is_referenceable<T>::value>
struct __is_move_assignable_impl;

template<typename T>
struct __is_move_assignable_impl<T, false> : public false_type {};

template<typename T>
struct __is_move_assignable_impl<T, true> : public is_assignable<T&, T&&> {};

template<typename T>
struct is_move_assignable : public __is_move_assignable_impl<T> {};

// is_swappable
template<typename T>
struct is_swappable;

template<typename T>
inline typename enable_if<__and_</*__not_<__is_tuple_like<T>>,*/ is_move_constructible<T>, is_move_assignable<T>>::value>::type
swap (T& __a, T& __b);

template<typename T, size_t N>
inline typename enable_if<__is_swappable<T>::value>::type
swap (T (&__a)[N], T (&__b)[N]);

namespace __is_swappable_detail {
	struct __do_is_swappable_impl {
		template<typename T, typename = decltype(swap(declval<T&>(), declval<T&>()))>
		static true_type __test(int);

		template<typename>
		static false_type __test(...);
	};
}

template<typename T>
struct __is_swappable_impl : public __is_swappable_detail::__do_is_swappable_impl { typedef decltype(__test<T>(0)) type; };

template<typename T>
struct __is_swappable : public __is_swappable_impl<T>::type {};

template<typename T>
struct is_swappable : public __is_swappable_impl<T>::type {};

// is_convertible
template<typename From, typename To, bool = __or_<is_void<From>, is_function<To>, is_array<To>>::value>
struct __is_convertible_impl { typedef typename is_void<To>::type type; };

template<typename From, typename To>
struct __is_convertible_impl<From, To, false> {
	private:
		template<typename To1>
		static void __test_aux(To1);

		template<typename From1, typename To1, typename = decltype(__test_aux<To1>(declval<From1>()))>
		static true_type __test(int);

		template<typename, typename>
		static false_type __test(...);
	
	public:
		typedef decltype(__test<From, To>(0)) type;
};

template<typename From, typename To>
struct is_convertible : public __is_convertible_impl<From, To>::type {};

// is_integral & is_*signed

template<typename T> struct __is_integral_helper;
template<typename T> struct __is_integral_helper           : public false_type {};
template<> struct __is_integral_helper<bool>               : public true_type  {};
template<> struct __is_integral_helper<wchar_t>            : public true_type  {};
template<> struct __is_integral_helper<char16_t>           : public true_type  {};
template<> struct __is_integral_helper<char32_t>           : public true_type  {};
template<> struct __is_integral_helper<i8>                 : public true_type  {};
template<> struct __is_integral_helper<u8>                 : public true_type  {};
template<> struct __is_integral_helper<i16>                : public true_type  {};
template<> struct __is_integral_helper<u16>                : public true_type  {};
template<> struct __is_integral_helper<i32>                : public true_type  {};
template<> struct __is_integral_helper<u32>                : public true_type  {};
template<> struct __is_integral_helper<i64>                : public true_type  {};
template<> struct __is_integral_helper<u64>                : public true_type  {};

template<typename T>
struct is_integral : __is_integral_helper<typename remove_cv<T>::type>::type {};

template<typename T>
inline constexpr bool is_integral_v = is_integral<T>::value;

template<typename T> struct __is_unsigned_helper;
template<typename T> struct __is_unsigned_helper           : public false_type {};
template<> struct __is_unsigned_helper<u8>                 : public true_type  {};
template<> struct __is_unsigned_helper<u16>                : public true_type  {};
template<> struct __is_unsigned_helper<u32>                : public true_type  {};
template<> struct __is_unsigned_helper<u64>                : public true_type  {};

template<typename T>
struct is_unsigned : __is_unsigned_helper<typename remove_cv<T>::type>::type {};

template<typename T>
struct is_signed : __not_<is_unsigned<T>> {};

// ***

}
}
