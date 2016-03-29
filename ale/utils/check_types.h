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
 * \file check_types.h
 * 
 * \brief Statically check if all arguments are of the same type.
 *
 ******************************************************************************/
#pragma once


// user includes
#include "detail/check_types.h"


namespace ale {
namespace utils {


////////////////////////////////////////////////////////////////////////////////
//! \brief Test to see if all variadic template arguments are of type Target
////////////////////////////////////////////////////////////////////////////////
template<typename Target, typename... Ts>
using are_type_t = detail::and_<std::is_same<Ts,Target>...>;


} // namespace
} // namespace



/*~------------------------------------------------------------------------~--*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~------------------------------------------------------------------------~--*/
