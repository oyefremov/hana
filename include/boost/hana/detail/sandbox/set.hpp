/*!
@file
Defines `boost::hana::Set`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_SET_HPP
#define BOOST_HANA_SET_HPP

#include <boost/hana/comparable/equal_mcd.hpp>
#include <boost/hana/detail/constexpr.hpp>
#include <boost/hana/detail/wrap.hpp>
#include <boost/hana/foldable/mcd.hpp>
#include <boost/hana/list/instance.hpp>
#include <boost/hana/logical/logical.hpp>
#include <boost/hana/searchable/find_mcd.hpp>


namespace boost { namespace hana {
    struct Set;

    BOOST_HANA_CONSTEXPR_LAMBDA auto set = [](auto ...elems) {
        return detail::wrap<Set>(list(elems...));
    };

    // a Maybe value
    BOOST_HANA_CONSTEXPR_LAMBDA auto lookup = [](auto elem, auto set) {
        return find([=](auto e) { return equal(elem, e); }, detail::unwrap(set));
    };

    // insert the given element in the set, or do nothing if it is already there
    BOOST_HANA_CONSTEXPR_LAMBDA auto insert = [](auto e, auto set) {
        return eval_if(elem(e, set),
            [=](auto) { return set; },
            [=](auto _) { return detail::wrap<Set>(_(cons)(e, detail::unwrap(set))); }
        );
    };

    template <>
    struct Foldable::instance<Set> : Foldable::mcd {
        template <typename F, typename State, typename Set>
        static constexpr auto foldr_impl(F f, State s, Set set) {
            return foldr(f, s, detail::unwrap(set));
        }

        template <typename F, typename State, typename Set>
        static constexpr auto foldl_impl(F f, State s, Set set) {
            return foldl(f, s, detail::unwrap(set));
        }

        template <typename Set>
        static constexpr auto length_impl(Set set)
        { return length(detail::unwrap(set)); }
    };

    template <>
    struct Searchable::instance<Set> : Searchable::find_mcd {
        template <typename Pred, typename Set>
        static constexpr auto find_impl(Pred pred, Set set)
        { return find(pred, to<List>(set)); }

        template <typename Pred, typename Set>
        static constexpr auto any_impl(Pred pred, Set set)
        { return any(pred, to<List>(set)); }
    };

    template <>
    struct Comparable::instance<Set, Set> : Comparable::equal_mcd {
        template <typename S1, typename S2>
        static constexpr auto equal_impl(S1 s1, S2 s2) {
            return and_(
                equal(length(s1), length(s2)),
                all([=](auto k) { return equal(lookup(k, s1), lookup(k, s2)); }, s1)
            );
        }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_SET_HPP