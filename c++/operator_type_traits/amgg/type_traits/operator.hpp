#pragma once
#ifndef _AMGG_TYPE_TRAITS_OPERATOR
#define _AMGG_TYPE_TRAITS_OPERATOR

#include <type_traits>
#include <utility>

namespace amgg{
	namespace operator_type_traits{
		#define OP_RETURNTYPE_NAME(op_name) op_ ## op_name ## _returntype
		#define HASOP_NAME(op_name) has_op_ ## op_name
		#define HASOP_IMPL_NAME(op_name) __has_op_ ## op_name ## _impl
		#define HASOP_V_NAME(op_name) has_op_ ## op_name ## _v


		#define BINARY_OP(op_name, op_returntype) \
		template<typename T1, typename T2> \
		using OP_RETURNTYPE_NAME(op_name) = decltype(op_returntype); \
		\
		template<typename T1, typename T2, typename Enable = void> \
		struct HASOP_IMPL_NAME(op_name) : std::false_type {}; \
		\
		template<typename T1, typename T2> \
		struct HASOP_IMPL_NAME(op_name) <T1, T2, std::void_t<decltype(op_returntype)>> : std::true_type {}; \
		\
		template<typename T1, typename T2> \
		using HASOP_NAME(op_name) = HASOP_IMPL_NAME(op_name)<T1, T2>; \
		\
		template<typename T1, typename T2> \
		inline constexpr bool HASOP_V_NAME(op_name) = HASOP_NAME(op_name)<T1, T2>::value;


		#define BINARY_OP_ALIAS(op_name, alias_name) \
		template<typename T1, typename T2> \
		using OP_RETURNTYPE_NAME(alias_name) = OP_RETURNTYPE_NAME(op_name)<T1, T2>; \
		\
		template<typename T1, typename T2> \
		using HASOP_NAME(alias_name) = HASOP_IMPL_NAME(op_name)<T1, T2>; \
		template<typename T1, typename T2> \
		inline constexpr bool HASOP_V_NAME(alias_name) = HASOP_NAME(op_name)<T1, T2>::value;


		#define BINARY_INFIX_OP(op_name, op) \
		BINARY_OP(op_name, std::declval<T1>() op std::declval<T2>())

		#define UNARY_OP(op_name, op_returntype) \
		template<typename T> \
		using OP_RETURNTYPE_NAME(op_name) = decltype(op_returntype); \
		\
		template<typename T, typename Enable = void> \
		struct HASOP_IMPL_NAME(op_name) : std::false_type {}; \
		\
		template<typename T> \
		struct HASOP_IMPL_NAME(op_name) <T, std::void_t<decltype(op_returntype)>> : std::true_type {}; \
		\
		template<typename T> \
		using HASOP_NAME(op_name) = HASOP_IMPL_NAME(op_name)<T>; \
		\
		template<typename T> \
		inline constexpr bool HASOP_V_NAME(op_name) = HASOP_NAME(op_name)<T>::value;

		#define UNARY_OP_ALIAS(op_name, alias_name) \
		template<typename T1, typename T2> \
		using OP_RETURNTYPE_NAME(alias_name) = OP_RETURNTYPE_NAME(op_name)<T1, T2>; \
		\
		template<typename T1, typename T2> \
		using HASOP_NAME(alias_name) = HASOP_IMPL_NAME(op_name)<T1, T2>; \
		template<typename T1, typename T2> \
		inline constexpr bool HASOP_V_NAME(alias_name) = HASOP_NAME(op_name)<T1, T2>::value;

		#define UNARY_PREFIX_OP(name, op) \
		UNARY_OP(name, op std::declval<T>())

		#define UNARY_POSTFIX_OP(name, op) \
		UNARY_OP(name, std::declval<T>() op)

		// assignment operators - https://en.cppreference.com/w/cpp/language/operator_assignment
		BINARY_INFIX_OP(assignment, =)
		BINARY_INFIX_OP(addition_assignment, +=)
		BINARY_INFIX_OP(subtraction_assignment, -=)
		BINARY_INFIX_OP(multiplication_assignment, *=)
		BINARY_INFIX_OP(division_assignment, /=)
		BINARY_INFIX_OP(modulo_assignment, %=)
		BINARY_INFIX_OP(bitwise_and_assignment, &=)
		BINARY_INFIX_OP(bitwise_or_assignment, |=)
		BINARY_INFIX_OP(bitwise_xor_assignment, ^=)
		BINARY_INFIX_OP(left_shift_assignment, <<=)
		BINARY_INFIX_OP(right_shift_assignment, >>=)
		// increment/decrement operators - https://en.cppreference.com/w/cpp/language/operator_incdec
		UNARY_PREFIX_OP(pre_increment, ++)
		UNARY_PREFIX_OP(pre_decrement, --)
		UNARY_POSTFIX_OP(post_increment, ++)
		UNARY_POSTFIX_OP(post_decrement, --)
		// arithmetic operators - https://en.cppreference.com/w/cpp/language/operator_arithmetic
		UNARY_PREFIX_OP(unary_plus, +)
		UNARY_PREFIX_OP(unary_minus, -)
		BINARY_INFIX_OP(addition, +)
		BINARY_INFIX_OP(subtraction, -)
		BINARY_INFIX_OP(multiplication, *)
		BINARY_INFIX_OP(division, /)
		BINARY_INFIX_OP(modulo, %)
		UNARY_PREFIX_OP(bitwise_not, ~)
		BINARY_INFIX_OP(bitwise_and, &)
		BINARY_INFIX_OP(bitwise_or, |)
		BINARY_INFIX_OP(bitwise_xor, ^)
		BINARY_INFIX_OP(left_shift, <<)
		BINARY_INFIX_OP(right_shift, >>)
		// logical operators - https://en.cppreference.com/w/cpp/language/operator_logical
		UNARY_PREFIX_OP(negation, !)
		BINARY_INFIX_OP(and, &&)
		BINARY_INFIX_OP(or, ||)
		// comparison operators - https://en.cppreference.com/w/cpp/language/operator_comparison
		BINARY_INFIX_OP(equal_to, ==)
		BINARY_INFIX_OP(not_equal_to, !=)
		BINARY_INFIX_OP(less_than, <)
		BINARY_INFIX_OP(greater_than, >)
		BINARY_INFIX_OP(less_than_or_equal_to, <=)
		BINARY_INFIX_OP(greater_than_or_equal_to, >=)
#ifdef __cpp_impl_three_way_comparison
		BINARY_INFIX_OP(three_way_comparison, <=>)
#endif
		// member access - https://en.cppreference.com/w/cpp/language/operator_member_access
		BINARY_OP(subscript, std::declval<T1>()[std::declval<T2>()])
		UNARY_PREFIX_OP(indirection, *)
		UNARY_PREFIX_OP(address_of, &)
		// member of object (a.b)
		// member of pointer (a->b)
		// pointer to member of object (a.*b)
		// pointer to member of pointer (a->*b)

		#undef OP_RETURNTYPE_NAME
		#undef HASOP_NAME
		#undef HASOP_IMPL_NAME
		#undef HASOP_V_NAME

		#undef BINARY_OP
		#undef BINARY_OP_ALIAS
		#undef BINARY_INFIX_OP
		#undef UNARY_OP
		#undef UNARY_OP_ALIAS
		#undef UNARY_PREFIX_OP
		#undef UNARY_POSTFIX_OP
	}
	using namespace operator_type_traits;
}

#endif