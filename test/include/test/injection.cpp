/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/assert.hpp>
#include <boost/hana/config.hpp>
#include <boost/hana/functional/infix.hpp>
#include <boost/hana/logical.hpp>
#include <boost/hana/tuple.hpp>

#include <test/injection.hpp>
using namespace boost::hana;


BOOST_HANA_CONSTEXPR_LAMBDA auto implies = infix([](auto p, auto q) {
    return or_(not_(p), q);
});

int main() {
    using test::x;
    BOOST_HANA_CONSTEXPR_LAMBDA auto f = test::injection([]{});
    BOOST_HANA_CONSTEXPR_LAMBDA auto g = test::injection([]{});

    // injectivity
    {
        BOOST_HANA_CONSTEXPR_LAMBDA auto is_injection = [=](auto x, auto y) {
            return equal(f(x), f(y)) ^implies^ equal(x, y);
        };

        BOOST_HANA_CONSTANT_CHECK(all(
            ap(make<Tuple>(is_injection),
                make<Tuple>(x<0>, x<1>, x<2>, x<3>),
                make<Tuple>(x<0>, x<1>, x<2>, x<3>))
        ));
    }

    // various number of arguments
    {
        BOOST_HANA_CONSTANT_CHECK(not_equal(
            f(), g()
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            f(), f()
        ));

        BOOST_HANA_CONSTANT_CHECK(not_equal(
            f(x<0>),
            g(x<0>)
        ));

        BOOST_HANA_CONSTANT_CHECK(not_equal(
            f(x<0>, x<1>),
            f(x<0>)
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            f(x<0>, x<1>),
            f(x<0>, x<1>)
        ));

        BOOST_HANA_CONSTANT_CHECK(not_equal(
            f(x<0>, x<1>),
            f(x<1>, x<0>)
        ));
    }
}
