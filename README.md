# isqrt

Implementing `integer isqrt ( integer )`, the fastest way:


    #include <cassert>
    #include <cmath>

    // Integer square root.
    template<typename SizeType>
    [[nodiscard]] inline constexpr SizeType isqrt ( SizeType val_ ) noexcept {
        assert ( val_ > 0 );
        return static_cast<SizeType> ( std::sqrt ( static_cast<double> ( val_ ) ) );
    }
