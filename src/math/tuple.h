/*~-------------------------------------------------------------------------~~*
 *     _   ______________     ___    __    ______
 *    / | / / ____/ ____/    /   |  / /   / ____/
 *   /  |/ / / __/ /  ______/ /| | / /   / __/   
 *  / /|  / /_/ / /__/_____/ ___ |/ /___/ /___   
 * /_/ |_/\____/\____/    /_/  |_/_____/_____/   
 * 
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~-------------------------------------------------------------------------~~*/
/*!
 *
 * \file tuple.h
 * 
 * \brief Provides a tuple type which functions as a vector.
 *
 ******************************************************************************/
#pragma once

// system includes
#include <tuple>

//! user includes
#include "ale/math/operators.h"
#include "ale/utils/tuple_for_each.h"
#include "ale/utils/tuple_visit.h"
#include "ale/utils/tuple_zip.h"

namespace ale {
namespace math {

////////////////////////////////////////////////////////////////////////////////
//!  \brief The dimensioned_array type provides a general base for defining
//!  contiguous array types that have a specific dimension.
//!
//!  \tparam T The type of the array, e.g., P.O.D. type.
//!  \tparam D The dimension of the array, i.e., the number of elements
//!    to be stored in the array.
////////////////////////////////////////////////////////////////////////////////
template <typename... Types>
using tuple = std::tuple<Types...>;

//! \brief use std::get for access of tuple_t
using std::get;

//! \brief Constructor with one value.
//! \param[in] val The value to set the array to
template < typename... Types >
void fill( tuple<Types...> & t, const auto& val ) 
{ 
  //std::cout << "tuple (single value constructor)\n";
  using std::fill;  // enable ADL
  //using std::begin; // enable ADL  
  //using std::end;   // enable ADL
  utils::tuple_for_each( t, 
                         [&](auto & tup) { 
                           fill( begin(tup), end(tup), val ); 
                         } );
}


//! \brief Add to a tuple.
//! \param[in] lhs The value on the left hand side of the operator.
//! \param[in] rhs The value on the right hand side of the operator.
template <typename... Types>
void plus_equal( tuple<Types...>& lhs, const tuple<Types...>& rhs )
{
  utils::tuple_for_each( utils::tuple_tie( lhs, rhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = get<0>(tup) + get<1>(tup);
                         } );                        
}

template <typename... Types>
void plus_equal( tuple<Types...>& lhs, const auto & rhs )
{
  utils::tuple_for_each( lhs,
                         [&](auto & tup) { 
                           tup = tup + rhs;
                         } );                        
}


//! \brief Addition operator involving tuples.
//! \param[in] lhs The value on the left hand side of the operator.
//! \param[in] rhs The value on the right hand side of the operator.
//! \return The result of the operation.
template <typename... Types>
auto operator+( const tuple<Types...>& lhs, 
                const tuple<Types...>& rhs )
{
  tuple<Types...> tmp;
  utils::tuple_visit( 
                     [](auto & a, const auto & b, const auto & c) { 
                       a = b + c;
                     },
                     tmp, lhs, rhs );
  return tmp;
}

template <typename... Types>
auto operator+( const tuple<Types...>& lhs, const auto & rhs )
{
  tuple<Types...> tmp;
  utils::tuple_for_each( utils::tuple_tie( tmp, lhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = get<1>(tup) + rhs;
                         } );                        
  return tmp;
}

template <typename... Types>
auto operator+( const auto & lhs, const tuple<Types...>& rhs )
{
  tuple<Types...> tmp;
  utils::tuple_for_each( utils::tuple_tie( tmp, rhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = lhs + get<1>(tup);
                         } );                        
  return tmp;
}



//! \brief Subtract a tuple.
//! \param[in] lhs The value on the left hand side of the operator.
//! \param[in] rhs The value on the right hand side of the operator.
template <typename... Types>
void minus_equal( tuple<Types...>& lhs, const tuple<Types...>& rhs )
{
  utils::tuple_for_each( utils::tuple_tie( lhs, rhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = get<0>(tup) - get<1>(tup);
                         } );                        
}

template <typename... Types>
void minus_equal( tuple<Types...>& lhs, const auto & rhs )
{
  utils::tuple_for_each( lhs,
                         [&](auto & tup) { 
                           tup = tup - rhs;
                         } );                        
}


//! \brief Subtraction operator involving tuples.
//! \param[in] lhs The value on the left hand side of the operator.
//! \param[in] rhs The value on the right hand side of the operator.
//! \return The result of the operation.
template <typename... Types>
auto operator-( const tuple<Types...>& lhs, const tuple<Types...>& rhs )
{
  tuple<Types...> tmp;
  utils::tuple_visit( 
                     [](auto & a, const auto & b, const auto & c) { 
                       a = b - c;
                     },
                     tmp, lhs, rhs );
  return tmp;
}

template <typename... Types>
auto operator-( const tuple<Types...>& lhs, const auto & rhs )
{
  tuple<Types...> tmp;
  utils::tuple_for_each( utils::tuple_tie( tmp, lhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = get<1>(tup) - rhs;
                         } );                        
  return tmp;
}

template <typename... Types>
auto operator-( const auto & lhs, const tuple<Types...>& rhs )
{
  tuple<Types...> tmp;
  utils::tuple_for_each( utils::tuple_tie( tmp, rhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = lhs - get<1>(tup);
                         } );                        
  return tmp;
}


//! \brief Multiply a tuple.
//! \param[in] lhs The value on the left hand side of the operator.
//! \param[in] rhs The value on the right hand side of the operator.
template <typename... Types>
void multiplies_equal( tuple<Types...>& lhs, const tuple<Types...>& rhs )
{
  utils::tuple_for_each( utils::tuple_tie( lhs, rhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = get<0>(tup) * get<1>(tup);
                         } );                        
}

template <typename... Types>
void multiplies_equal( tuple<Types...>& lhs, const auto & rhs )
{
  utils::tuple_for_each( lhs,
                         [&](auto & tup) { 
                           tup = tup * rhs;
                         } );                        
}


//! \brief Multiplication operator involving tuples.
//! \param[in] lhs The value on the left hand side of the operator.
//! \param[in] rhs The value on the right hand side of the operator.
//! \return The result of the operation.
template <typename... Types>
auto operator*( const tuple<Types...>& lhs, const tuple<Types...>& rhs )
{
  tuple<Types...> tmp;
  utils::tuple_visit( 
                     [](auto & a, const auto & b, const auto & c) { 
                       a = b * c;
                     },
                     tmp, lhs, rhs );
  return tmp;
}

template <typename... Types>
auto operator*( const tuple<Types...>& lhs, const auto & rhs )
{
  tuple<Types...> tmp;
  utils::tuple_for_each( utils::tuple_tie( tmp, lhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = get<1>(tup) * rhs;
                         } );                        
  return tmp;
}

template <typename... Types>
auto operator*( const auto & lhs, const tuple<Types...>& rhs )
{
  tuple<Types...> tmp;
  utils::tuple_for_each( utils::tuple_tie( tmp, rhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = lhs * get<1>(tup);
                         } );                        
  return tmp;
}





//! \brief Divide a tuple.
//! \param[in] lhs The value on the left hand side of the operator.
//! \param[in] rhs The value on the right hand side of the operator.
template <typename... Types>
void divides_equal( tuple<Types...>& lhs, const tuple<Types...>& rhs )
{
  utils::tuple_for_each( utils::tuple_tie( lhs, rhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = get<0>(tup) / get<1>(tup);
                         } );                        
}

template <typename... Types>
void divides_equal( tuple<Types...>& lhs, const auto & rhs )
{
  utils::tuple_for_each( lhs,
                         [&](auto & tup) { 
                           tup = tup / rhs;
                         } );                        
}


//! \brief Division operator involving tuples.
//! \param[in] lhs The value on the left hand side of the operator.
//! \param[in] rhs The value on the right hand side of the operator.
//! \return The result of the operation.
template <typename... Types>
auto operator/( const tuple<Types...>& lhs, const tuple<Types...>& rhs )
{
  tuple<Types...> tmp;
  utils::tuple_visit( 
                     [](auto & a, const auto & b, const auto & c) { 
                       a = b / c;
                     },
                     tmp, lhs, rhs );
  return tmp;
}

template <typename... Types>
auto operator/( const tuple<Types...>& lhs, const auto & rhs )
{
  tuple<Types...> tmp;
  utils::tuple_for_each( utils::tuple_tie( tmp, lhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = get<1>(tup) / rhs;
                         } );                        
  return tmp;
}

template <typename... Types>
auto operator/( const auto & lhs, const tuple<Types...>& rhs )
{
  tuple<Types...> tmp;
  utils::tuple_for_each( utils::tuple_tie( tmp, rhs ),
                         [&](auto tup) { 
                           using std::get;
                           get<0>(tup) = lhs / get<1>(tup);
                         } );                        
  return tmp;
}




//! \brief Output operator for tuple.
//! \tparam T  The array base value type.
//! \tparam D  The array dimension.
//! \param[in,out] os  The ostream to dump output to.
//! \param[in]     rhs The tuple on the right hand side of the operator.
//! \return A reference to the current ostream.
template <typename... Types>
auto & operator<<(std::ostream& os, const tuple<Types...>& a)
{
  os << "{";
  utils::tuple_for_each( a, 
                          [](auto & tup) { std::cout << " [ " << tup << " ]"; } );
  os << " }";
  return os;
}
  
 
 

} // namespace
} // namespace

/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
