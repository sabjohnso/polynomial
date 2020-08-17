#pragma once

#define QUOTE_AUX(...) #__VA_ARGS__
#define QUOTE(...) QUOTE_AUX(__VA_ARGS__)

#define STATIC_EXPECT_EQUAL(a, b)                                       \
  [=]{                                                                  \
    static_assert(                                                      \
      (a) == (b),                                                       \
      "Expected equality of the inputs expressions: "                   \
      QUOTE(a) ", " QUOTE(b));                                          \
    EXPECT_EQ((a), (b));}()

#define STATIC_EXPECT_TRUE(...)                                         \
  [=]{                                                                  \
    static_assert(                                                      \
      (__VA_ARGS__) ,                                                   \
      "Expected the input expression to be true: "                      \
      QUOTE(__VA_ARGS__));                                              \
    EXPECT_TRUE((__VA_ARGS__));}()
