/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable.hpp>

#include <boost/hana/detail/static_assert.hpp>
#include <boost/hana/integral.hpp>

#include "minimal_foldable.hpp"
using namespace boost::hana;


int main() {
    BOOST_HANA_STATIC_ASSERT(product(foldable()) == int_<1>);
    BOOST_HANA_STATIC_ASSERT(product(foldable(int_<2>)) == int_<2>);
    BOOST_HANA_STATIC_ASSERT(product(foldable(int_<2>, int_<3>)) == int_<2 * 3>);
    BOOST_HANA_STATIC_ASSERT(product(foldable(int_<2>, int_<3>, int_<4>)) == int_<2 * 3 * 4>);
    BOOST_HANA_STATIC_ASSERT(product(foldable(int_<2>, int_<3>, int_<4>, int_<5>)) == int_<2 * 3 * 4 * 5>);

    BOOST_HANA_STATIC_ASSERT(product(foldable(2)) == 2);
    BOOST_HANA_STATIC_ASSERT(product(foldable(2, 3)) == 2 * 3);
    BOOST_HANA_STATIC_ASSERT(product(foldable(2, 3, 4)) == 2 * 3 * 4);
    BOOST_HANA_STATIC_ASSERT(product(foldable(2, 3, 4, 5)) == 2 * 3 * 4 * 5);
}