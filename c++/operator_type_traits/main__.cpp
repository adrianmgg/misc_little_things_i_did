#include <iostream>
using std::ostream, std::cout, std::endl;
#include <type_traits>
using std::enable_if, std::enable_if_t, std::is_default_constructible_v, std::void_t;
#include <utility>
using std::declval;

#include "amgg/type_traits/operator.hpp"
#include "amgg/type_traits/misc.hpp"
using amgg::bool_if_t;
#include "amgg/util/lazyprint.hpp"

struct A{
	A operator+(const A& other){
		A ret;
		return ret;
	}
};

struct NotDefaultConstructible{
	int i;
	NotDefaultConstructible(int i): i(i) {};
};


template<typename T1, typename T2 = T1>
class Pair{
public:
	T1 v1;
	T2 v2;
	using t1 = T1;
	using t2 = T2;

	Pair(const T1& v1, const T2& v2): v1(v1), v2(v2) {}
	
	// default Pair<T1, T2> constructor exists iif T1 and T2 are default constructible
	template<typename = enable_if_t<is_default_constructible_v<T1> && is_default_constructible_v<T2>>>
	Pair(): v1(), v2() {}

	// ostream operator<< for Pair<T1, T2> exists iif ostream operator<< exists for T1 and T2 
	template<typename = enable_if_t<amgg::has_op_left_shift_v<ostream&, T1>>, typename = enable_if_t<amgg::has_op_left_shift_v<ostream&, T2>>>
	friend ostream& operator<<(ostream& o, const Pair<T1, T2>& p){
		o << "Pair(" << p.v1 << ", " << p.v2 << ")";
		return o;
	}

	template<typename TA, typename TB, typename = enable_if_t<amgg::has_op_addition_assignment_v<T1&, const TA&> && amgg::has_op_addition_assignment_v<T2&, const TB&>>>
	Pair<T1, T2>& operator+=(const Pair<TA, TB>& rhs){
		this->v1 += rhs.v1;
		this->v2 += rhs.v2;
		return *this;
	}

	template<typename TA, typename TB, typename RT1 = amgg::op_addition_returntype<const T1&, const TA>, typename RT2 = amgg::op_addition_returntype<const T2&, const TB>>
	Pair<RT1, RT2> operator+(const Pair<TA, TB> other){
		Pair<RT1, RT2> ret(this->v1 + other.v1, this->v2 + other.v2);
		return ret;
	}
};


int main() {
	Pair<float> p1(1, 2);
	Pair<A, A> p2;

	p1 += p1;
}
