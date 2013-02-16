//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
// 
// QUESO - a library to support the Quantification of Uncertainty
// for Estimation, Simulation and Optimization
//
// Copyright (C) 2008,2009,2010,2011,2012 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor, 
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-
// 
// $Id: $
//
//--------------------------------------------------------------------------

#ifndef __QUESO_FUNCTIONOPERATORBUILDER_BASE__
#define __QUESO_FUNCTIONOPERATORBUILDER_BASE__

/*!
 * \file uqFunctionOperatorBuilder.h
 * \brief Helper class for function and operator objects. This class is meant
 *        to hold common FEM library backend options in a library-agnostic
 *        fashion
 */

#include <string>

class uqFunctionOperatorBuilder {
public:
  //! @name Constructor/Destructor methods
  //@{
  //! Default constructor
  uqFunctionOperatorBuilder();

  //! Destructor
  ~uqFunctionOperatorBuilder();
  //@}

  //! String to store the polynomial family to use. Default is "LAGRANGE".
  std::string family;

  //! String to store the polynomial order to use. Default is "FIRST".
  std::string order;

  //! Number of eigenpairs to request when building an operator. Default is 0.
  unsigned int num_req_eigenpairs;
};

#endif // __QUESO_FUNCTIONOPERATORBUILDER_BASE__
