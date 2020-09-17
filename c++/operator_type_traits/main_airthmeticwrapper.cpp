#include <type_traits>
using std::enable_if_t, std::is_default_constructible_v;
#include <utility>
#include <vector>
using std::vector;
#include "amgg/type_traits/operator.hpp"
#include "amgg/type_traits/misc.hpp"


struct A{
	A& operator+=(const A& other) {
		return *this;
	}
	A& operator=(const A& other) = delete;
	A operator+(const A& other) const = delete;
};

struct B{
	B& operator+=(const B& other) = delete;
	B& operator=(const B& other) = default;
	B operator+(const B& other) const {
		B ret;
		return ret;
	}
};

struct C{
	C& operator+=(const C& other) = delete;
	C& operator=(const C& other) = delete;
	C operator+(const C& other) const = delete;
};

struct D{
	D& operator+=(const D& other) = delete;
	D& operator=(const D& other) = default;
	D operator+(const D& other) const = delete;
};

struct E{
	E& operator+=(const E& other) = delete;
	E& operator=(const E& other) = delete;
	E operator+(const E& other) const{
		E ret;
		return ret;
	};
};

constexpr bool not(bool b){return !b;}

template<typename T>
struct ArithmeticWrapper{
	T v;
	template<typename = enable_if_t<is_default_constructible_v<T>>>
	ArithmeticWrapper(): v() {};


	// template<typename T2, bool_if_t<has_op_addition_assignment_v<T&, const T2>> = true>
	// ArithmeticWrapper<T>& operator+=(const T2& other){
	// 	this->v += other;
	// 	return *this;
	// }
	// template<typename T2, bool_if_t<!has_op_addition_assignment_v<T&, const T2> && has_op_assignment_v<T, op_addition_returntype<const T, const T2>>> = true>
	// ArithmeticWrapper<T>& operator+=(const T2& other){
	// 	this->v = this->v + other;
	// 	return *this;
	// }
	
	#define ArithmeticWrapper_COMPOUND_ASSIGNMENT(op_name, op) \
	template<typename T2, amgg::bool_if_t<amgg::has_op_ ## op_name ## _assignment_v<T&, const T2&>> = true> \
	ArithmeticWrapper<T>& operator ## op ## = (const T2& other){ \
		this->v op ## = other; \
		return *this; \
	} \
	template<typename T2, amgg::bool_if_t<!amgg::has_op_ ## op_name ## _assignment_v<T&, const T2> && amgg::has_op_assignment_v<T, amgg::op_ ## op_name ## _returntype<const T, const T2>>> = true> \
	ArithmeticWrapper<T>& operator ## op ## = (const T2& other){ \
		this->v = this->v op other; \
		return *this; \
	}

	ArithmeticWrapper_COMPOUND_ASSIGNMENT(addition, +)
	ArithmeticWrapper_COMPOUND_ASSIGNMENT(subtraction, -)
	ArithmeticWrapper_COMPOUND_ASSIGNMENT(multiplication, *)
	ArithmeticWrapper_COMPOUND_ASSIGNMENT(division, /)
	ArithmeticWrapper_COMPOUND_ASSIGNMENT(modulo, %)
	ArithmeticWrapper_COMPOUND_ASSIGNMENT(bitwise_and, &)
	ArithmeticWrapper_COMPOUND_ASSIGNMENT(bitwise_or, |)
	ArithmeticWrapper_COMPOUND_ASSIGNMENT(bitwise_xor, ^)
	ArithmeticWrapper_COMPOUND_ASSIGNMENT(left_shift, <<)
	ArithmeticWrapper_COMPOUND_ASSIGNMENT(right_shift, >>)

	#undef ArithmeticWrapper_COMPOUND_ASSIGNMENT
};


template<typename T>
inline constexpr bool test = amgg::has_op_addition_assignment_v<ArithmeticWrapper<T>&, T>;

int main(){
	static_assert(test<float>);
	static_assert(test<A>);
	static_assert(test<B>);
	static_assert(!test<C>);
	static_assert(!test<D>);
	static_assert(!test<E>);
}
