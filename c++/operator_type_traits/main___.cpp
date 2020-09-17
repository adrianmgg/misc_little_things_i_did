#include <type_traits>
#include <utility>
using std::enable_if_t, std::void_t, std::is_same_v, std::true_type, std::false_type, std::is_default_constructible_v, std::declval;
#include <iostream>
using std::ostream, std::cout, std::endl;
#include "amgg/type_traits/operator.hpp"
using namespace amgg::operator_type_traits;


template<typename T, typename Enable = void>
struct is_counter_impl : false_type {};
template<typename T>
struct is_counter_impl<T, void_t<
	decltype(declval<T>().decrement(), declval<T>().increment())
>> : true_type {};

template<typename Counter, typename Enable = void>
struct counter_supports_print_impl : false_type {};
template<typename Counter>
struct counter_supports_print_impl<Counter, enable_if_t<has_op_left_shift_v<ostream&, Counter>>> : true_type {};

template<typename Counter, typename Enable = void>
struct counter_supports_reset_impl : false_type {};
template<typename Counter>
struct counter_supports_reset_impl<Counter, void_t<decltype(declval<Counter>().reset())>> : true_type {};

template<typename Counter>
struct counter_traits {
	static constexpr bool is_counter = is_counter_impl<Counter>::value;
	static constexpr bool supports_print = counter_supports_print_impl<Counter>::value;
	static constexpr bool supports_reset = counter_supports_reset_impl<Counter>::value;
};


template<typename T>
class BasicCounter{
	static_assert(is_default_constructible_v<T>, "T in BasicCounter<T> must be default constructible");
	static_assert(has_op_post_increment_v<T&> || has_op_pre_increment_v<T&>, "T in BasicCounter<T> must implement the prefix and/or postfix ++ operator(s).");
	static_assert(has_op_post_decrement_v<T&> || has_op_pre_decrement_v<T&>, "T in BasicCounter<T> must implement the prefix and/or postfix -- operator(s).");
private:
	T value;
public:
	BasicCounter(): value() {};
	void increment(){
		if constexpr(has_op_post_increment_v<T&>){
			value++;
		}
		else if(has_op_pre_increment_v<T&>){
			++value;
		}
	}
	void decrement(){
		if constexpr(has_op_post_decrement_v<T&>){
			value--;
		}
		else if(has_op_pre_decrement_v<T&>){
			--value;
		}
	}
	template<typename = enable_if_t<has_op_left_shift_v<ostream&, T>>>
	friend ostream& operator<< (ostream& os, const BasicCounter<T>& counter){
		os << counter.value;
		return os;
	}
};


template<typename Counter>
void foo(Counter& counter, int loops = 8){
	static_assert(counter_traits<Counter>::is_counter, "Argument must be a counter");
	if constexpr(!counter_traits<Counter>::supports_print){
		cout << "(this counter doesn't support printing)" << endl;
	}
	for(int i = 0; i < loops; i++) {
		counter.increment();
		if constexpr(counter_traits<Counter>::supports_print){
			cout << counter << endl;
		}
	}

	if constexpr(counter_traits<Counter>::supports_reset){
		counter.reset();
		cout << "counter reset" << endl;
	}
}



struct A{
	int i = 0;
	A& operator++() {
		i++;
		return *this;
	}
	A& operator--() {
		i--;
		return *this;
	}
};

class CustomCounter{
public:
	void increment(){}
	void decrement(){}
	void reset(){}
	friend ostream& operator<<(ostream& os, const CustomCounter& cc){
		os << "Hello World!";
		return os;
	}
};

int main(){
	cout << "BasicCounter<int>" << endl;
	BasicCounter<int> ctr1;
	foo(ctr1);
	
	cout << endl << "BasicCounter<A>" << endl;
	BasicCounter<A> ctr2;
	foo(ctr2);

	cout << endl << "CustomCounter" << endl;
	CustomCounter ctr3;
	foo(ctr3);
    
	return 0;
}
