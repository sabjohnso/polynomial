#pragma once

//
// ... Polynomial header files
//
#include <polynomial/details/import.hpp>

namespace Poly::Details
{
  template<typename T>
  using oType_from = typename T::type;

  /**
   * @brief Return the square of input
   */
  template<typename T>
  constexpr T
  sqr(T x){ return x*x; }

  /**
   * @brief Return the input raised to a nonegative integer power
   */
  template<typename T, size_type I>
  constexpr T
  powi(T x, Nat<I>){
    if constexpr (I == 0){
      return T(1);
    } else if constexpr (I == 1){
      return x;
    } else if constexpr (I % 2 == 1){
      return x*powi(x, nat<I-1>);
    } else if constexpr (I % 2 == 0){
      return sqr(powi(x, nat<I/2>));
    }
  }


  /**
   * @brief Return the input raised to a nonegative integer power
   */
  template<typename T>
  constexpr T
  powi(T x, size_t i){
    return i == 0
      ? T(1)
      : (i == 1
         ? x
         : (i % 2 == 1
            ? x*powi(x, i-1)
            : sqr(powi(x, i/2))));
  }

} // end of namespace Poly::Details
