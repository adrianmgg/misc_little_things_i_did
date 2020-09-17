#include <type_traits>
#include <iostream>

using std::declval, std::enable_if, std::enable_if_t;


#define lazyPrint(x) std::cout << #x "=" << (x) << std::endl




template<typename Base, typename Derived>
// using enable_if_is_subclass = std::enable_if_t<std::is_convertible<Base, Derived>::value>;
using enable_if_is_subclass = int;  // TODO



template<typename T>
class OP_compat;


template<typename C, typename T1, typename T2>
class OP2 : public OP_compat<OP2<C, T1, T2>>{
public:
	T1 lhs;
	T2 rhs;
	OP2(T1 lhs, T2 rhs): lhs(lhs), rhs(rhs) {}
	OP2(): lhs(), rhs() {}
};



template<typename T1, typename T2>
class OP_add : public OP2<OP_add<T1, T2>, T1, T2> {
private:
	using calcType = decltype(declval<T1>() + declval<T2>());

	template<typename>
	calcType calc_impl(){
		return this->lhs + this->rhs;
	}

	template<>
	calcType calc_impl<decltype(declval<T1>().calc())>(){
		return this->lhs + this->rhs;
	}
public:
	using OP2<OP_add<T1, T2>, T1, T2>::OP2;
	
	template<typename T = calcType>
	calcType calc(){
		return this->lhs + this->rhs;
	}
};

template<typename T1, typename T2>
class OP_sub : public OP2<OP_sub<T1, T2>, T1, T2> {
	using OP2<OP_sub<T1, T2>, T1, T2>::OP2;
};

template<typename T1, typename T2>
class OP_mul : public OP2<OP_mul<T1, T2>, T1, T2> {
	using OP2<OP_mul<T1, T2>, T1, T2>::OP2;
};

template<typename T1, typename T2>
class OP_div : public OP2<OP_div<T1, T2>, T1, T2> {
	using OP2<OP_div<T1, T2>, T1, T2>::OP2;
};


template<typename T>
class OP_compat{
private:
	template<typename T_>
	using enable_if_is_op_compat = enable_if_is_subclass<OP_compat<T_>, T_>;
public:
	template<typename T2, class = enable_if_is_op_compat<T2>>
	OP_add<T, T2> operator+(const T2& other) {
		OP_add<T, T2> ret;
		return ret;
	}

	template<typename T2, class = enable_if_is_op_compat<T2>>
	OP_sub<T, T2> operator-(const T2& other) {
		OP_sub<T, T2> ret;
		return ret;
	}

	template<typename T2, class = enable_if_is_op_compat<T2>>
	OP_mul<T, T2> operator*(const T2& other) {
		OP_mul<T, T2> ret;
		return ret;
	}

	template<typename T2, class = enable_if_is_op_compat<T2>>
	OP_div<T, T2> operator/(const T2& other) {
		OP_div<T, T2> ret;
		return ret;
	}
};



class Meters : public OP_compat<Meters>{};
class Seconds : public OP_compat<Seconds>{};
using Velocity = OP_div<Meters, Seconds>;
using Acceleration = OP_div<Meters, OP_mul<Seconds, Seconds>>;

int main() {
	Meters x;
	Seconds t;
	Velocity v = x / t;
	auto foo = v / t;
	auto bar = v / v;
	OP_add<float, float> baz(1, 2);
	lazyPrint(baz.lhs);
	lazyPrint(baz.rhs);
	lazyPrint(baz.calc());
}
