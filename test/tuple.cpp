/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/tuple.hpp>

#include <boost/hana/integral_constant.hpp>

#include <laws/applicative.hpp>
#include <laws/base.hpp>
#include <laws/comparable.hpp>
#include <laws/foldable.hpp>
#include <laws/functor.hpp>
#include <laws/iterable.hpp>
#include <laws/monad.hpp>
#include <laws/monad_plus.hpp>
#include <laws/orderable.hpp>
#include <laws/searchable.hpp>
#include <laws/sequence.hpp>
#include <laws/traversable.hpp>

#include <string>
#include <type_traits>
#include <utility>
using namespace boost::hana;


template <int i>
using eq = test::ct_eq<i>;

template <int i>
using ord = test::ct_ord<i>;

struct x0; struct x1; struct x2; struct x3; struct x4;

template <typename ...>
struct F { struct type; };


int main() {
    auto eq_tuples = make<Tuple>(
          make<Tuple>()
        , make<Tuple>(eq<0>{})
        , make<Tuple>(eq<0>{}, eq<1>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{})
        , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{}, eq<5>{})
    );
    (void)eq_tuples;

    auto ord_tuples = make<Tuple>(
          make<Tuple>()
        , make<Tuple>(ord<0>{})
        , make<Tuple>(ord<0>{}, ord<1>{})
        , make<Tuple>(ord<0>{}, ord<1>{}, ord<2>{})
        , make<Tuple>(ord<0>{}, ord<1>{}, ord<2>{}, ord<3>{})
        , make<Tuple>(ord<0>{}, ord<1>{}, ord<2>{}, ord<3>{}, ord<4>{})
    );
    (void)ord_tuples;

    auto nested_eqs = make<Tuple>(
          make<Tuple>()
        , make<Tuple>(make<Tuple>(eq<0>{}))
        , make<Tuple>(make<Tuple>(eq<0>{}), make<Tuple>(eq<1>{}, eq<2>{}))
        , make<Tuple>(make<Tuple>(eq<0>{}),
                      make<Tuple>(eq<1>{}, eq<2>{}),
                      make<Tuple>(eq<3>{}, eq<4>{}))
    );
    (void)nested_eqs;

#if BOOST_HANA_TEST_PART == 1
    //////////////////////////////////////////////////////////////////////////
    // Move-only friendliness and reference semantics
    //////////////////////////////////////////////////////////////////////////
    {
        {
            auto xs = make<Tuple>(test::move_only{});
            auto by_val = [](auto) { };

            by_val(std::move(xs));
            by_val(head(std::move(xs)));
            by_val(at_c<0>(std::move(xs)));
            by_val(last(std::move(xs)));
        }

        {
            auto const& xs = make<Tuple>(test::move_only{});
            auto by_const_ref = [](auto const&) { };

            by_const_ref(xs);
            by_const_ref(head(xs));
            by_const_ref(at_c<0>(xs));
            by_const_ref(last(xs));
        }

        {
            auto xs = make<Tuple>(test::move_only{});
            auto by_ref = [](auto&) { };

            by_ref(xs);
            by_ref(head(xs));
            by_ref(at_c<0>(xs));
            by_ref(last(xs));
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // default-constructibility
    //////////////////////////////////////////////////////////////////////////
    {
        _tuple<> z1{}; (void)z1;
        _tuple<int> z2{}; (void)z2;
        _tuple<int, char> z3{}; (void)z3;
        _tuple<int, char, float> z4{}; (void)z4;

        using Types = decltype(tuple_t<x0, x1>);
        Types default_{}; (void)default_;
    }

    //////////////////////////////////////////////////////////////////////////
    // In some cases where a type has a constructor that is way too
    // general, copying a unary tuple holding an object of that type
    // could trigger the instantiation of that constructor. If that
    // constructor was ill-formed, the compilation could fail. We
    // make sure this does not happen.
    //////////////////////////////////////////////////////////////////////////
    {
        auto expr = make<Tuple>(test::trap_construct{});
        auto implicit_copy = expr;          (void)implicit_copy;
        decltype(expr) explicit_copy(expr); (void)explicit_copy;
    }

    //////////////////////////////////////////////////////////////////////////
    // This used to trigger an ICE on Clang
    //////////////////////////////////////////////////////////////////////////
    {
        struct Car { std::string name; };
        auto stuff = make<Tuple>(Car{}, Car{}, Car{});
        any_of(stuff, [](auto&& x) { return true; });
    }

    //////////////////////////////////////////////////////////////////////////
    // `decltype(tuple_t<T...>)` should inherit `_tuple_t<T...>`
    //////////////////////////////////////////////////////////////////////////
    {
        static_assert(std::is_base_of<
            _tuple_t<x0, x1>, decltype(tuple_t<x0, x1>)
        >{}, "");
    }

    //////////////////////////////////////////////////////////////////////////
    // Comparable
    //////////////////////////////////////////////////////////////////////////
    {
        // equal

        // tuple_t
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<>, tuple_t<>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0>, tuple_t<>)));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<>, tuple_t<x0>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0>, tuple_t<x0>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0, x1>, tuple_t<x0>)));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_t<x0>, tuple_t<x0, x1>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0, x1>, tuple_t<x0, x1>));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_t<x0, x1, x2>, tuple_t<x0, x1, x2>));

        // tuple_c
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int>, tuple_c<int>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_c<int, 0>, tuple_c<int>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int, 0>, tuple_c<int, 0>));
        BOOST_HANA_CONSTANT_CHECK(not_(equal(tuple_c<int, 0, 1>, tuple_c<int, 0>)));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int, 0, 1>, tuple_c<int, 0, 1>));
        BOOST_HANA_CONSTANT_CHECK(equal(tuple_c<int, 0, 1, 2>, tuple_c<int, 0, 1, 2>));

        test::TestComparable<Tuple>{eq_tuples};
    }

#elif BOOST_HANA_TEST_PART == 2
    //////////////////////////////////////////////////////////////////////////
    // Orderable
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestOrderable<Tuple>{ord_tuples};
    }

#elif BOOST_HANA_TEST_PART == 3
    //////////////////////////////////////////////////////////////////////////
    // Foldable
    //////////////////////////////////////////////////////////////////////////
    {
        // unpack
        {
            test::_injection<0> f{};

            // tuple
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<Tuple>(), f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<Tuple>(eq<0>{}), f),
                f(eq<0>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<Tuple>(eq<0>{}, eq<1>{}), f),
                f(eq<0>{}, eq<1>{})
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}), f),
                f(eq<0>{}, eq<1>{}, eq<2>{})
            ));

            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<>, f),
                f()
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0>, f),
                f(type<x0>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1>, f),
                f(type<x0>, type<x1>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1, x2>, f),
                f(type<x0>, type<x1>, type<x2>)
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                unpack(tuple_t<x0, x1, x2, x3>, f),
                f(type<x0>, type<x1>, type<x2>, type<x3>)
            ));
        }

        test::TestFoldable<Tuple>{eq_tuples};
    }

#elif BOOST_HANA_TEST_PART == 4
    //////////////////////////////////////////////////////////////////////////
    // Iterable
    //////////////////////////////////////////////////////////////////////////
    {
        // head
        {
            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_t<x0>), type<x0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_t<x0, x1>), type<x0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_t<x0, x1, x2>), type<x0>));

            // tuple_c
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_c<int, 0>), int_<0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_c<int, 0, 1>), int_<0>));
            BOOST_HANA_CONSTANT_CHECK(equal(head(tuple_c<int, 0, 1, 2>), int_<0>));
        }

        // is_empty
        {
            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(is_empty(tuple_t<>));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_t<x0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_t<x0, x1>)));

            // tuple_c
            BOOST_HANA_CONSTANT_CHECK(is_empty(tuple_c<int>));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0, 1>)));
            BOOST_HANA_CONSTANT_CHECK(not_(is_empty(tuple_c<int, 0, 1, 2>)));
        }

        // tail
        {
            // tuple_t
            BOOST_HANA_CONSTANT_CHECK(equal(tail(tuple_t<x0>), tuple_t<>));
            BOOST_HANA_CONSTANT_CHECK(equal(tail(tuple_t<x0, x1>), tuple_t<x1>));
            BOOST_HANA_CONSTANT_CHECK(equal(tail(tuple_t<x0, x1, x2>), tuple_t<x1, x2>));

            // tuple_c
            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_c<int, 0>),
                tuple_c<int>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_c<int, 0, 1>),
                tuple_c<int, 1>
            ));

            BOOST_HANA_CONSTANT_CHECK(equal(
                tail(tuple_c<int, 0, 1, 2>),
                tuple_c<int, 1, 2>
            ));
        }

        test::TestIterable<Tuple>{eq_tuples};
    }

    //////////////////////////////////////////////////////////////////////////
    // Traversable
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestTraversable<Tuple>{};
    }

#elif BOOST_HANA_TEST_PART == 5
    //////////////////////////////////////////////////////////////////////////
    // Searchable
    //////////////////////////////////////////////////////////////////////////
    {
        auto eq_tuples = make<Tuple>(
              make<Tuple>()
            , make<Tuple>(eq<0>{})
            , make<Tuple>(eq<0>{}, eq<1>{})
            , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{})
            , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{})
            , make<Tuple>(eq<0>{}, eq<1>{}, eq<2>{}, eq<3>{}, eq<4>{})
        );

        auto eq_tuple_keys = make<Tuple>(eq<3>{}, eq<5>{});
        test::TestSearchable<Tuple>{eq_tuples, eq_tuple_keys};
    }

#elif BOOST_HANA_TEST_PART == 6
    //////////////////////////////////////////////////////////////////////////
    // Sequence
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestSequence<Tuple>{};
    }

#elif BOOST_HANA_TEST_PART == 7
    //////////////////////////////////////////////////////////////////////////
    // Functor
    //////////////////////////////////////////////////////////////////////////
    {
        // Test transform with tuple_t and Metafunctions
        BOOST_HANA_CONSTANT_CHECK(equal(
            transform(tuple_t<>, metafunction<F>),
            tuple_t<>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            transform(tuple_t<x1>, metafunction<F>),
            tuple_t<F<x1>::type>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            transform(tuple_t<x1, x2>, metafunction<F>),
            tuple_t<F<x1>::type, F<x2>::type>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            transform(tuple_t<x1, x2, x3>, metafunction<F>),
            tuple_t<F<x1>::type, F<x2>::type, F<x3>::type>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            transform(tuple_t<x1, x2, x3, x4>, metafunction<F>),
            tuple_t<F<x1>::type, F<x2>::type, F<x3>::type, F<x4>::type>
        ));

        BOOST_HANA_CONSTANT_CHECK(equal(
            transform(tuple_t<x1, x2, x3, x4>, template_<F>),
            tuple_t<F<x1>, F<x2>, F<x3>, F<x4>>
        ));

        // laws
        auto eq_values = make<Tuple>(eq<0>{}, eq<2>{});
        test::TestFunctor<Tuple>{eq_tuples, eq_values};
    }

    //////////////////////////////////////////////////////////////////////////
    // Applicative
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestApplicative<Tuple>{};
    }

#elif BOOST_HANA_TEST_PART == 8
    //////////////////////////////////////////////////////////////////////////
    // Monad
    //////////////////////////////////////////////////////////////////////////
    {
        test::TestMonad<Tuple>{eq_tuples, nested_eqs};
    }

#elif BOOST_HANA_TEST_PART == 9
    //////////////////////////////////////////////////////////////////////////
    // MonadPlus
    //////////////////////////////////////////////////////////////////////////
    {
        // prepend
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(long_<0>, tuple_c<long>),
                tuple_c<long, 0>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(uint<0>, tuple_c<unsigned int, 1>),
                tuple_c<unsigned int, 0, 1>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(llong<0>, tuple_c<long long, 1, 2>),
                tuple_c<long long, 0, 1, 2>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                prepend(ulong<0>, tuple_c<unsigned long, 1, 2, 3>),
                tuple_c<unsigned long, 0, 1, 2, 3>
            ));
        }

        // empty
        {
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<Tuple>(),
                tuple_c<int>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<Tuple>(),
                tuple_c<long>
            ));
            BOOST_HANA_CONSTANT_CHECK(equal(
                empty<Tuple>(),
                tuple_c<void>
            ));
        }

        // laws
        test::TestMonadPlus<Tuple>{eq_tuples};
    }
#endif
}
