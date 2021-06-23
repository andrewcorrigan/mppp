#!/usr/bin/env bash

# Echo each command
set -x

# Exit on error.
set -e

# Core deps.
sudo apt-get install build-essential cmake clang libgmp-dev libmpfr-dev libmpc-dev libboost-dev libboost-serialization-dev

# Create the build dir and cd into it.
mkdir build
cd build

# Compile mppp and run the tests.
CC=clang CXX=clang++ cmake ../ -DCMAKE_BUILD_TYPE=Debug -DMPPP_WITH_QUADMATH=yes -DBoost_NO_BOOST_CMAKE=ON -DMPPP_WITH_BOOST_S11N=yes -DMPPP_WITH_MPFR=yes -DMPPP_WITH_MPC=yes -DMPPP_BUILD_TESTS=yes -DMPPP_QUADMATH_INCLUDE_DIR=/usr/lib/gcc/x86_64-linux-gnu/7/include/ -DMPPP_QUADMATH_LIBRARY=/usr/lib/gcc/x86_64-linux-gnu/7/libquadmath.so -DMPPP_ENABLE_IPO=yes
make VERBOSE=1
ctest -V

CC=clang CXX=clang++ cmake ../ -DCMAKE_BUILD_TYPE=Debug -DMPPP_WITH_QUADMATH=yes -DBoost_NO_BOOST_CMAKE=ON -DMPPP_WITH_BOOST_S11N=yes -DMPPP_WITH_MPFR=no -DMPPP_WITH_MPC=no -DMPPP_BUILD_TESTS=yes -DMPPP_QUADMATH_INCLUDE_DIR=/usr/lib/gcc/x86_64-linux-gnu/7/include/ -DMPPP_QUADMATH_LIBRARY=/usr/lib/gcc/x86_64-linux-gnu/7/libquadmath.so -DMPPP_ENABLE_IPO=yes
make VERBOSE=1
ctest -V

set +e
set +x
