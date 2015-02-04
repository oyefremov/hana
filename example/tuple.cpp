/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/integral_constant.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/sequence.hpp>
#include <boost/hana/tuple.hpp>
using namespace boost::hana;


int main() {

{

//! [tuple_c]
BOOST_HANA_CONSTANT_CHECK(
    to<Tuple>(tuple_c<int, 0, 1, 2>)
    ==
    tuple(int_<0>, int_<1>, int_<2>)
);
BOOST_HANA_CONSTANT_CHECK(head(tuple_c<int, 0, 1, 2>) == int_<0>);
//! [tuple_c]

}{

//! [Foldable]
using namespace literals;

constexpr auto numbers = tuple_c<int, 5, -1, 0, -7, -2, 0, -5, 4>;
constexpr auto negatives = tuple_c<int, -1, -7, -2, -5>;

BOOST_HANA_CONSTEXPR_LAMBDA auto keep_negatives = [](auto n, auto acc) {
    return if_(n < 0_c, prepend(n, acc), acc);
};

BOOST_HANA_CONSTANT_CHECK(foldr(numbers, tuple_c<int>, keep_negatives) == negatives);
//! [Foldable]

}{

//! [Tuple_interop]
BOOST_HANA_CONSTEXPR_CHECK(
    to<Tuple>(std::make_tuple(1, '2', 3.3)) == tuple(1, '2', 3.3)
);

BOOST_HANA_CONSTANT_CHECK(
    to<Tuple>(range(int_<1>, int_<4>)) == tuple(int_<1>, int_<2>, int_<3>)
);

BOOST_HANA_CONSTEXPR_CHECK(
    to<Tuple>(std::array<int, 3>{{1, 2, 3}}) == tuple(1, 2, 3)
);
//! [Tuple_interop]

}

}