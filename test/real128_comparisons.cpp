// Copyright 2016-2017 Francesco Biscani (bluescarni@gmail.com)
//
// This file is part of the mp++ library.
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <string>
#include <type_traits>
#include <utility>

#include <mp++/mp++.hpp>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace mppp;

using int_t = integer<1>;
using rat_t = rational<1>;

template <typename T, typename U>
using equal_t = decltype(std::declval<const T &>() == std::declval<const U &>());

template <typename T, typename U>
using inequal_t = decltype(std::declval<const T &>() != std::declval<const U &>());

template <typename T, typename U>
using is_eq_comparable = std::integral_constant<bool, std::is_same<detected_t<equal_t, T, U>, bool>::value>;

template <typename T, typename U>
using is_ineq_comparable = std::integral_constant<bool, std::is_same<detected_t<inequal_t, T, U>, bool>::value>;

TEST_CASE("real128 equality")
{
    REQUIRE((std::is_same<decltype(real128{} == real128{}), bool>::value));
    REQUIRE((std::is_same<decltype(real128{} != real128{}), bool>::value));
    REQUIRE((is_eq_comparable<real128, real128>::value));
    REQUIRE((!is_eq_comparable<real128, std::string>::value));
    REQUIRE((!is_eq_comparable<std::string, real128>::value));
    REQUIRE((is_ineq_comparable<real128, real128>::value));
    REQUIRE((!is_ineq_comparable<real128, std::string>::value));
    REQUIRE((!is_ineq_comparable<std::string, real128>::value));
    constexpr bool c0 = real128{} == real128{};
    constexpr bool c0a = real128{} != real128{};
    constexpr bool c0b = real128_equal_to(real128{}, real128{});
    REQUIRE(c0);
    REQUIRE(!c0a);
    REQUIRE(c0b);
    constexpr bool c1 = real128{-1} == real128{1};
    constexpr bool c1a = real128{-1} != real128{1};
    constexpr bool c1b = real128_equal_to(real128{-1}, real128{1});
    REQUIRE(!c1);
    REQUIRE(c1a);
    REQUIRE(!c1b);
    constexpr bool c2 = real128{-1} == -1;
    constexpr bool c2a = real128{-1} != -1;
    REQUIRE(c2);
    REQUIRE(!c2a);
    constexpr bool c3 = 1.23 == real128{-1};
    constexpr bool c3a = 1.23 != real128{-1};
    REQUIRE(!c3);
    REQUIRE(c3a);
    REQUIRE(real128{10} == int_t{10});
    REQUIRE(int_t{10} == real128{10});
    REQUIRE(real128{10} != int_t{-10});
    REQUIRE(int_t{-10} != real128{10});
    REQUIRE((real128{"1.5"} == rat_t{3, 2}));
    REQUIRE((rat_t{3, 2} == real128{"1.5"}));
    REQUIRE((real128{"1.5"} != rat_t{3, 5}));
    REQUIRE((rat_t{-3, 2} != real128{"1.5"}));
    REQUIRE(real128_inf() == real128_inf());
    REQUIRE(-real128_inf() != real128_inf());
    REQUIRE(real128_nan() != real128_nan());
    REQUIRE(-real128_nan() != -real128_nan());
    REQUIRE(!(real128_nan() == real128_nan()));
    REQUIRE(!(-real128_nan() == -real128_nan()));
    REQUIRE(real128_equal_to(real128_inf(), real128_inf()));
    REQUIRE(!real128_equal_to(-real128_inf(), real128_inf()));
    REQUIRE(real128_equal_to(real128_nan(), real128_nan()));
    REQUIRE(real128_equal_to(real128_nan(), -real128_nan()));
    REQUIRE(real128_equal_to(-real128_nan(), real128_nan()));
    REQUIRE(!real128_equal_to(real128{-1}, real128_nan()));
    REQUIRE(!real128_equal_to(real128_nan(), real128{-1}));
}

template <typename T, typename U>
using lt_t = decltype(std::declval<const T &>() == std::declval<const U &>());

template <typename T, typename U>
using is_lt_comparable = std::integral_constant<bool, std::is_same<detected_t<lt_t, T, U>, bool>::value>;

TEST_CASE("real128 less_than")
{
    REQUIRE((std::is_same<decltype(real128{} < real128{}), bool>::value));
    REQUIRE((is_lt_comparable<real128, real128>::value));
    REQUIRE((!is_lt_comparable<real128, std::string>::value));
    REQUIRE((!is_lt_comparable<std::string, real128>::value));
    constexpr bool c0 = real128{} < real128{};
    constexpr bool c0b = real128_less_than(real128{}, real128{});
    REQUIRE(!c0);
    REQUIRE(!c0b);
    constexpr bool c1 = real128{-1} < real128{1};
    constexpr bool c1b = real128_less_than(real128{-1}, real128{1});
    REQUIRE(c1);
    REQUIRE(c1b);
    constexpr bool c2 = real128{1} < -1;
    constexpr bool c2a = real128{1} < -1;
    REQUIRE(!c2);
    REQUIRE(!c2a);
    constexpr bool c3 = 1.23 < real128{-1};
    constexpr bool c3a = 1.23 < real128{-1};
    REQUIRE(!c3);
    REQUIRE(!c3a);
    REQUIRE(!(real128{10} < int_t{10}));
    REQUIRE(!(int_t{10} < real128{10}));
    REQUIRE(!(real128{10} < int_t{-10}));
    REQUIRE(int_t{-10} < real128{10});
    REQUIRE(!(real128{"2"} < rat_t{3, 2}));
    REQUIRE((rat_t{3, 2} < real128{"2"}));
    REQUIRE(!(real128{"1.5"} < rat_t{3, 5}));
    REQUIRE((rat_t{-3, 2} < real128{"1.5"}));
    REQUIRE(!(real128_inf() < real128_inf()));
    REQUIRE(-real128_inf() < real128_inf());
    REQUIRE(!(real128_inf() < -real128_inf()));
    REQUIRE(!(real128_nan() < real128_nan()));
    REQUIRE(!(-real128_nan() < -real128_nan()));
    REQUIRE(!(real128_nan() < real128_nan()));
    REQUIRE(!(-real128_nan() < -real128_nan()));
    REQUIRE(!real128_less_than(real128_inf(), real128_inf()));
    REQUIRE(real128_less_than(-real128_inf(), real128_inf()));
    REQUIRE(!real128_less_than(real128_nan(), real128_nan()));
    REQUIRE(!real128_less_than(real128_nan(), -real128_nan()));
    REQUIRE(!real128_less_than(-real128_nan(), real128_nan()));
    REQUIRE(real128_less_than(-real128_inf(), real128_nan()));
    REQUIRE(real128_less_than(real128{-1}, real128_nan()));
    REQUIRE(real128_less_than(real128{100}, real128_nan()));
    REQUIRE(real128_less_than(real128_inf(), real128_nan()));
    REQUIRE(!real128_less_than(real128_nan(), -real128_inf()));
    REQUIRE(!real128_less_than(real128_nan(), real128{-1}));
    REQUIRE(!real128_less_than(real128_nan(), real128{100}));
    REQUIRE(!real128_less_than(real128_nan(), real128_inf()));
}
