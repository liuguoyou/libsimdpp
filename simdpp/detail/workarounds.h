/*  Copyright (C) 2011-2012  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LIBSIMDPP_SIMDPP_DETAIL_WORKAROUNDS_H
#define LIBSIMDPP_SIMDPP_DETAIL_WORKAROUNDS_H

// this file contains workarounds for common compiler problems

// the implementation of XOP's com instruction is buggy in clang 3.5 and 3.4.
// clang 3.3 doesn't support the corresponding intrinsic at all
#if defined(__clang__)
#define SIMDPP_WORKAROUND_XOP_COM 1
#endif

#if SIMDPP_USE_NEON64
#if (__GNUC__ == 4) && (__GNUC_MINOR__ <= 8)
#define vreinterpretq_f64_u64(x) ((float64x2_t) (uint64x2_t) (x))
#define vreinterpretq_u64_f64(x) ((uint64x2_t) (float64x2_t) (x))
#endif

#if (__GNUC__ == 4) && (__GNUC_MINOR__ <= 9)
#define vmul_f64(x, y) ((float64x1_t)( ((float64x1_t)(x)) * ((float64x1_t)(y)) ))
#endif
#endif

#if SIMDPP_USE_AVX512F
#if ((__GNUC__ == 4) || (__GNUC__ == 5)) && !__INTEL_COMPILER
#define SIMDPP_WORKAROUND_AVX512F_NO_REDUCE 1
#endif

#if (__GNUC__ == 4) && (__GNUC_MINOR__ <= 9) && !__INTEL_COMPILER
#define _mm512_castps512_ps128(x) ((__m128)(x))
#define _mm512_castps512_ps256(x) ((__m256)(x))
#define _mm512_castpd512_pd128(x) ((__m128d)(x))
#define _mm512_castpd512_pd256(x) ((__m256d)(x))
#define _mm512_castsi512_si256(x) ((__m256i)(x))
#define _mm512_castsi512_si128(x) ((__m128i)(x))
#define _mm512_castpd_ps(x) ((__m512)(x))
#define _mm512_castps_pd(x) ((__m512d)(x))
#define _mm512_castpd_si512(x) ((__m512i)(x))

// Certain casts are missing and we can't use _mm512_inserti64x4 due to GCC
// codegen bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=70059
#define _mm512_castps256_ps512(x) (__builtin_ia32_ps512_256ps((x)))
#define _mm512_castpd256_pd512(x) (__builtin_ia32_pd512_256pd((x)))
#define _mm512_castsi256_si512(x) ((__m512i)__builtin_ia32_si512_256si ((__v8si)(x)))
#endif

#endif

namespace simdpp {
namespace SIMDPP_ARCH_NAMESPACE {
namespace detail {

template<unsigned V> struct make_constexpr { static const unsigned value = V; };

} // namespace detail
} // namespace SIMDPP_ARCH_NAMESPACE
} // namespace simdpp

#if __GNUC__
#define SIMDPP_WORKAROUND_MAKE_CONSTEXPR(X) detail::make_constexpr<(X)>::value
#else
#define SIMDPP_WORKAROUND_MAKE_CONSTEXPR(X) (X)
#endif

#endif
