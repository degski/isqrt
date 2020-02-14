
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

#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <limits>
#include <type_traits>

#if _HAS_EXCEPTIONS == 0
#    define NOEXCEPT
#else
#    define NOEXCEPT noexcept
#endif

namespace sax {

namespace detail {

#define ever                                                                                                                       \
    ;                                                                                                                              \
    ;
template<typename USizeType>
[[nodiscard]] inline constexpr USizeType isqrt_impl_0 ( USizeType val_ ) NOEXCEPT {
    USizeType c = USizeType{ 1 } << ( 4 * sizeof ( USizeType ) - 1 );
    USizeType g = USizeType{ 1 } << ( 4 * sizeof ( USizeType ) - 1 );
    for ( ever ) {
        if ( g * g > val_ )
            g ^= c;
        c >>= 1;
        if ( not c )
            return g;
        g |= c;
    }
}

} // namespace detail

template<typename SizeType>
[[nodiscard]] inline constexpr SizeType isqrt_0 ( SizeType const val_ ) NOEXCEPT {
    assert ( val_ > 0 );
    return static_cast<SizeType> ( detail::isqrt_impl_0 ( static_cast<typename std::make_unsigned<SizeType>::type> ( val_ ) ) );
}

// https://web.archive.org/web/20120306040058/http://medialab.freaknet.org/martin/src/sqrt/sqrt.c

template<typename SizeType>
[[nodiscard]] inline constexpr SizeType isqrt_1 ( SizeType val_ ) NOEXCEPT {
    assert ( val_ > 0 );
    SizeType res                                    = 0;
    typename std::make_unsigned<SizeType>::type bit = typename std::make_unsigned<SizeType>::type{ 1 }
                                                      << ( 8 * sizeof ( SizeType ) - 2 );
    // "bit" starts at the highest power of four <= the argument.
    while ( static_cast<SizeType> ( bit ) > val_ )
        bit >>= 2;
    while ( bit ) {
        if ( val_ >= res + static_cast<SizeType> ( bit ) ) {
            val_ -= res + static_cast<SizeType> ( bit );
            res /= 2;
            res += static_cast<SizeType> ( bit );
        }
        else {
            res /= 2;
        }
        bit >>= 2;
    }
    return res;
}

// https://stackoverflow.com/questions/31117497/fastest-integer-square-root-in-the-least-amount-of-instructions

template<typename SizeType>
[[nodiscard]] inline constexpr SizeType isqrt_2 ( SizeType val_ ) NOEXCEPT {
    using unsigned_size_type = typename std::make_unsigned<SizeType>::type;
    assert ( val_ > 0 );
    unsigned_size_type val = static_cast<unsigned_size_type> ( val_ ), g = 0,
                       bshft = unsigned_size_type{ 4 } * sizeof ( unsigned_size_type ) - unsigned_size_type{ 1 },
                       b     = unsigned_size_type{ 1 } << bshft;
    do {
        if ( unsigned_size_type const temp = ( ( g << 1 ) + b ) << bshft--; val >= temp ) {
            g += b;
            val -= temp;
        }
    } while ( b >>= 1 );
    return static_cast<SizeType> ( g );
}

// https://stackoverflow.com/questions/21657491/an-efficient-algorithm-to-calculate-the-integer-square-root-isqrt-of-arbitrari

namespace detail {
template<typename U, typename HalfWidthU = typename std::conditional<
                         sizeof ( U ) == sizeof ( uint64_t ), uint32_t,
                         typename std::conditional<sizeof ( U ) == sizeof ( uint32_t ), uint16_t, uint8_t>::type>::type>
[[nodiscard]] inline constexpr HalfWidthU isqrt_impl_3 ( U const val_, U const xk ) NOEXCEPT {
    U const xk1 = ( xk + val_ / xk ) / 2;
    return xk1 >= xk ? static_cast<HalfWidthU> ( xk ) : isqrt_impl_3 ( val_, xk1 );
}
} // namespace detail

template<typename SizeType>
[[nodiscard]] inline constexpr SizeType isqrt_3 ( SizeType const val_ ) NOEXCEPT {
    assert ( val_ > 0 );
    return static_cast<SizeType> ( detail::isqrt_impl_3 ( static_cast<typename std::make_unsigned<SizeType>::type> ( val_ ),
                                                          static_cast<typename std::make_unsigned<SizeType>::type> ( val_ ) ) );
}

namespace detail {

#define INNER_ISQRT( s )                                                                                                           \
    temp = ( g << ( s ) ) + ( static_cast<decltype ( g )> ( 1 ) << ( ( s ) *2 - 2 ) );                                             \
    if ( val >= temp ) {                                                                                                           \
        g += static_cast<decltype ( g )> ( 1 ) << ( ( s ) -1 );                                                                    \
        val -= temp;                                                                                                               \
    }

// by Mark Crowne
[[nodiscard]] inline constexpr uint64_t isqrt_impl_4 ( uint64_t val ) NOEXCEPT {
    uint64_t temp = 0, g = 0;
    if ( val >= 0x4000'0000'0000'0000 ) {
        g = 0x8000'0000;
        val -= 0x4000'0000'0000'0000;
    }

    INNER_ISQRT ( 31 )
    INNER_ISQRT ( 30 )
    INNER_ISQRT ( 29 )
    INNER_ISQRT ( 28 )
    INNER_ISQRT ( 27 )
    INNER_ISQRT ( 26 )
    INNER_ISQRT ( 25 )
    INNER_ISQRT ( 24 )
    INNER_ISQRT ( 23 )
    INNER_ISQRT ( 22 )
    INNER_ISQRT ( 21 )
    INNER_ISQRT ( 20 )
    INNER_ISQRT ( 19 )
    INNER_ISQRT ( 18 )
    INNER_ISQRT ( 17 )
    INNER_ISQRT ( 16 )
    INNER_ISQRT ( 15 )
    INNER_ISQRT ( 14 )
    INNER_ISQRT ( 13 )
    INNER_ISQRT ( 12 )
    INNER_ISQRT ( 11 )
    INNER_ISQRT ( 10 )
    INNER_ISQRT ( 9 )
    INNER_ISQRT ( 8 )
    INNER_ISQRT ( 7 )
    INNER_ISQRT ( 6 )
    INNER_ISQRT ( 5 )
    INNER_ISQRT ( 4 )
    INNER_ISQRT ( 3 )
    INNER_ISQRT ( 2 )

    temp = g + g + 1;
    if ( val >= temp )
        g++;
    return g;
}

[[nodiscard]] inline constexpr uint64_t isqrt_impl_4_ ( uint64_t val ) NOEXCEPT {
    uint64_t temp = 0, g = 0;
    if ( val >= ( 0x4000'0000'0000'0000 >> 1 ) ) {
        g = ( 0x8000'0000 >> 1 );
        val -= ( 0x4000'0000'0000'0000 >> 1 );
    }

    INNER_ISQRT ( 30 )
    INNER_ISQRT ( 29 )
    INNER_ISQRT ( 28 )
    INNER_ISQRT ( 27 )
    INNER_ISQRT ( 26 )
    INNER_ISQRT ( 25 )
    INNER_ISQRT ( 24 )
    INNER_ISQRT ( 23 )
    INNER_ISQRT ( 22 )
    INNER_ISQRT ( 21 )
    INNER_ISQRT ( 20 )
    INNER_ISQRT ( 19 )
    INNER_ISQRT ( 18 )
    INNER_ISQRT ( 17 )
    INNER_ISQRT ( 16 )
    INNER_ISQRT ( 15 )
    INNER_ISQRT ( 14 )
    INNER_ISQRT ( 13 )
    INNER_ISQRT ( 12 )
    INNER_ISQRT ( 11 )
    INNER_ISQRT ( 10 )
    INNER_ISQRT ( 9 )
    INNER_ISQRT ( 8 )
    INNER_ISQRT ( 7 )
    INNER_ISQRT ( 6 )
    INNER_ISQRT ( 5 )
    INNER_ISQRT ( 4 )
    INNER_ISQRT ( 3 )
    INNER_ISQRT ( 2 )

    temp = g + g + 1;
    if ( val >= temp )
        g++;
    return g;
}

// by Mark Crowne
[[nodiscard]] inline constexpr uint32_t isqrt_impl_4 ( uint32_t val ) NOEXCEPT {
    uint32_t temp = 0, g = 0;
    if ( val >= 0x4000'0000 ) {
        g = 0x8000;
        val -= 0x4000'0000;
    }

    INNER_ISQRT ( 15 )
    INNER_ISQRT ( 14 )
    INNER_ISQRT ( 13 )
    INNER_ISQRT ( 12 )
    INNER_ISQRT ( 11 )
    INNER_ISQRT ( 10 )
    INNER_ISQRT ( 9 )
    INNER_ISQRT ( 8 )
    INNER_ISQRT ( 7 )
    INNER_ISQRT ( 6 )
    INNER_ISQRT ( 5 )
    INNER_ISQRT ( 4 )
    INNER_ISQRT ( 3 )
    INNER_ISQRT ( 2 )

    temp = g + g + 1;
    if ( val >= temp )
        g++;
    return g;
}

[[nodiscard]] inline constexpr uint32_t isqrt_impl_4_ ( uint32_t val ) NOEXCEPT {
    uint32_t temp = 0, g = 0;
    if ( val >= ( 0x4000'0000 >> 1 ) ) {
        g = ( 0x8000 > 1 );
        val -= ( 0x4000'0000 >> 1 );
    }

    INNER_ISQRT ( 14 )
    INNER_ISQRT ( 13 )
    INNER_ISQRT ( 12 )
    INNER_ISQRT ( 11 )
    INNER_ISQRT ( 10 )
    INNER_ISQRT ( 9 )
    INNER_ISQRT ( 8 )
    INNER_ISQRT ( 7 )
    INNER_ISQRT ( 6 )
    INNER_ISQRT ( 5 )
    INNER_ISQRT ( 4 )
    INNER_ISQRT ( 3 )
    INNER_ISQRT ( 2 )

    temp = g + g + 1;
    if ( val >= temp )
        g++;
    return g;
}

// by Mark Crowne
[[nodiscard]] inline constexpr uint16_t isqrt_impl_4 ( uint16_t val ) NOEXCEPT {
    uint16_t temp = 0, g = 0;
    if ( val >= 0x4000 ) {
        g = 0x80;
        val -= 0x4000;
    }

    INNER_ISQRT ( 7 )
    INNER_ISQRT ( 6 )
    INNER_ISQRT ( 5 )
    INNER_ISQRT ( 4 )
    INNER_ISQRT ( 3 )
    INNER_ISQRT ( 2 )

    temp = g + g + 1;
    if ( val >= temp )
        g++;
    return g;
}

[[nodiscard]] inline constexpr uint16_t isqrt_impl_4_ ( uint16_t val ) NOEXCEPT {
    uint16_t temp = 0, g = 0;
    if ( val >= ( 0x4000 >> 1 ) ) {
        g = ( 0x80 >> 1 );
        val -= ( 0x4000 >> 1 );
    }

    INNER_ISQRT ( 6 )
    INNER_ISQRT ( 5 )
    INNER_ISQRT ( 4 )
    INNER_ISQRT ( 3 )
    INNER_ISQRT ( 2 )

    temp = g + g + 1;
    if ( val >= temp )
        g++;
    return g;
}

// by Mark Crowne
[[nodiscard]] inline constexpr uint8_t isqrt_impl_4 ( uint8_t val ) NOEXCEPT {
    uint8_t temp = 0, g = 0;
    if ( val >= 0x40 ) {
        g = 0x8;
        val -= 0x40;
    }

    INNER_ISQRT ( 3 )
    INNER_ISQRT ( 2 )

    temp = g + g + 1;
    if ( val >= temp )
        g++;
    return g;
}

[[nodiscard]] inline constexpr uint8_t isqrt_impl_4_ ( uint8_t val ) NOEXCEPT {
    uint8_t temp = 0, g = 0;
    if ( val >= ( 0x40 >> 1 ) ) {
        g = ( 0x8 >> 1 );
        val -= ( 0x40 >> 1 );
    }

    INNER_ISQRT ( 2 )

    temp = g + g + 1;
    if ( val >= temp )
        g++;
    return g;
}

#undef INNER_ISQRT

} // namespace detail

template<typename SizeType>
[[nodiscard]] inline constexpr SizeType isqrt_4 ( SizeType const val_ ) NOEXCEPT {
    assert ( val_ > 0 );
    if constexpr ( sizeof ( SizeType ) >= 8 )
        if ( val_ > std::numeric_limits<uint32_t>::max ( ) )
            return static_cast<SizeType> ( detail::isqrt_impl_4 ( static_cast<uint64_t> ( val_ ) ) );
    if constexpr ( sizeof ( SizeType ) >= 4 )
        if ( val_ > std::numeric_limits<uint16_t>::max ( ) )
            return static_cast<SizeType> ( detail::isqrt_impl_4 ( static_cast<uint32_t> ( val_ ) ) );
    if constexpr ( sizeof ( SizeType ) >= 2 )
        if ( val_ > std::numeric_limits<uint8_t>::max ( ) )
            return static_cast<SizeType> ( detail::isqrt_impl_4 ( static_cast<uint16_t> ( val_ ) ) );
    return static_cast<SizeType> ( detail::isqrt_impl_4 ( static_cast<uint8_t> ( val_ ) ) );
}

} // namespace sax

#undef NOEXCEPT
#undef ever
#undef GNU
#undef MSVC
#undef CLANG
#undef GCC
#undef M64
#undef M32
#undef NOEXCEPT
