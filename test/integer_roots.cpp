// Copyright 2016-2018 Francesco Biscani (bluescarni@gmail.com)
//
// This file is part of the mp++ library.
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <cstddef>
#include <gmp.h>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

#include <mp++/integer.hpp>

#include "test_utils.hpp"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

static int ntries = 1000;

using namespace mppp;
using namespace mppp_test;

using sizes = std::tuple<std::integral_constant<std::size_t, 1>, std::integral_constant<std::size_t, 2>,
                         std::integral_constant<std::size_t, 3>, std::integral_constant<std::size_t, 6>,
                         std::integral_constant<std::size_t, 10>>;

static std::mt19937 rng;

struct sqrt_tester {
    template <typename S>
    inline void operator()(const S &) const
    {
        using integer = integer<S::value>;
        // Start with all zeroes.
        mpz_raii m1, m2;
        integer n1, n2;
        ::mpz_sqrt(&m1.m_mpz, &m2.m_mpz);
        REQUIRE(&sqrt(n1, n2) == &n1);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE(n1.is_static());
        REQUIRE((lex_cast(sqrt(n2)) == lex_cast(m1)));
        REQUIRE(sqrt(n2).is_static());
        n2.sqrt();
        REQUIRE((lex_cast(n2) == lex_cast(m1)));
        REQUIRE(n2.is_static());
        // Try one.
        n2 = integer{1};
        ::mpz_set_ui(&m2.m_mpz, 1u);
        ::mpz_sqrt(&m1.m_mpz, &m2.m_mpz);
        sqrt(n1, n2);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE(n1.is_static());
        REQUIRE((lex_cast(sqrt(n2)) == lex_cast(m1)));
        REQUIRE(sqrt(n2).is_static());
        n2.sqrt();
        REQUIRE((lex_cast(n2) == lex_cast(m1)));
        REQUIRE(n2.is_static());
        // Two.
        n2 = integer{2};
        ::mpz_set_ui(&m2.m_mpz, 2u);
        ::mpz_sqrt(&m1.m_mpz, &m2.m_mpz);
        sqrt(n1, n2);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE(n1.is_static());
        REQUIRE((lex_cast(sqrt(n2)) == lex_cast(m1)));
        REQUIRE(sqrt(n2).is_static());
        n2.sqrt();
        REQUIRE((lex_cast(n2) == lex_cast(m1)));
        REQUIRE(n2.is_static());
        // Four.
        n2 = integer{4};
        ::mpz_set_ui(&m2.m_mpz, 4u);
        ::mpz_sqrt(&m1.m_mpz, &m2.m_mpz);
        sqrt(n1, n2);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE(n1.is_static());
        REQUIRE((lex_cast(sqrt(n2)) == lex_cast(m1)));
        REQUIRE(sqrt(n2).is_static());
        n2.sqrt();
        REQUIRE((lex_cast(n2) == lex_cast(m1)));
        REQUIRE(n2.is_static());
        // Ten.
        n2 = integer{10};
        ::mpz_set_ui(&m2.m_mpz, 10u);
        ::mpz_sqrt(&m1.m_mpz, &m2.m_mpz);
        sqrt(n1, n2);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE(n1.is_static());
        REQUIRE((lex_cast(sqrt(n2)) == lex_cast(m1)));
        REQUIRE(sqrt(n2).is_static());
        n2.sqrt();
        REQUIRE((lex_cast(n2) == lex_cast(m1)));
        REQUIRE(n2.is_static());
        // Error testing.
        n2 = integer{-1};
        REQUIRE_THROWS_PREDICATE(sqrt(n1, n2), std::domain_error, [](const std::domain_error &ex) {
            return std::string(ex.what()) == "Cannot compute the square root of the negative number -1";
        });
        REQUIRE_THROWS_PREDICATE(sqrt(integer{-2}), std::domain_error, [](const std::domain_error &ex) {
            return std::string(ex.what()) == "Cannot compute the square root of the negative number -2";
        });
        n2 = integer{-3};
        REQUIRE_THROWS_PREDICATE(n2.sqrt(), std::domain_error, [](const std::domain_error &ex) {
            return std::string(ex.what()) == "Cannot compute the square root of the negative number -3";
        });
        mpz_raii tmp;
        std::uniform_int_distribution<int> sdist(0, 1);
        // Run a variety of tests with operands with x number of limbs.
        auto random_xy = [&](unsigned x) {
            for (int i = 0; i < ntries; ++i) {
                if (sdist(rng) && sdist(rng) && sdist(rng)) {
                    // Reset rop every once in a while.
                    n1 = integer{};
                }
                random_integer(tmp, x, rng);
                ::mpz_set(&m2.m_mpz, &tmp.m_mpz);
                n2 = integer(mpz_to_str(&tmp.m_mpz));
                if (n2.is_static() && sdist(rng)) {
                    // Promote sometimes, if possible.
                    n2.promote();
                }
                ::mpz_sqrt(&m1.m_mpz, &m2.m_mpz);
                sqrt(n1, n2);
                REQUIRE((lex_cast(n1) == lex_cast(m1)));
                REQUIRE((lex_cast(sqrt(n2)) == lex_cast(m1)));
                n2.sqrt();
                REQUIRE((lex_cast(n2) == lex_cast(m1)));
                // Overlap.
                n2 = integer(mpz_to_str(&m2.m_mpz));
                ::mpz_sqrt(&m2.m_mpz, &m2.m_mpz);
                sqrt(n2, n2);
                REQUIRE((lex_cast(n2) == lex_cast(m2)));
            }
        };

        random_xy(0);
        random_xy(1);
        random_xy(2);
        random_xy(3);
        random_xy(4);
    }
};

TEST_CASE("sqrt")
{
    tuple_for_each(sizes{}, sqrt_tester{});

    // Test proper zeroing of the upper limbs.
    using int_t = integer<2>;
    int_t n{1};
    // Fill up both limbs.
    n <<= GMP_NUMB_BITS;
    n += 1;
    // Test with zero.
    sqrt(n, int_t{0});
    REQUIRE(n == 0);
    // Test with nonzero.
    n = 1;
    n <<= GMP_NUMB_BITS;
    n += 1;
    sqrt(n, int_t{3});
    REQUIRE(n == 1);
    // Nonzero, overlapping.
    n = 1;
    n <<= GMP_NUMB_BITS;
    sqrt(n, n);
}

struct sqrtrem_tester {
    template <typename S>
    inline void operator()(const S &) const
    {
        using integer = integer<S::value>;
        // Start with all zeroes.
        mpz_raii m1, m2, m3;
        integer n1, n2, n3;
        ::mpz_sqrtrem(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        sqrtrem(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(n2) == lex_cast(m2)));
        REQUIRE(n1.is_static());
        REQUIRE(n2.is_static());
        // Try one.
        n3 = 1;
        ::mpz_set_ui(&m3.m_mpz, 1u);
        ::mpz_sqrtrem(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        sqrtrem(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(n2) == lex_cast(m2)));
        REQUIRE(n1.is_static());
        REQUIRE(n2.is_static());
        // Two.
        n3 = 2;
        ::mpz_set_ui(&m3.m_mpz, 2u);
        ::mpz_sqrtrem(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        sqrtrem(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(n2) == lex_cast(m2)));
        REQUIRE(n1.is_static());
        REQUIRE(n2.is_static());
        // Four.
        n3 = 4;
        ::mpz_set_ui(&m3.m_mpz, 4u);
        ::mpz_sqrtrem(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        sqrtrem(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(n2) == lex_cast(m2)));
        REQUIRE(n1.is_static());
        REQUIRE(n2.is_static());
        // Ten.
        n3 = 10;
        ::mpz_set_ui(&m3.m_mpz, 10u);
        ::mpz_sqrtrem(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
        sqrtrem(n1, n2, n3);
        REQUIRE((lex_cast(n1) == lex_cast(m1)));
        REQUIRE((lex_cast(n2) == lex_cast(m2)));
        REQUIRE(n1.is_static());
        REQUIRE(n2.is_static());
        // Error testing.
        n3 = -1;
        REQUIRE_THROWS_PREDICATE(sqrtrem(n1, n2, n3), std::domain_error, [](const std::domain_error &ex) {
            return std::string(ex.what()) == "Cannot compute the square root with remainder of the negative number -1";
        });
        REQUIRE_THROWS_PREDICATE(sqrtrem(n1, n2, integer{-2}), std::domain_error, [](const std::domain_error &ex) {
            return std::string(ex.what()) == "Cannot compute the square root with remainder of the negative number -2";
        });
        REQUIRE_THROWS_PREDICATE(
            sqrtrem(n1, n1, integer{2}), std::invalid_argument, [](const std::invalid_argument &ex) {
                return std::string(ex.what())
                       == "When performing an integer square root with remainder, the result 'rop' and the "
                          "remainder 'rem' must be distinct objects";
            });
        mpz_raii tmp;
        std::uniform_int_distribution<int> sdist(0, 1);
        // Run a variety of tests with operands with x number of limbs.
        auto random_xy = [&](unsigned x) {
            for (int i = 0; i < ntries; ++i) {
                // Reset rops every once in a while.
                if (sdist(rng) && sdist(rng) && sdist(rng)) {
                    n1 = integer{};
                }
                if (sdist(rng) && sdist(rng) && sdist(rng)) {
                    n2 = integer{};
                }
                random_integer(tmp, x, rng);
                ::mpz_set(&m3.m_mpz, &tmp.m_mpz);
                n3 = integer(mpz_to_str(&tmp.m_mpz));
                if (n3.is_static() && sdist(rng)) {
                    // Promote sometimes, if possible.
                    n3.promote();
                }
                ::mpz_sqrtrem(&m1.m_mpz, &m2.m_mpz, &m3.m_mpz);
                sqrtrem(n1, n2, n3);
                REQUIRE((lex_cast(n1) == lex_cast(m1)));
                REQUIRE((lex_cast(n2) == lex_cast(m2)));
                // Argument overlaps with rop.
                ::mpz_sqrtrem(&m3.m_mpz, &m2.m_mpz, &m3.m_mpz);
                sqrtrem(n3, n2, n3);
                REQUIRE((lex_cast(n2) == lex_cast(m2)));
                REQUIRE((lex_cast(n3) == lex_cast(m3)));
                // Argument overlaps with rem.
                ::mpz_set(&m3.m_mpz, &tmp.m_mpz);
                n3 = integer(mpz_to_str(&tmp.m_mpz));
                ::mpz_sqrtrem(&m1.m_mpz, &m3.m_mpz, &m3.m_mpz);
                sqrtrem(n1, n3, n3);
                REQUIRE((lex_cast(n1) == lex_cast(m1)));
                REQUIRE((lex_cast(n3) == lex_cast(m3)));
            }
        };

        random_xy(0);
        random_xy(1);
        random_xy(2);
        random_xy(3);
        random_xy(4);
    }
};

TEST_CASE("sqrtrem")
{
    tuple_for_each(sizes{}, sqrtrem_tester{});

    // Test proper zeroing of the upper limbs.
    using int_t = integer<2>;
    int_t n1{1}, n2;
    // Fill up both limbs.
    n1 <<= GMP_NUMB_BITS;
    n1 += 1;
    n2 = n1;
    // Test with zero.
    sqrtrem(n1, n2, int_t{0});
    REQUIRE(n1 == 0);
    REQUIRE(n2 == 0);
    // Test with nonzero.
    n1 = 1;
    n1 <<= GMP_NUMB_BITS;
    n1 += 1;
    n2 = n1;
    sqrtrem(n1, n2, int_t{3});
    REQUIRE(n1 == 1);
    REQUIRE(n2 == 2);
    // Nonzero, overlapping.
    n1 = 1;
    n1 <<= GMP_NUMB_BITS;
    n2 = n1;
    sqrtrem(n1, n2, n1);
    n1 = 1;
    n1 <<= GMP_NUMB_BITS;
    n2 = n1;
    sqrtrem(n1, n2, n2);
}
