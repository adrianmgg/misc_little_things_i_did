#include <cstdlib> // EXIT_SUCCESS, EXIT_FAILURE
#include <ratio>
#include <type_traits>


namespace thing{
	class thing_base {
	public:
		double value;
		thing_base(double value): value(value) {}
		thing_base(): thing_base(0) {}
	};

	template<typename T>
	using enable_if_thing_type = std::enable_if_t<std::is_base_of<thing_base, T>::value>;

	template<typename A, typename B>
	class op_add : public thing_base{
	};

	template<typename A, typename B>
	class op_sub : public thing_base{
	};

	template<typename A, typename B>
	class op_mul : public thing_base{
	};

	template<typename A, typename B>
	class op_div : public thing_base{
	public:
		template<typename C>
		op_div<A, op_mul<B, C>>& operator*(const C& c){
		}

		template<typename C, typename D>
		op_div<op_mul<A, C>, op_mul<B, D>>& operator*(const op_div<C, D>& other){
		}
	};


	enum UnitCategory{
		DISTANCE,
		TIME
	};


	template<UnitCategory Category>
	class unit_base : public thing_base{
	public:
		unit_base(double value): thing_base(value) {}

		template<typename A, class = enable_if_thing_type<A>>
		op_mul<unit_base<Category>, A> operator* (const A& other){
			op_mul<unit_base<Category>, A> ret;
			ret.value = value * other.value;
			return ret;
		}

		template<typename A, class = enable_if_thing_type<A>>
		op_div<unit_base<Category>, A> operator/ (const A& other){
			op_div<unit_base<Category>, A> ret;
			ret.value = value / other.value;
			return ret;
		}

		template<typename A, class = enable_if_thing_type<A>>
		op_add<unit_base<Category>, A> operator+ (const A& other){
			op_add<unit_base<Category>, A> ret;
			ret.value = value + other.value;
			return ret;
		}

		template<typename A, class = enable_if_thing_type<A>>
		op_sub<unit_base<Category>, A> operator- (const A& other){
			op_sub<unit_base<Category>, A> ret;
			ret.value = value - other.value;
			return ret;
		}
	};

	namespace types{
		using meters = unit_base<DISTANCE>;
		using seconds = unit_base<TIME>;
		using velocity = op_div<meters, seconds>;
		using acceleration = op_div<meters, op_mul<seconds, seconds>>;
	}
}

using namespace thing::types;

int main() {
	meters x(10.0);
	seconds t(2.0);
	acceleration n = x / (t * t);
	velocity v_0x;
	acceleration a_x;
	return 0;
}
