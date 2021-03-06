/*!
@file
Defines adapters for standard components in the `<type_traits>` header, and
also most things in the `boost::hana::traits` namespace.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_EXT_STD_TYPE_TRAITS_HPP
#define BOOST_HANA_EXT_STD_TYPE_TRAITS_HPP

#include <boost/hana/ext/std/integral_constant.hpp>

#include <boost/hana/detail/std/size_t.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/type.hpp>

#include <type_traits>


namespace boost { namespace hana { namespace traits {
    ///////////////////
    // Type properties
    ///////////////////
    // Primary type categories
    constexpr auto is_void = trait<std::is_void>;
    constexpr auto is_null_pointer = trait<std::is_null_pointer>;
    constexpr auto is_integral = trait<std::is_integral>;
    constexpr auto is_floating_point = trait<std::is_floating_point>;
    constexpr auto is_array = trait<std::is_array>;
    constexpr auto is_enum = trait<std::is_enum>;
    constexpr auto is_union = trait<std::is_union>;
    constexpr auto is_class = trait<std::is_class>;
    constexpr auto is_function = trait<std::is_function>;
    constexpr auto is_pointer = trait<std::is_pointer>;
    constexpr auto is_lvalue_reference = trait<std::is_lvalue_reference>;
    constexpr auto is_rvalue_reference = trait<std::is_rvalue_reference>;
    constexpr auto is_member_object_pointer = trait<std::is_member_object_pointer>;
    constexpr auto is_member_function_pointer = trait<std::is_member_function_pointer>;

    // Composite type categories
    constexpr auto is_fundamental = trait<std::is_fundamental>;
    constexpr auto is_arithmetic = trait<std::is_arithmetic>;
    constexpr auto is_scalar = trait<std::is_scalar>;
    constexpr auto is_object = trait<std::is_object>;
    constexpr auto is_compound = trait<std::is_compound>;
    constexpr auto is_reference = trait<std::is_reference>;
    constexpr auto is_member_pointer = trait<std::is_member_pointer>;

    // Type properties
    constexpr auto is_const = trait<std::is_const>;
    constexpr auto is_volatile = trait<std::is_volatile>;
    constexpr auto is_trivial = trait<std::is_trivial>;
    constexpr auto is_trivially_copyable = trait<std::is_trivially_copyable>;
    constexpr auto is_standard_layout = trait<std::is_standard_layout>;
    constexpr auto is_pod = trait<std::is_pod>;
    constexpr auto is_literal_type = trait<std::is_literal_type>;
    constexpr auto is_empty = trait<std::is_empty>;
    constexpr auto is_polymorphic = trait<std::is_polymorphic>;
    constexpr auto is_abstract = trait<std::is_abstract>;
    constexpr auto is_signed = trait<std::is_signed>;
    constexpr auto is_unsigned = trait<std::is_unsigned>;

    // Supported operations
    constexpr auto is_constructible = trait<std::is_constructible>;
    constexpr auto is_trivially_constructible = trait<std::is_trivially_constructible>;
    constexpr auto is_nothrow_constructible = trait<std::is_nothrow_constructible>;

    constexpr auto is_default_constructible = trait<std::is_default_constructible>;
    constexpr auto is_trivially_default_constructible = trait<std::is_trivially_default_constructible>;
    constexpr auto is_nothrow_default_constructible = trait<std::is_nothrow_default_constructible>;

    constexpr auto is_copy_constructible = trait<std::is_copy_constructible>;
    constexpr auto is_trivially_copy_constructible = trait<std::is_trivially_copy_constructible>;
    constexpr auto is_nothrow_copy_constructible = trait<std::is_nothrow_copy_constructible>;

    constexpr auto is_move_constructible = trait<std::is_move_constructible>;
    constexpr auto is_trivially_move_constructible = trait<std::is_trivially_move_constructible>;
    constexpr auto is_nothrow_move_constructible = trait<std::is_nothrow_move_constructible>;

    constexpr auto is_assignable = trait<std::is_assignable>;
    constexpr auto is_trivially_assignable = trait<std::is_trivially_assignable>;
    constexpr auto is_nothrow_assignable = trait<std::is_nothrow_assignable>;

    constexpr auto is_copy_assignable = trait<std::is_copy_assignable>;
    constexpr auto is_trivially_copy_assignable = trait<std::is_trivially_copy_assignable>;
    constexpr auto is_nothrow_copy_assignable = trait<std::is_nothrow_copy_assignable>;

    constexpr auto is_move_assignable = trait<std::is_move_assignable>;
    constexpr auto is_trivially_move_assignable = trait<std::is_trivially_move_assignable>;
    constexpr auto is_nothrow_move_assignable = trait<std::is_nothrow_move_assignable>;

    constexpr auto is_destructible = trait<std::is_destructible>;
    constexpr auto is_trivially_destructible = trait<std::is_trivially_destructible>;
    constexpr auto is_nothrow_destructible = trait<std::is_nothrow_destructible>;

    constexpr auto has_virtual_destructor = trait<std::has_virtual_destructor>;

    // Property queries
    constexpr auto alignment_of = trait<std::alignment_of>;
    constexpr auto rank = trait<std::rank>;
    constexpr struct _extent {
        template <typename T, typename N>
        constexpr auto operator()(T, N n) const {
            constexpr unsigned n_ = hana::value(n);
            return integral_constant<
                decltype(std::extent<typename T::type, n_>::value),
                std::extent<typename T::type, n_>::value
            >;
        }

        template <typename T>
        constexpr auto operator()(T t) const
        { return (*this)(t, uint<0>); }
    } extent{};

    // Type relationships
    constexpr auto is_same = trait<std::is_same>;
    constexpr auto is_base_of = trait<std::is_base_of>;
    constexpr auto is_convertible = trait<std::is_convertible>;

    ///////////////////
    // Type modifications
    ///////////////////
    // Const-volatility specifiers
    constexpr auto remove_cv = metafunction<std::remove_cv>;
    constexpr auto remove_const = metafunction<std::remove_const>;
    constexpr auto remove_volatile = metafunction<std::remove_volatile>;

    constexpr auto add_cv = metafunction<std::add_cv>;
    constexpr auto add_const = metafunction<std::add_const>;
    constexpr auto add_volatile = metafunction<std::add_volatile>;

    // References
    constexpr auto remove_reference = metafunction<std::remove_reference>;
    constexpr auto add_lvalue_reference = metafunction<std::add_lvalue_reference>;
    constexpr auto add_rvalue_reference = metafunction<std::add_rvalue_reference>;

    // Pointers
    constexpr auto remove_pointer = metafunction<std::remove_pointer>;
    constexpr auto add_pointer = metafunction<std::add_pointer>;

    // Sign modifiers
    constexpr auto make_signed = metafunction<std::make_signed>;
    constexpr auto make_unsigned = metafunction<std::make_unsigned>;

    // Arrays
    constexpr auto remove_extent = metafunction<std::remove_extent>;
    constexpr auto remove_all_extents = metafunction<std::remove_all_extents>;

    // Miscellaneous transformations
    constexpr struct _aligned_storage {
        template <typename Len, typename Align>
        constexpr auto operator()(Len len, Align align) const {
            constexpr detail::std::size_t len_ = hana::value(len);
            constexpr detail::std::size_t align_ = hana::value(align);
            return type<std::aligned_storage_t<len_, align_>>;
        }

        template <typename Len>
        constexpr auto operator()(Len len) const {
            constexpr detail::std::size_t len_ = hana::value(len);
            return type<std::aligned_storage_t<len_>>;
        }
    } aligned_storage{};

    constexpr struct _aligned_union {
        template <typename Len, typename ...T>
        constexpr auto operator()(Len len, T...) const {
            constexpr detail::std::size_t len_ = hana::value(len);
            return type<std::aligned_union_t<len_, typename T::type...>>;
        }
    } aligned_union{};

    constexpr auto decay = metafunction<std::decay>;
    // enable_if
    // disable_if
    // conditional

    constexpr auto common_type = metafunction<std::common_type>;
    constexpr auto underlying_type = metafunction<std::underlying_type>;
    constexpr auto result_of = metafunction<std::result_of>;
}}} // end namespace boost::hana::traits

#endif // !BOOST_HANA_EXT_STD_TYPE_TRAITS_HPP
