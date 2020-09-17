#include <type_traits>
using std::integral_constant, std::is_same_v, std::conditional;


template<typename X, typename T>
struct derivative{
    using type = void;
};

template<typename X, typename T, T V>
struct derivative<X, integral_constant<T, V>>{
	using type = integral_constant<T, 0>;
};


template<int ID>
struct runtime_varying_variable : integral_constant<int, ID> {};
template<typename LHS, typename RHS>
struct add{};
template<typename LHS, typename RHS>
struct subtract{};
template<typename LHS, typename RHS>
struct multiply{};
template<typename LHS, typename RHS>
struct divide{};
template<typename BASE, typename EXPONENT>
struct power{};


template<typename X, int ID>
struct derivative<X, runtime_varying_variable<ID>>{
	using type = conditional<
		is_same_v<runtime_varying_variable<ID>, X>,
		integral_constant<int, 0>,
		integral_constant<int, 1>
	>;
};

template<typename X, typename LHS, typename RHS>
struct derivative<X, add<LHS, RHS>>{
    using type = add<typename derivative<X, LHS>::type, typename derivative<X, RHS>::type>;
};

template<typename X, typename LHS, typename RHS>
struct derivative<X, subtract<LHS, RHS>>{
    using type = subtract<typename derivative<X, LHS>::type, typename derivative<X, RHS>::type>;
};

template<typename X, typename LHS, typename RHS>
struct derivative<X, multiply<LHS, RHS>>{
    using type = add<
		multiply<derivative<X, LHS>, RHS>,
		multiply<RHS, derivative<X, LHS>>
	>;
};

template<typename X, typename LHS, typename RHS>
struct derivative<X, divide<LHS, RHS>>{
    using type = divide<
		subtract<
			multiply<derivative<X, LHS>, RHS>,
			multiply<RHS, derivative<X, LHS>>
		>,
		power<RHS, integral_constant<int, 2>>
	>;
};

template<typename X, typename Base, typename Exponent>
struct derivative<X, power<Base, Exponent>>{
};




int main(){
	using x = runtime_varying_variable<0>;
	using foo = add<add<multiply<x, x>, x>, divide<x, x>>;
	using bar = derivative<x, foo>::type;
}
