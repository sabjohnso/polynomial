#pragma once

//
// ... Polynomial header files
//
#include <polynomial/details/import.hpp>
#include <polynomial/details/utility.hpp>

namespace Poly::Details
{

  template<typename T, size_type Degree, typename Summation = NeumaierSummation>
  class Polynomial
  {
    static_assert(Degree >= 0);

    using value_type = T;
    static constexpr integer degree = Degree;
    static constexpr integer N = degree+1;

    using storage = array<T,N>;

    storage coefficients;

  public:

    constexpr
    Polynomial(T c)
      : coefficients{{c}}
    {
      static_assert(degree == 0);
    }

    template<typename T1, typename T2, typename ... Ts>
    constexpr
    Polynomial(T1&& c1, T2&& c2, Ts&& ... cs)
      : coefficients{{
          forward<T1>(c1),
          forward<T2>(c2),
          forward<Ts>(cs) ...}}
    {}

    friend constexpr bool
    operator ==(Polynomial const& p1, Polynomial const& p2){
      return equal(p1, p2);
    }

    friend constexpr bool
    operator !=(Polynomial const& p1, Polynomial const& p2){ return !(p1 == p2); }




    template<typename U>
    constexpr auto
    operator()(U&& x) const {
      return call(generate_indices<N>(), forward<U>(x));
    }

    constexpr auto
    operator [](integer i) const { return coefficients[i]; }

    template<typename U, size_type M>
    friend constexpr auto
    operator +(Polynomial const& p1, Polynomial<U,M,Summation> const& p2){
      return add(p1, p2);
    }

    friend constexpr auto
    operator -(Polynomial const& p){
      return negate(p);
    }

    template<typename U, size_type M>
    friend constexpr auto
    operator -(Polynomial const& p1, Polynomial<U,M,Summation> const& p2){
      return p1 + (-p2);
    }

    friend constexpr Polynomial
    operator *(Polynomial const& p, T s){
      return scale(p, s);
    }

    friend constexpr Polynomial
    operator *(T s, Polynomial const& p){
      return scale(p, s);
    }


    template<typename U, size_type M>
    friend constexpr auto
    operator *(Polynomial const& p1, Polynomial<U,M,Summation> const& p2){
      return multiply(p1, p2);
    }

  private:

    static constexpr bool
    equal(Polynomial const& p1, Polynomial const& p2){
      return Equality_helper::exec(p1, p2, generate_indices<Degree+1>());
    }

    struct Equality_helper
    {
      template<size_type ... Indices>
      static constexpr bool
      exec(Polynomial const& p1, Polynomial const& p2, index_sequence<Indices ...>){
        return ((p1[Indices] == p2[Indices]) && ... && true);
      }
    }; // end of struct Equality_helper

    template<size_t ... Indices, typename U>
    constexpr auto
    call(index_sequence<Indices ...>, U&& x) const {
      return sum(Summation{}, get<Indices>(coefficients)*powi(x, nat<Indices>) ...);
    }

    template<typename U1, size_type M1,typename U2, size_type M2, typename S >
    static constexpr Polynomial<common_type_t<U1,U2>, max(M1,M2), S>
    add(Polynomial<U1,M1,S> const& p1, Polynomial<U2,M2,S> const& p2){
      constexpr auto Mmin = min(M1,M2);
      constexpr auto Mmax = max(M1,M2);
      return M1 < M2
        ? Addition_helper::exec(p1, p2, generate_indices<Mmin+1>(), drop(generate_indices<Mmax+1>(), nat<Mmin+1>))
        : Addition_helper::exec(p2, p1, generate_indices<Mmin+1>(), drop(generate_indices<Mmax+1>(), nat<Mmin+1>));
    }

    struct Addition_helper
    {

      template<
        typename U1, size_type M1,
        typename U2, size_type M2,
        typename S,
        size_type ... Combined_indices,
        size_type ... Larger_indices>
      static constexpr auto
      exec(
        Polynomial<U1,M1,S> const& p1,
        Polynomial<U2,M2,S> const& p2,
        index_sequence<Combined_indices ...>,
        index_sequence<Larger_indices ...>){
        using R = Polynomial<common_type_t<U1,U2>, max(M1,M2), S>;
        return R{
          p1[Combined_indices]+p2[Combined_indices] ...,
          p2[Larger_indices] ...};
      }
    }; // end of struct Addition_helper



    static constexpr auto
    negate(Polynomial const& p){
      return Negation_helper::exec(p, generate_indices<Degree+1>());
    }

    struct Negation_helper
    {
      template<size_type ... Indices>
      static constexpr auto
      exec(Polynomial const& p, index_sequence<Indices ...>){
        return Polynomial(-p[Indices] ...);
      }
    }; // end of struct Negation_helper


    static constexpr Polynomial
    scale(Polynomial const& p, T x){
      return Scale_helper::exec(p, x, generate_indices<Degree+1>());
    }

    struct Scale_helper
    {
      template<size_type ... Indices>
      static constexpr Polynomial
      exec(Polynomial const& p, T s, index_sequence<Indices ...>){
        return Polynomial(p[Indices]*s ...);
      }
    }; // end of struct Scale_helper


    template<
      typename U1, size_type M1,
      typename U2, size_type M2,
      typename S>
    static constexpr auto
    multiply(Polynomial<U1, M1,S> const& p1, Polynomial<U2,M2,S> const& p2){
      return Multiplication_helper::exec(
        p1, p2,
        generate_indices<M1+1>(),
        generate_indices<M2+1>(),
        generate_indices<M1+M2+1>());
    }

    struct Multiplication_helper
    {
      template<
        typename U1, size_type M1,
        typename U2, size_type M2,
        typename S,
        typename Indices1,
        typename Indices2,
        size_type ... Degrees>
      static constexpr auto
      exec(Polynomial<U1,M1,S> const& p1, Polynomial<U2,M2,S> const& p2,
           Indices1, Indices2,
           index_sequence<Degrees ...>){
        return Polynomial<common_type_t<U1,U2>, M1+M2,Summation>(
          Multiplication_helper::product_coefficient(
            p1, p2,
            Multiplication_helper::index_pair_set(
              integral_constant<size_type, Degrees>{},
              Indices1{}, Indices2{}))
          ...);
      }

    private:

      template<typename P1, typename P2, typename ... Ts>
      static constexpr auto
      product_coefficient(P1 const& p1, P2 const& p2, Type_sequence<>, Ts ... xs){
        return sum(Summation{}, xs ...);
      }

      template<typename P1, typename P2, size_type Index1, size_type Index2, typename ... Ts, typename ... Us>
      static constexpr auto
      product_coefficient(
        P1 const& p1, P2 const& p2,
        Type_sequence<index_sequence<Index1, Index2>, Ts ...>, Us ... xs){
        return product_coefficient(p1, p2, types<Ts ...>, xs ..., p1[Index1]*p2[Index2]);
      }

      template<typename ... Ts, typename ... Us>
      static constexpr auto
      append(Type_sequence<Ts ...>, Type_sequence<Us ...>){
        return types<Ts ..., Us ...>;
      }

      template<size_type Index, size_type ... Indices>
      static constexpr auto
      pair_indices(integral_constant<size_type, Index>, index_sequence<Indices ...>){
        return types<index_sequence<Index,Indices> ...>;
      }



      template<typename Indices2>
      static constexpr auto
      all_index_pairs(index_sequence<>, Indices2){
        return types<>;
      }

      template<size_type Index1, size_type ... Indices1, typename Indices2>
      static constexpr auto
      all_index_pairs(index_sequence<Index1, Indices1 ...>, Indices2){
        return append(
          pair_indices(integral_constant<size_type,Index1>{}, Indices2{}),
          all_index_pairs(index_sequence<Indices1 ...>{}, Indices2{}));
      }



      /**
       * @brief Terminal specialization of index pair filtering
       */
      template<size_type Index, typename ... Ts>
      static constexpr auto
      filter_index_pairs(integral_constant<size_type, Index>, Type_sequence<>, Ts ...){
        return types<Ts ...>;
      }

      /**
       * @brief Iterate specialization of index pair filtering
       */
      template<size_type Index, size_type Index1, size_type Index2,typename ... Pairs, typename ... Ts>
      static constexpr auto
      filter_index_pairs(
        integral_constant<size_type, Index>,
        Type_sequence<index_sequence<Index1, Index2>, Pairs ...>,
        Ts ...){
        if constexpr (Index == Index1 + Index2){
          return filter_index_pairs(
            integral_constant<size_type, Index>{},
            types<Pairs ...>, Ts{} ...,
            index_sequence<Index1,Index2>{});
        } else {
          return filter_index_pairs(
            integral_constant<size_type, Index>{},
            types<Pairs ...>, Ts{} ...);
        }
      }

      template<size_type Index, typename Indices1, typename Indices2>
      static constexpr auto
      index_pair_set(integral_constant<size_type, Index>, Indices1, Indices2){
        return filter_index_pairs(
          integral_constant<size_type, Index>{},
          all_index_pairs(Indices1{}, Indices2{}));
      }

      static_assert(
        std::is_same_v<decay_t<decltype(index_pair_set(
                                          integral_constant<size_type, 0>{},
                                          index_sequence<0>{},
                                          index_sequence<0>{}))>,
        Type_sequence<index_sequence<0,0>>>);

      static_assert(
        std::is_same_v<
        decay_t<decltype(pair_indices(integral_constant<size_type, 0>{},index_sequence<0, 1, 2, 3>{}))>,
        Type_sequence<index_sequence<0,0>, index_sequence<0,1>, index_sequence<0,2>, index_sequence<0,3>>>);

      static_assert(
        std::is_same_v<
        decay_t<decltype(all_index_pairs(index_sequence<0,1>{},index_sequence<0, 1>{}))>,
        Type_sequence<index_sequence<0,0>,
        index_sequence<0,1>,
        index_sequence<1,0>,
        index_sequence<1,1>>>);

    }; // end of class Multiplication_helper


  }; // end of class Polynomial;

  template<typename T, typename ... Ts>
  Polynomial(T x, Ts ... xs)
    -> Polynomial<common_type_t<T, Ts ...>, count_types<T,Ts...>()-1>;

  template<typename T, size_type N>
  constexpr Polynomial<T,N>
  monomial(Type<T>, Nat<N>);

  class Monomial_helper
  {

    template<typename T, size_type N>
    friend constexpr Polynomial<T,N>
    monomial(Type<T>, Nat<N>);

    template<typename T, size_type N, typename ... Ts>
    static constexpr Polynomial<T,N>
    exec(Type<T>, Nat<N>, Nat<N>, Ts ... xs){
      return Polynomial(xs ..., T(1));
    }

    template<typename T, size_type N, size_type I, typename ... Ts>
    static constexpr Polynomial<T,N>
    exec(Type<T>, Nat<N>, Nat<I>, Ts ... xs){
      return exec(type<T>, nat<N>, nat<I+1>, xs ..., T(0));
    }
  };// end of class Monomial_helper

  template<typename T, size_type N>
  constexpr Polynomial<T,N>
  monomial(Type<T>, Nat<N>){
    return Monomial_helper::exec(type<T>, nat<N>, nat<0>);
  }


} // end of namespace Poly::Details
