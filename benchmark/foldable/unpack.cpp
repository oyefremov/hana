/*
@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/foldable/foldable.hpp>

#include <boost/hana/benchmark/measure.hpp>

<%= setup %>


int main() {
    auto foldable = <%= foldable %>;
    auto f = [](auto&& ...xs) { };

    boost::hana::benchmark::measure([=] {
        boost::hana::unpack(foldable, f);
    });
}