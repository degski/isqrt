
// MIT License
//
// Copyright (c) 2020 degski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "isqrt.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <array>
#include <iomanip>
#include <limits>
#include <random>
#include <sax/iostream.hpp>
#include <span>
#include <tuple>
#include <type_traits>
#include <vector>

#include <sax/integer.hpp>
#include <sax/iostream.hpp>
#include <sax/splitmix.hpp>
#include <sax/uniform_int_distribution.hpp>

#include <plf/plf_nanotimer.h>

constexpr int64_t size = 1'024 * 4;

inline auto const seed = [] ( ) {
    std::random_device rdev;
    return ( static_cast<std::size_t> ( rdev ( ) ) << 32 ) | static_cast<std::size_t> ( rdev ( ) );
}( );

inline sax::uniform_int_distribution<int64_t> dis_test{ 3, 1'000'000'000 };
inline sax::uniform_int_distribution<std::size_t> dis_idx{ 0, size - 1 };

[[maybe_unused]] bool test_speed_0 ( ) noexcept {
    std::array<int64_t, size> data = { };
    sax::splitmix64 rng{ seed };
    plf::nanotimer t;
    t.start ( );
    for ( int64_t i = 0; i < size * size; ++i )
        data[ dis_idx ( rng ) ] += sax::isqrt_0 ( dis_test ( rng ) );
    uint64_t time = static_cast<uint64_t> ( t.get_elapsed_ns ( ) ) / ( size * size );
    std::cout << "algo 0 " << std::setw ( 4 ) << time << std::setw ( 0 ) << " ns/calc     " << data[ dis_idx ( rng ) ] << nl;
    return true;
}

[[maybe_unused]] bool test_speed_1 ( ) noexcept {
    std::array<int64_t, size> data = { };
    sax::splitmix64 rng{ seed };
    plf::nanotimer t;
    t.start ( );
    for ( int64_t i = 0; i < size * size; ++i )
        data[ dis_idx ( rng ) ] += sax::isqrt_1 ( dis_test ( rng ) );
    uint64_t time = static_cast<uint64_t> ( t.get_elapsed_ns ( ) ) / ( size * size );
    std::cout << "algo 1 " << std::setw ( 4 ) << time << std::setw ( 0 ) << " ns/calc     " << data[ dis_idx ( rng ) ] << nl;
    return true;
}

[[maybe_unused]] bool test_speed_2 ( ) noexcept {
    std::array<int64_t, size> data = { };
    sax::splitmix64 rng{ seed };
    plf::nanotimer t;
    t.start ( );
    for ( int64_t i = 0; i < size * size; ++i )
        data[ dis_idx ( rng ) ] += sax::isqrt_2 ( dis_test ( rng ) );
    uint64_t time = static_cast<uint64_t> ( t.get_elapsed_ns ( ) ) / ( size * size );
    std::cout << "algo 2 " << std::setw ( 4 ) << time << std::setw ( 0 ) << " ns/calc     " << data[ dis_idx ( rng ) ] << nl;
    return true;
}

[[maybe_unused]] bool test_speed_3 ( ) noexcept {
    std::array<int64_t, size> data = { };
    sax::splitmix64 rng{ seed };
    plf::nanotimer t;
    t.start ( );
    for ( int64_t i = 0; i < size * size; ++i )
        data[ dis_idx ( rng ) ] += sax::isqrt_3 ( dis_test ( rng ) );
    uint64_t time = static_cast<uint64_t> ( t.get_elapsed_ns ( ) ) / ( size * size );
    std::cout << "algo 3 " << std::setw ( 4 ) << time << std::setw ( 0 ) << " ns/calc     " << data[ dis_idx ( rng ) ] << nl;
    return true;
}

[[maybe_unused]] bool test_speed_4 ( ) noexcept {
    std::array<int64_t, size> data = { };
    sax::splitmix64 rng{ seed };
    plf::nanotimer t;
    t.start ( );
    for ( int64_t i = 0; i < size * size; ++i )
        data[ dis_idx ( rng ) ] += sax::isqrt_4 ( dis_test ( rng ) );
    uint64_t time = static_cast<uint64_t> ( t.get_elapsed_ns ( ) ) / ( size * size );
    std::cout << "algo 4 " << std::setw ( 4 ) << time << std::setw ( 0 ) << " ns/calc     " << data[ dis_idx ( rng ) ] << nl;
    return true;
}

[[maybe_unused]] bool test_algorithm ( ) noexcept {
    sax::splitmix64 rng{ seed };
    sax::uniform_int_distribution<std::size_t> dis_idx{ 0, size - 1 };
    for ( int64_t i = 0; i < size; ++i ) {
        auto v     = dis_test ( rng );
        int64_t i0 = sax::isqrt_0 ( v );
        int64_t i1 = sax::isqrt_1 ( v );
        int64_t i2 = sax::isqrt_2 ( v );
        int64_t i3 = sax::isqrt_3 ( v );
        int64_t i4 = sax::isqrt_4 ( v );
        // For debugging.
        // std::cout << std::setw ( 4 ) << i << ' ' << std::setw ( 2 ) << i1 << ' ' << i2 << ' ' << i3 << " == " << std::setw ( 0 )
        //          << ( i1 == i2 and i2 == i3 ? "" : " boo . . ." ) << nl;
        assert ( i0 == i1 and i1 == i2 and i2 == i3 and i3 == i4 );
    }
    return true;
}

int main ( ) {

    auto algo = test_algorithm ( );

    auto speed_0 = test_speed_0 ( );
    auto speed_1 = test_speed_1 ( );
    auto speed_2 = test_speed_2 ( );
    auto speed_3 = test_speed_3 ( );
    auto speed_4 = test_speed_4 ( );

    std::cout << ( ( algo and speed_0 and speed_1 and speed_2 and speed_3 and speed_4 ) ? "success" : "failure" ) << nl;

    return EXIT_SUCCESS;
}
