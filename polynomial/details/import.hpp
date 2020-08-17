#pragma once

//
// ... Standard header files
//
#include <cmath>
#include <cstdint>

#include <type_traits>
#include <utility>
#include <array>
#include <vector>

//
// ... External header files
//
#include <type_utility/type_utility.hpp>
#include <summation/summation.hpp>

namespace Poly::Details
{

  using size_type = std::size_t;
  using integer = std::ptrdiff_t;

  using std::abs;
  using std::min;
  using std::max;
  using std::pow;

  using std::forward;
  using std::move;
  using std::get;

  using std::array;

  using std::integral_constant;
  using std::index_sequence;
  using std::decay_t;
  using std::common_type_t;
  using std::conditional_t;


  using TypeUtility::generate_indices;
  using TypeUtility::count_types;
  using TypeUtility::count_args;
  using TypeUtility::Nat;
  using TypeUtility::nat;
  using TypeUtility::drop;
  using TypeUtility::type;
  using TypeUtility::Type;
  using TypeUtility::Type_sequence;
  using TypeUtility::types;

  using Summation::sum;
  using NaiveSummation = decay_t<decltype(Summation::naive)>;
  using NeumaierSummation = decay_t<decltype(Summation::neumaier)>;

} // end of namespace Poly::Details
