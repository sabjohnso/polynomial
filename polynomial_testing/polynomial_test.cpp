//
// ... Testing header files
//
#include <gtest/gtest.h>
#include <polynomial_testing/test_macros.hpp>

//
// ... External header files
//
#include <type_utility/type_utility.hpp>


//
// ... Polynomial header files
//
#include <polynomial/polynomial.hpp>


namespace Poly::Testing
{

  namespace
  {
    using TypeUtility::type;
    using TypeUtility::nat;
    template<size_type N>
    constexpr auto x = monomial(type<double>, nat<N>);
  }

  TEST(Polynomial, ConstantZero){
    constexpr auto p = Polynomial(0.0);
    STATIC_EXPECT_EQUAL(p(0.0), 0.0);
    STATIC_EXPECT_EQUAL(p(1.0), 0.0);
    STATIC_EXPECT_EQUAL(p(2.0), 0.0);
  }

  TEST(Polynomial, ConstantOne){
    constexpr auto p = Polynomial(1.0);
    STATIC_EXPECT_EQUAL(p(0.0), 1.0);
    STATIC_EXPECT_EQUAL(p(1.0), 1.0);
    STATIC_EXPECT_EQUAL(p(2.0), 1.0);
  }

  TEST(Polynomial, LinearMonomial){
    constexpr auto p = Polynomial(0.0, 1.0);
    STATIC_EXPECT_EQUAL(p(0.0), 0.0);
    STATIC_EXPECT_EQUAL(p(1.0), 1.0);
    STATIC_EXPECT_EQUAL(p(2.0), 2.0);
  }

  TEST(Polynomial, Linear){
    constexpr auto p = Polynomial(1.0, 1.0);
    STATIC_EXPECT_EQUAL(p(0.0), 1.0);
    STATIC_EXPECT_EQUAL(p(1.0), 2.0);
    STATIC_EXPECT_EQUAL(p(2.0), 3.0);
  }

  TEST(Polynomial, QuadraticMonomial){
    constexpr auto p = Polynomial(0.0, 0.0, 1.0);
    STATIC_EXPECT_EQUAL(p(0.0), 0.0);
    STATIC_EXPECT_EQUAL(p(1.0), 1.0);
    STATIC_EXPECT_EQUAL(p(2.0), 4.0);
  }

  TEST(Polynomial, SimpleAddition1){
    constexpr auto p1 = Polynomial(1.0);
    constexpr auto p2 = Polynomial(2.0);
    constexpr auto p3 = p1 + p2;

    STATIC_EXPECT_EQUAL(p3(0.0), 3.0);
  }

  TEST(Polynomial, LinearAddition){
    constexpr auto p1 = Polynomial(1.0, 2.0);
    constexpr auto p2 = Polynomial(3.0, 4.0);
    constexpr auto p3 = p1 + p2;

    STATIC_EXPECT_EQUAL(p3(0.0), p1(0.0) + p2(0.0));
    STATIC_EXPECT_EQUAL(p3(1.0), p1(1.0) + p2(1.0));
    STATIC_EXPECT_EQUAL(p3(2.0), p1(2.0) + p2(2.0));
  }

  TEST(Polynomial, ConstantMultiplication){
    constexpr auto p1 = Polynomial(2.0);
    constexpr auto p2 = Polynomial(3.0);
    constexpr auto p3 = p1 * p2;

    STATIC_EXPECT_TRUE(p3(0.0), 6.0);
  }

  TEST(Polynomial, LinearMultiplication){
    constexpr auto p1 = Polynomial( 1.0, 1.0);
    constexpr auto p2 = Polynomial(-1.0, 1.0);
    constexpr auto p3 = p1 * p2;

    STATIC_EXPECT_EQUAL(p3[0], -1.0);
    STATIC_EXPECT_EQUAL(p3[1],  0.0);
    STATIC_EXPECT_EQUAL(p3[2],  1.0);
  }

  TEST(Polynomial, Construction){
    constexpr auto p1 = Polynomial(1.0, 2.0, 3.0);
    constexpr auto p2 = 3.0*x<2> + 2.0*x<1> + x<0>;
    STATIC_EXPECT_EQUAL(p1, p2);
  }

} // end of namespace Poly::Testing
