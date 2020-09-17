#include <type_traits>
using std::false_type, std::true_type, std::enable_if_t, std::void_t, std::is_same_v;
#include <utility>
using std::declval;
#include <cmath>

#pragma region forward declarations
template<typename LHS, typename RHS>
struct add;
template<typename LHS, typename RHS>
struct subtract;
template<typename LHS, typename RHS>
struct multiply;
template<typename LHS, typename RHS>
struct divide;
template<signed long long int V>
struct whole_number;
template<signed long long int P, signed long long int Q>
struct rational_number;
#pragma endregion


template<typename Child>
struct typespace_ast_operators{
	template<typename Other>
	add<Child, Other> operator+(Other other);
	
	template<typename Other>
	subtract<Child, Other> operator-(Other other);
	
	template<typename Other>
	multiply<Child, Other> operator*(Other other);

	template<typename Other>
	divide<Child, Other> operator/(Other other);
};


template<typename LHS, typename RHS>
struct add : public typespace_ast_operators<add<LHS, RHS>>{
	using simplified = add<LHS, RHS>;
	template<typename T>
	static T calculate(){
		return LHS::template calculate<T>() + RHS::template calculate<T>();
	}
	template<typename T>
	constexpr static T constexpr_calculate(){
		return LHS::template constexpr_calculate<T>() + RHS::template constexpr_calculate<T>();
	}
};

template<typename LHS, typename RHS>
struct subtract : public typespace_ast_operators<subtract<LHS, RHS>>{
	using simplified = subtract<LHS, RHS>;
	template<typename T>
	static T calculate(){
		return LHS::template calculate<T>() - RHS::template calculate<T>();
	}
	template<typename T>
	constexpr static T constexpr_calculate(){
		return LHS::template constexpr_calculate<T>() - RHS::template constexpr_calculate<T>();
	}
};

template<typename LHS, typename RHS>
struct multiply : public typespace_ast_operators<multiply<LHS, RHS>>{
	using simplified = multiply<LHS, RHS>;
	template<typename T>
	static T calculate(){
		return LHS::template calculate<T>() * RHS::template calculate<T>();
	}
	template<typename T>
	constexpr static T constexpr_calculate(){
		return LHS::template constexpr_calculate<T>() * RHS::template constexpr_calculate<T>();
	}
};

template<typename LHS, typename RHS>
struct divide : public typespace_ast_operators<divide<LHS, RHS>>{
	using simplified = divide<LHS, RHS>;
	template<typename T>
	static T calculate(){
		return LHS::template calculate<T>() / RHS::template calculate<T>();
	}
	template<typename T>
	constexpr static T constexpr_calculate(){
		return LHS::template constexpr_calculate<T>() / RHS::template constexpr_calculate<T>();
	}
};

template<signed long long int V>
struct whole_number : public typespace_ast_operators<whole_number<V>>{
	using simplified = whole_number<V>;
	template<typename T>
	static T calculate(){
		return constexpr_calculate<T>();
	}
	template<typename T>
	constexpr static T constexpr_calculate(){
		return T(V);
	}
};

template<signed long long int P, signed long long int Q>
struct rational_number : public typespace_ast_operators<rational_number<P, Q>>{
	using simplified = rational_number<P, Q>;
	template<typename T>
	static T calculate(){
		return constexpr_calculate<T>();
	}
	template<typename T>
	constexpr static T constexpr_calculate(){
		return T(P) / T(Q);
	}
};



struct A : public typespace_ast_operators<A>{
	template<typename T>
	static T calculate(){
		return T(5);
	}
	template<typename T>
	constexpr static T constexpr_calculate(){
		return T(5);
	}
};
struct B : public typespace_ast_operators<B>{
	template<typename T>
	static T calculate(){
		return T(8);
	}
	template<typename T>
	constexpr static T constexpr_calculate(){
		return T(8);
	}
};


int main(){
	A a;
	B b;
	using foo = decltype((a + b) * (a + b));
	using bar = foo::simplified;
	float foo_ = foo::constexpr_calculate<float>();
	float bar_ = bar::constexpr_calculate<float>();
	// static_assert(bar_ == foo_, "");
	return 0;
}
