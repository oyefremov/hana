/*!
@file
Forward declares `boost::hana::Foldable`.

@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_FWD_FOLDABLE_HPP
#define BOOST_HANA_FWD_FOLDABLE_HPP

#include <boost/hana/config.hpp>
#include <boost/hana/detail/std/forward.hpp>
#include <boost/hana/functional/curry.hpp>
#include <boost/hana/functional/flip.hpp>
#include <boost/hana/fwd/core/datatype.hpp>
#include <boost/hana/fwd/core/models.hpp>
#include <boost/hana/fwd/monad.hpp>


namespace boost { namespace hana {
    //! @ingroup group-concepts
    //! The `Foldable` concept represents data structures that can be folded,
    //! i.e. summarized into a single value.
    //!
    //! Another way of seeing `Foldable`s is as data structures supporting
    //! internal iteration with the ability to accumulate a result. Also
    //! note that since C++ only supports eager evaluation, all models of
    //! `Foldable` must represent finite data structures.
    //!
    //! Additionally, only structures whose total size is known at
    //! compile-time can be models of `Foldable`. This is because of
    //! the `unpack` method, whose return _type_ depends on the number
    //! and types of the objects in the structure.
    //!
    //! @note
    //! While the fact that `Foldable` only works for finite structures may
    //! seem overly restrictive in comparison to the Haskell definition of
    //! `Foldable`, a finer grained separation of the concepts (see `Iterable`
    //! and `Searchable`) should mitigate the issue.
    //!
    //!
    //! Laws
    //! ----
    //! For any `Foldable` `xs`, the following laws must be satisfied:
    //! @code
    //!     foldl(xs, s, f) == foldl(to<Tuple>(xs), s, f)
    //!     foldr(xs, s, f) == foldr(to<Tuple>(xs), s, f)
    //! @endcode
    //!
    //! where `s` and `f` are an arbitrary state and function that can be
    //! used for folding, respectively. Intuitively, these laws say that
    //! `Foldable` respects the left-to-right order of elements within a
    //! structure.
    //!
    //!
    //! Minimal complete definition
    //! ---------------------------
    //! (`foldl` and `foldr`) or `unpack`
    //!
    //!
    //! Provided models
    //! ---------------
    //! 1. For builtin arrays\n
    //! Builtin arrays whose size is known can be folded as-if they were
    //! homogeneous tuples. However, note that builtin arrays can't be
    //! made more than `Foldable` (e.g. `Iterable`) because they can't
    //! be empty and they also can't be returned from functions.
    //!
    //!
    //! @todo
    //! Make it possible to specify the Monoid used for `sum` and `product`?
    struct Foldable { };

    //! Left-associative fold of a structure using a binary operation.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure to fold.
    //!
    //! @param state
    //! The initial value used for folding.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the
    //! result accumulated so far and `x` is an element in the structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp foldl
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/foldl.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto foldl = [](auto&& foldable, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct foldl_impl;

    struct _foldl {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::foldl(xs, state, f) requires xs to be Foldable");
#endif

            return foldl_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _foldl foldl{};
#endif

    //! Monadic left fold over the elements of a structure.
    //! @relates Foldable
    //!
    //!
    //! @tparam M
    //! A Monad corresponding to the data type of `f`'s result.
    //!
    //! @param foldable
    //! The structure to fold.
    //!
    //! @param state
    //! The initial value used for folding. If the structure is empty, this
    //! value is lifted in to the `M` Monad and then returned as-is.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the result
    //! accumulated so far and `x` is an element in the structure. The
    //! function must return its result inside the `M` Monad.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.foldlM.cpp foldlM
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto foldlM = [](auto&& foldable, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct foldlM_impl;

    template <typename M>
    struct _foldlM {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(_models<Monad, M>{},
        "hana::foldlM<M>(xs, state, f) requires M to be a Monad");
#endif
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::foldlM<M>(xs, state, f) requires xs to be Foldable");
#endif
            return foldlM_impl<typename datatype<Xs>::type>::template apply<M>(
                detail::std::forward<Xs>(xs),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    template <typename M>
    constexpr _foldlM<M> foldlM{};
#endif

    //! Right-associative fold of a structure using a binary operation.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure to fold.
    //!
    //! @param state
    //! The initial value used for folding.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the
    //! result accumulated so far and `x` is an element in the structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp foldr
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/foldr.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto foldr = [](auto&& foldable, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct foldr_impl;

    struct _foldr {
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::foldr(xs, state, f) requires xs to be Foldable");
#endif

            return foldr_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _foldr foldr{};
#endif

    //! Monadic right fold over the elements of a structure.
    //! @relates Foldable
    //!
    //!
    //! @tparam M
    //! A Monad corresponding to the data type of `f`'s result.
    //!
    //! @param foldable
    //! The structure to fold.
    //!
    //! @param state
    //! The initial value used for folding. If the structure is empty, this
    //! value is lifted in to the `M` Monad and then returned as-is.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the result
    //! accumulated so far and `x` is an element in the structure. The
    //! function must return its result inside the `M` Monad.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp foldrM
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    template <typename M>
    constexpr auto foldrM = [](auto&& foldable, auto&& state, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct foldrM_impl;

    template <typename M>
    struct _foldrM {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
        static_assert(_models<Monad, M>{},
        "hana::foldrM<M>(xs, state, f) requires M to be a Monad");
#endif
        template <typename Xs, typename State, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, State&& state, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::foldrM<M>(xs, state, f) requires xs to be Foldable");
#endif
            return foldrM_impl<typename datatype<Xs>::type>::template apply<M>(
                detail::std::forward<Xs>(xs),
                detail::std::forward<State>(state),
                detail::std::forward<F>(f)
            );
        }
    };

    template <typename M>
    constexpr _foldrM<M> foldrM{};
#endif

    //! Variant of `foldr` that has no base case, and thus may only be
    //! applied to non-empty structures.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure to fold.
    //!
    //! @param f
    //! A binary function called as `f(x, state)`, where `state` is the
    //! result accumulated so far and `x` is an element in the structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp foldr1
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/foldr1.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto foldr1 = [](auto&& foldable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct foldr1_impl;

    struct _foldr1 {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::foldr1(xs, f) requires xs to be Foldable");
#endif
            return foldr1_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _foldr1 foldr1{};
#endif

    //! Variant of `foldl` that has no base case, and thus may only be
    //! applied to non-empty structures.
    //! @relates Foldable
    //!
    //!
    //! @param foldable
    //! The structure to fold.
    //!
    //! @param f
    //! A binary function called as `f(state, x)`, where `state` is the
    //! result accumulated so far and `x` is an element in the structure.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp foldl1
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/foldl1.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto foldl1 = [](auto&& foldable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct foldl1_impl;

    struct _foldl1 {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::foldl1(xs, f) requires xs to be Foldable");
#endif
            return foldl1_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _foldl1 foldl1{};
#endif

    //! Equivalent to `fold` in Boost.Fusion and Boost.MPL.
    //! @relates Foldable
    //!
    //! This method has the same semantics as `fold` in Boost.Fusion and
    //! Boost.MPL, with the extension that an initial state is not required.
    //! Hence, this method can be called in two different ways. When called
    //! with three arguments, `fold` is just equivalent to `foldl`.
    //! In other words,
    //! @code
    //!     fold(sequence, state, f) == foldl(sequence, state, f)
    //! @endcode
    //!
    //! When called with only two arguments, `fold` is equivalent to `foldl1`.
    //! In this case, `fold` assumes that the two arguments consist of a
    //! Foldable and a function. In other words,
    //! @code
    //!     fold(sequence, f) == foldl1(sequence, f)
    //! @endcode
    //!
    //! @note
    //! This method is a convenience alias to `foldl` or `foldl1`. As an
    //! alias, `fold` is not tag-dispatched on its own and `foldl`/`foldl1`
    //! should be customized instead.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp fold
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto fold = [](auto&& foldable[, auto&& state], auto&& f) -> decltype(auto) {
        return see-documentation;
    };
#else
    struct _fold {
        template <typename Xs, typename S, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, S&& s, F&& f) const {
            return hana::foldl(detail::std::forward<Xs>(xs),
                               detail::std::forward<S>(s),
                               detail::std::forward<F>(f));
        }

        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return hana::foldl1(detail::std::forward<Xs>(xs),
                                detail::std::forward<F>(f));
        }
    };

    constexpr _fold fold{};
#endif

    //! Equivalent to `reverse_fold` in Boost.Fusion and Boost.MPL.
    //! @relates Foldable
    //!
    //! This method has the same semantics as `reverse_fold` in Boost.Fusion
    //! and Boost.MPL, with the extension that an initial state is not
    //! required. Hence, this method can be called in two different ways.
    //! When called with three arguments, `reverse_fold` is equivalent to
    //! `foldr`, except that the accumulating function should expect its
    //! arguments in reverse order (to match the order used in Fusion).
    //! In other words,
    //! @code
    //!     reverse_fold(sequence, state, f) == foldr(sequence, state, flip(f))
    //! @endcode
    //!
    //! When called with only two arguments, `reverse_fold` is equivalent to
    //! `foldr1`, except that the accumulating function should expect its
    //! arguments in reverse order (to match the order used in Fusion).
    //! In this case, `reverse_fold` assumes that the two arguments consist
    //! of a Foldable and a function. In other words,
    //! @code
    //!     reverse_fold(sequence, f) == foldr1(sequence, flip(f))
    //! @endcode
    //!
    //!
    //! @note
    //! This method is a convenience alias to `foldr` or `foldr1`. As an
    //! alias, `reverse_fold` is not tag-dispatched on its own and
    //! `foldr`/`foldr1` should be customized instead.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp reverse_fold
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto reverse_fold = [](auto&& foldable[, auto&& state], auto&& f) -> decltype(auto) {
        return see-documentation;
    };
#else
    struct _reverse_fold {
        template <typename Xs, typename S, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, S&& s, F&& f) const {
            return hana::foldr(detail::std::forward<Xs>(xs),
                               detail::std::forward<S>(s),
                               hana::flip(detail::std::forward<F>(f)));
        }

        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
            return hana::foldr1(detail::std::forward<Xs>(xs),
                                hana::flip(detail::std::forward<F>(f)));
        }
    };

    constexpr _reverse_fold reverse_fold{};
#endif

    //! Perform an action on each element of a foldable, discarding
    //! the result each time.
    //! @relates Foldable
    //!
    //! Iteration is done from left to right, i.e. in the same order as when
    //! using `foldl`. If the structure is not finite, this method will not
    //! terminate.
    //!
    //!
    //! @param foldable
    //! The structure to iterate over.
    //!
    //! @param f
    //! A function called as `f(x)` for each element `x` of the structure.
    //! The result of `f(x)`, whatever it is, is ignored.
    //!
    //!
    //! @todo
    //! The presence of implicit side effects in this function might be a
    //! smell that it should be moved to a different concept and handled
    //! through `Monad`s.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp for_each
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/for_each.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto for_each = [](auto&& foldable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct for_each_impl;

    struct _for_each {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::for_each(xs, f) requires xs to be Foldable");
#endif
            return for_each_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _for_each for_each{};
#endif

    //! Return the number of elements in a finite structure.
    //! @relates Foldable
    //!
    //! Specifically, returns an object of an unsigned integral type, or
    //! a `Constant` holding such an object, which represents the number
    //! of elements in the structure. The structure has to be finite for
    //! this method to return.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp length
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/length.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto length = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct length_impl;

    struct _length {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::length(xs) requires xs to be Foldable");
#endif
            return length_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _length length{};
#endif

    //! Equivalent to `length`; provided for consistency with the
    //! standard library.
    //! @relates Foldable
    //!
    //! This method is an alias to `length` provided for convenience and
    //! consistency with the standard library. As an alias, `size` is not
    //! tag-dispatched on its own and `length` should be customized instead.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp size
    constexpr auto size = length;

    //! Return the least element of a non-empty structure with respect to
    //! a `predicate`.
    //! @relates Foldable
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x, y)`, where `x` and `y` are elements
    //! of the structure. `predicate` should be a strict weak ordering on the
    //! elements of the structure and its return value should be a `Logical`.
    //!
    //! @param foldable
    //! The structure to find the least element of.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp minimum_by
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/minimum_by.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto minimum_by = [](auto&& predicate, auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct minimum_by_impl;

    struct _minimum_by {
        template <typename Pred, typename Xs>
        constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::minimum_by(pred, xs) requires xs to be Foldable");
#endif
            return minimum_by_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Pred>(pred),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _minimum_by minimum_by{};
#endif

    //! Return the least element of a non-empty structure.
    //! @relates Foldable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp minimum
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/minimum.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto minimum = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct minimum_impl;

    struct _minimum {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::minimum(xs) requires xs to be Foldable");
#endif

            return minimum_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _minimum minimum{};
#endif

    //! Return the greatest element of a non-empty structure with respect to
    //! a `predicate`.
    //! @relates Foldable
    //!
    //!
    //! @param predicate
    //! A function called as `predicate(x, y)`, where `x` and `y` are elements
    //! of the structure. `predicate` should be a strict weak ordering on the
    //! elements of the structure and its return value should be a `Logical`.
    //!
    //! @param foldable
    //! The structure to find the greatest element of.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp maximum_by
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/maximum_by.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto maximum_by = [](auto&& predicate, auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct maximum_by_impl;

    struct _maximum_by {
        template <typename Pred, typename Xs>
        constexpr decltype(auto) operator()(Pred&& pred, Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::maximum_by(pred, xs) requires xs to be Foldable");
#endif

            return maximum_by_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Pred>(pred),
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _maximum_by maximum_by{};
#endif

    //! Return the greatest element of a non-empty structure.
    //! @relates Foldable
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp maximum
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/maximum.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto maximum = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct maximum_impl;

    struct _maximum {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::maximum(xs) requires xs to be Foldable");
#endif

            return maximum_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _maximum maximum{};
#endif

    //! Compute the sum of the numbers of a structure.
    //! @relates Foldable
    //!
    //! It must be possible to perform `plus` on any two adjacent elements of
    //! the structure. The meaning of "adjacent" as used here is that two
    //! elements of the structure `x` and `y` are adjacent if and only if
    //! they are adjacent in `to<Tuple>(foldable)`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp sum
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/sum.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto sum = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct sum_impl;

    struct _sum {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::sum(xs) requires xs to be Foldable");
#endif

            return sum_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _sum sum{};
#endif

    //! Compute the product of the numbers of a structure.
    //! @relates Foldable
    //!
    //! It must be possible to perform `mult` on any two adjacent elements of
    //! the structure. The meaning of "adjacent" as used here is that two
    //! elements of the structure `x` and `y` are adjacent if and only if
    //! they are adjacent in `to<Tuple>(foldable)`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp product
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/product.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto product = [](auto&& foldable) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct product_impl;

    struct _product {
        template <typename Xs>
        constexpr decltype(auto) operator()(Xs&& xs) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::product(xs) requires xs to be Foldable");
#endif
            return product_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs)
            );
        }
    };

    constexpr _product product{};
#endif

    //! Return the number of elements in the structure for which the
    //! `predicate` is satisfied.
    //! @relates Foldable
    //!
    //! Specifically, returns an object of an unsigned integral type, or
    //! a `Constant` holding such an object, which represents the number
    //! of elements in the structure satisfying the given `predicate`.
    //!
    //!
    //! @param foldable
    //! The structure whose elements are counted.
    //!
    //! @param predicate
    //! A function called as `predicate(x)`, where `x` is an element of the
    //! structure, and returning a `Logical` representing whether `x` should
    //! be counted.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp count_if
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/count_if.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto count_if = [](auto&& foldable, auto&& predicate) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct count_if_impl;

    struct _count_if {
        template <typename Xs, typename Pred>
        constexpr decltype(auto) operator()(Xs&& xs, Pred&& pred) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::count_if(xs, pred) requires xs to be Foldable");
#endif
            return count_if_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Pred>(pred)
            );
        }
    };

    constexpr _count_if count_if{};
#endif

    //! Return the number of elements in the structure that compare equal to
    //! a given value.
    //! @relates Foldable
    //!
    //! Given a Foldable structure `foldable` and a value `value`, `count`
    //! returns an unsigned integral, or a Constant thereof, representing the
    //! number of elements of `foldable` that compare equal to `value`. For
    //! this method to be well-defined, all the elements of the structure must
    //! be Comparable with the given value.
    //!
    //!
    //! @param foldable
    //! The structure whose elements are counted.
    //!
    //! @param value
    //! A value compared with each element in the structure. Elements
    //! that compare equal to this value are counted, others are not.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp count
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto count = [](auto&& foldable, auto&& value) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct count_impl;

    struct _count {
        template <typename Xs, typename Value>
        constexpr decltype(auto) operator()(Xs&& xs, Value&& value) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::count(xs, value) requires xs to be Foldable");
#endif
            return count_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<Value>(value)
            );
        }
    };

    constexpr _count count{};
#endif

    //! Invoke a function with the elements of a structure as arguments.
    //! @relates Foldable
    //!
    //! Given a function and a foldable structure whose length can be known at
    //! compile-time, `unpack` invokes the function with the contents of that
    //! structure. In other words, `unpack(foldable, f)` is equivalent to
    //! `f(x...)`, where `x...` are the elements of the structure. The length
    //! of the structure must be known at compile-time, because the version of
    //! `f`'s `operator()` that will be compiled depends on the number of
    //! arguments it is called with, which has to be known at compile-time.
    //!
    //! To create a function that accepts a foldable instead of variadic
    //! arguments, see `fuse`, which is roughly equivalent to
    //! `boost::fusion::make_fused`.
    //!
    //!
    //! @param foldable
    //! The structure to expand into the function.
    //!
    //! @param f
    //! A function to be invoked as `f(x...)`, where `x...` are the elements
    //! of the structure as-if they had been linearized with `to<Tuple>`.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp unpack
    //!
    //!
    //! Benchmarks
    //! ----------
    //! @image html benchmark/foldable/unpack.ctime.png
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto unpack = [](auto&& foldable, auto&& f) -> decltype(auto) {
        return tag-dispatched;
    };
#else
    template <typename Xs, typename = void>
    struct unpack_impl;

    struct _unpack {
        template <typename Xs, typename F>
        constexpr decltype(auto) operator()(Xs&& xs, F&& f) const {
#ifdef BOOST_HANA_CONFIG_CHECK_DATA_TYPES
            static_assert(_models<Foldable, typename datatype<Xs>::type>{},
            "hana::unpack(xs, f) requires xs to be Foldable");
#endif
            return unpack_impl<typename datatype<Xs>::type>::apply(
                detail::std::forward<Xs>(xs),
                detail::std::forward<F>(f)
            );
        }
    };

    constexpr _unpack unpack{};
#endif

    //! Transform a function taking multiple arguments into a function that
    //! can be called with a `Foldable`.
    //! @relates Foldable
    //!
    //!
    //! This function is provided for convenience as a different way of
    //! calling `unpack`. Specifically, `fuse(f)` is a function such that
    //! @code
    //!     fuse(f)(foldable) == unpack(foldable, f)
    //!                       == f(x...)
    //! @endcode
    //! where `x...` are the elements in the foldable. This function is
    //! useful when one wants to create a function that accepts a foldable
    //! which is not known yet.
    //!
    //! @note
    //! This function is not tag-dispatched and so it can not be customized.
    //!
    //!
    //! Example
    //! -------
    //! @snippet example/foldable.cpp fuse
#ifdef BOOST_HANA_DOXYGEN_INVOKED
    constexpr auto fuse = [](auto&& f) -> decltype(auto) {
        return [perfect-capture](auto&& xs) -> decltype(auto) {
            return unpack(forwarded(xs), forwarded(f));
        };
    };
#else
    constexpr auto fuse = curry<2>(flip(unpack));
#endif
}} // end namespace boost::hana

#endif // !BOOST_HANA_FWD_FOLDABLE_HPP
