/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <test/identity.hpp>

#include <boost/hana/assert.hpp>
#include <boost/hana/core/operators.hpp>
#include <boost/hana/functional/always.hpp>
#include <boost/hana/functional/compose.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>

#include <test/auto/base.hpp>
#include <test/injection.hpp>
#include <test/cnumeric.hpp>

// tested instances
#include <test/auto/applicative.hpp>
#include <test/auto/functor.hpp>
#include <test/auto/monad.hpp>
#include <test/auto/traversable.hpp>
using namespace boost::hana;


namespace boost { namespace hana { namespace test {
    template <>
    auto instances<Identity> = make<Tuple>(
        type<Functor>,
        type<Applicative>,
        type<Monad>,
        type<Traversable>
    );

    template <>
    auto objects<Identity> = make<Tuple>(
        identity(x<0>),
        identity(x<1>),
        identity(x<2>),
        identity(x<3>)
    );
}}}

namespace boost { namespace hana {
    template <>
    struct operators::of<test::Identity>
        : operators::of<Monad>
    { };
}}


int main() {
    using test::x;
    using test::cnumeric;
    auto f = test::injection([]{});

    test::check_datatype<test::Identity>();

    // Functor
    {
        auto functor = test::identity;
        // adjust_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                adjust_if(functor(x<0>), always(cnumeric<bool, true>), f),
                functor(f(x<0>))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                adjust_if(functor(x<0>), always(cnumeric<bool, false>), f),
                functor(x<0>)
            ));
        }

        // fill
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                fill(functor(x<0>), x<1>),
                functor(x<1>)
            ));
        }

        // transform
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                transform(functor(x<0>), f),
                functor(f(x<0>))
            ));
        }

        // replace_if
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                replace_if(functor(x<0>), always(cnumeric<bool, true>), x<1>),
                functor(x<1>)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                replace_if(functor(x<0>), always(cnumeric<bool, false>), x<1>),
                functor(x<0>)
            ));
        }
    }

    // Applicative
    {
        auto a = test::identity;
        using A = test::Identity;

        // ap
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(a(f), a(x<0>)),
                a(f(x<0>))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(a(f), a(x<0>), a(x<1>)),
                a(f(x<0>, x<1>))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(a(f), a(x<0>), a(x<1>), a(x<2>)),
                a(f(x<0>, x<1>, x<2>))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                ap(a(f), a(x<0>), a(x<1>), a(x<2>), a(x<3>)),
                a(f(x<0>, x<1>, x<2>, x<3>))
            ));
        }

        // lift
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                lift<A>(x<0>),
                a(x<0>)
            ));
        }
    }

    // Monad
    {
        auto monad = test::identity;
        using M = test::Identity;
        auto f = compose(monad, test::injection([]{}));
        auto g = compose(monad, test::injection([]{}));

        // bind
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                bind(monad(x<1>), f),
                f(x<1>)
            ));
        }

        // tap
        {
            bool executed = false;
            auto exec = [&](auto) { executed = true; };
            BOOST_HANA_CONSTANT_CHECK(equal(
                bind(monad(x<0>), tap<M>(exec)),
                monad(x<0>)
            ));
            BOOST_HANA_RUNTIME_CHECK(executed);
        }

        // then
        {
            struct invalid { };
            BOOST_HANA_CONSTANT_CHECK(equal(
                then(monad(invalid{}), monad(x<0>)),
                monad(x<0>)
            ));
        }

        // operators
        {
            using namespace boost::hana::operators;

            BOOST_HANA_CONSTANT_CHECK(equal(
                monad(x<1>) | f,
                bind(monad(x<1>), f)
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                monad(x<1>) | f | g,
                bind(bind(monad(x<1>), f), g)
            ));
        }
    }

    // Traversable
    {
        // traverse
        {
            auto f = test::injection([]{});
            BOOST_HANA_CONSTANT_CHECK(equal(
                traverse<test::Identity>(test::identity(x<0>), compose(test::identity, f)),
                test::identity(test::identity(f(x<0>)))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                traverse<Tuple>(test::identity(x<0>), compose(make<Tuple>, f)),
                make<Tuple>(test::identity(f(x<0>)))
            ));
        }

        // sequence
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                sequence<test::Identity>(test::identity(test::identity(x<0>))),
                test::identity(test::identity(x<0>))
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                sequence<Tuple>(test::identity(make<Tuple>(x<0>, x<1>, x<2>))),
                make<Tuple>(test::identity(x<0>), test::identity(x<1>), test::identity(x<2>))
            ));
        }
    }
}
