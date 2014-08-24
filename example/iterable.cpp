/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/assert.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/foreign.hpp>
#include <boost/hana/integral.hpp>
#include <boost/hana/range.hpp>
#include <boost/hana/tuple.hpp>

#include <sstream>
using namespace boost::hana;


int main() {
    //! [head]
    BOOST_HANA_CONSTEXPR_ASSERT(head(tuple(1, '2', 3.3, nullptr)) == 1);
    //! [head]

    //! [at]
    BOOST_HANA_CONSTEXPR_ASSERT(at(int_<0>, tuple(0, '1', 2.0)) == 0);
    BOOST_HANA_CONSTEXPR_ASSERT(at(int_<1>, tuple(0, '1', 2.0)) == '1');
    BOOST_HANA_CONSTEXPR_ASSERT(at(int_<2>, tuple(0, '1', 2.0)) == 2.0);
    //! [at]

    //! [drop]
    BOOST_HANA_CONSTEXPR_LAMBDA auto xs = tuple(0, '1', 2.0);
    BOOST_HANA_CONSTEXPR_ASSERT(drop(int_<0>, xs) == xs);
    BOOST_HANA_CONSTEXPR_ASSERT(drop(int_<1>, xs) == tuple('1', 2.0));
    BOOST_HANA_CONSTEXPR_ASSERT(drop(int_<2>, xs) == tuple(2.0));
    BOOST_HANA_CONSTANT_ASSERT(drop(int_<3>, xs) == tuple());
    BOOST_HANA_CONSTANT_ASSERT(drop(int_<4>, xs) == tuple());
    //! [drop]

    {
    //! [drop_until]
    using namespace literals;
    BOOST_HANA_CONSTEXPR_LAMBDA auto positive = [](auto x) {
        return x > int_<0>;
    };

    BOOST_HANA_CONSTANT_ASSERT(
        drop_until(positive, range(int_<-3>, int_<6>)) == range(int_<1>, int_<6>)
    );

    BOOST_HANA_CONSTANT_ASSERT(
        drop_until(positive, tuple(1_c, -2_c, 4_c, 5_c)) == tuple(1_c, -2_c, 4_c, 5_c)
    );
    //! [drop_until]
    }

    {
    //! [drop_while]
    using namespace literals;
    BOOST_HANA_CONSTEXPR_LAMBDA auto negative = [](auto x) {
        return x < int_<0>;
    };

    BOOST_HANA_CONSTANT_ASSERT(
        drop_while(negative, range(int_<-3>, int_<6>)) == range(int_<0>, int_<6>)
    );

    BOOST_HANA_CONSTANT_ASSERT(
        drop_while(negative, tuple(1_c, -2_c, 4_c, 5_c)) == tuple(1_c, -2_c, 4_c, 5_c)
    );
    //! [drop_while]
    }

    {
    //! [for_each]
    std::stringstream ss;
    for_each(tuple(0, '1', "234", 5.5), [&](auto x) {
        ss << x << ' ';
    });

    BOOST_HANA_RUNTIME_ASSERT(ss.str() == "0 1 234 5.5 ");
    //! [for_each]
    }

    //! [is_empty]
    BOOST_HANA_CONSTANT_ASSERT(!is_empty(tuple(1, '2')));
    BOOST_HANA_CONSTANT_ASSERT( is_empty(tuple()));
    //! [is_empty]

    //! [last]
    BOOST_HANA_CONSTEXPR_ASSERT(last(tuple(1, '2', 3.3, nullptr)) == nullptr);
    //! [last]

    //! [tail]
    BOOST_HANA_CONSTEXPR_ASSERT(
        tail(tuple(1, '2', 3.3, nullptr)) == tuple('2', 3.3, nullptr)
    );
    //! [tail]
}
