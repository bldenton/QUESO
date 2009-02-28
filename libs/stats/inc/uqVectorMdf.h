/*--------------------------------------------------------------------------
 *--------------------------------------------------------------------------
 *
 * Copyright (C) 2008 The PECOS Development Team
 *
 * Please see http://pecos.ices.utexas.edu for more information.
 *
 * This file is part of the QUESO Library (Quantification of Uncertainty
 * for Estimation, Simulation and Optimization).
 *
 * QUESO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QUESO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QUESO. If not, see <http://www.gnu.org/licenses/>.
 *
 *--------------------------------------------------------------------------
 *
 * $Id$
 *
 * Brief description of this file: 
 * 
 *--------------------------------------------------------------------------
 *-------------------------------------------------------------------------- */

#ifndef __UQ_VECTOR_MARGINAL_DENSITY_FUNCTION_H__
#define __UQ_VECTOR_MARGINAL_DENSITY_FUNCTION_H__

#include <uqArrayOfOneDGrids.h>
#include <uqArrayOfOneDTables.h>
#include <uqEnvironment.h>
#include <math.h>

//*****************************************************
// Classes to accomodate a marginal density function
//*****************************************************

//*****************************************************
// Base class
//*****************************************************
template<class V, class M>
class uqBaseVectorMdfClass {
public:
           uqBaseVectorMdfClass(const char*                  prefix,
                                const uqVectorSetClass<V,M>& domainSet);
  virtual ~uqBaseVectorMdfClass();

  const   uqVectorSetClass<V,M>& domainSet() const;
  virtual void                   values   (const V& paramValues,
                                                 V& mdfVec)  const = 0;
  virtual void                   print    (std::ostream& os) const = 0;

protected:

  const   uqBaseEnvironmentClass& m_env;
          std::string             m_prefix;
  const   uqVectorSetClass<V,M>&  m_domainSet;
};

template<class V, class M>
uqBaseVectorMdfClass<V,M>::uqBaseVectorMdfClass(
  const char*                  prefix,
  const uqVectorSetClass<V,M>& domainSet)
  :
  m_env      (domainSet.env()),
  m_prefix   ((std::string)(prefix)+"mdf_"),
  m_domainSet(domainSet)
{
  if ((m_env.subScreenFile()) && (m_env.verbosity() >= 5)) {
    *m_env.subScreenFile() << "Entering uqBaseVectorMdfClass<V,M>::constructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  if ((m_env.subScreenFile()) && (m_env.verbosity() >= 5)) {
    *m_env.subScreenFile() << "Leaving uqBaseVectorMdfClass<V,M>::constructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }
}

template<class V, class M>
uqBaseVectorMdfClass<V,M>::~uqBaseVectorMdfClass()
{
}

template<class V, class M>
const uqVectorSetClass<V,M>&
uqBaseVectorMdfClass<V,M>::domainSet() const
{
  return m_domainSet;
}

//*****************************************************
// Generic probability distibution function class
//*****************************************************
template<class V, class M>
class uqGenericVectorMdfClass : public uqBaseVectorMdfClass<V,M> {
public:
  uqGenericVectorMdfClass(const char*                    prefix,
                          const uqVectorSetClass<V,M>& domainSet,
                          double (*routinePtr)(const V& paramValues, const void* routineDataPtr, V& mdfVec),
                          const void* routineDataPtr);
 ~uqGenericVectorMdfClass();

  void values(const V& paramValues, V& mdfVec) const;
  void print (std::ostream& os)                const;

protected:
  double (*m_routinePtr)(const V& paramValues, const void* routineDataPtr, V& mdfVec);
  const void* m_routineDataPtr;

  using uqBaseVectorMdfClass<V,M>::m_env;
  using uqBaseVectorMdfClass<V,M>::m_prefix;
  using uqBaseVectorMdfClass<V,M>::m_domainSet;
};

template<class V, class M>
uqGenericVectorMdfClass<V,M>::uqGenericVectorMdfClass(
  const char*                    prefix,
  const uqVectorSetClass<V,M>& domainSet,
  double (*routinePtr)(const V& paramValues, const void* routineDataPtr, V& mdfVec),
  const void* routineDataPtr)
  :
  uqBaseVectorMdfClass<V,M>(prefix,domainSet),
  m_routinePtr    (routinePtr),
  m_routineDataPtr(routineDataPtr)
{
}

template<class V, class M>
uqGenericVectorMdfClass<V,M>::~uqGenericVectorMdfClass()
{
}

template<class V, class M>
void
uqGenericVectorMdfClass<V,M>::values(
  const V& paramValues,
        V& mdfVec) const
{
  m_routinePtr(paramValues, m_routineDataPtr, mdfVec);
  return;
}

template <class V, class M>
void
uqGenericVectorMdfClass<V,M>::print(std::ostream& os) const
{
  return;
}

//*****************************************************
// Gaussian probability distribution function class
//*****************************************************
template<class V, class M>
class uqGaussianVectorMdfClass : public uqBaseVectorMdfClass<V,M> {
public:
  uqGaussianVectorMdfClass(const char*                    prefix,
                           const uqVectorSetClass<V,M>& domainSet,
                           const V&                       domainExpectedValues,
                           const V&                       domainVarianceValues);
  uqGaussianVectorMdfClass(const char*                    prefix,
                           const uqVectorSetClass<V,M>& domainSet,
                           const V&                       domainExpectedValues,
                           const M&                       covMatrix);
 ~uqGaussianVectorMdfClass();

  void values(const V& paramValues, V& mdfVec) const;
  void print (std::ostream& os)                const;

protected:
  const M*                         m_covMatrix;

  using uqBaseVectorMdfClass<V,M>::m_env;
  using uqBaseVectorMdfClass<V,M>::m_prefix;
  using uqBaseVectorMdfClass<V,M>::m_domainSet;

  void commonConstructor();
};

template<class V,class M>
uqGaussianVectorMdfClass<V,M>::uqGaussianVectorMdfClass(
  const char*                    prefix,
  const uqVectorSetClass<V,M>& domainSet,
  const V&                       domainExpectedValues,
  const V&                       domainVarianceValues)
  :
  uqBaseVectorMdfClass<V,M>(prefix,domainSet),
  m_covMatrix              (m_domainSet.newDiagMatrix(domainVarianceValues*domainVarianceValues))
{
  if ((m_env.subScreenFile()) && (m_env.verbosity() >= 5)) {
    *m_env.subScreenFile() << "Entering uqGaussianVectorMdfClass<V,M>::constructor() [1]"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  commonConstructor();

  if ((m_env.subScreenFile()) && (m_env.verbosity() >= 5)) {
    *m_env.subScreenFile() << "Leaving uqGaussianVectorMdfClass<V,M>::constructor() [1]"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }
}

template<class V,class M>
uqGaussianVectorMdfClass<V,M>::uqGaussianVectorMdfClass(
  const char*                    prefix,
  const uqVectorSetClass<V,M>& domainSet,
  const V&                       domainExpectedValues,
  const M&                       covMatrix)
  :
  uqBaseVectorMdfClass<V,M>(prefix,domainSet),
  m_covMatrix              (new M(covMatrix))
{
  if ((m_env.subScreenFile()) && (m_env.verbosity() >= 5)) {
    *m_env.subScreenFile() << "Entering uqGaussianVectorMdfClass<V,M>::constructor() [2]"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  commonConstructor();

  if ((m_env.subScreenFile()) && (m_env.verbosity() >= 5)) {
    *m_env.subScreenFile() << "Leaving uqGaussianVectorMdfClass<V,M>::constructor() [2]"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }
}

template<class V,class M>
void
uqGaussianVectorMdfClass<V,M>::commonConstructor()
{
  UQ_FATAL_TEST_MACRO(true,
                      m_env.rank(),
                      "uqGaussianVectorMdfClass<V,M>::commonConstructor()",
                      "incomplete code");
  return;
}

template<class V,class M>
uqGaussianVectorMdfClass<V,M>::~uqGaussianVectorMdfClass()
{
  delete m_covMatrix;
}

template<class V, class M>
void
uqGaussianVectorMdfClass<V,M>::values(
  const V& paramValues,
        V& mdfVec) const
{
  UQ_FATAL_TEST_MACRO(true,
                      m_env.rank(),
                      "uqGaussianVectorMdfClass<V,M>::mdfVec()",
                      "incomplete code");
  return;
}

template <class V, class M>
void
uqGaussianVectorMdfClass<V,M>::print(std::ostream& os) const
{
  return;
}

//*****************************************************
// Sampled probability distribution function class
//*****************************************************
template<class V, class M>
class uqSampledVectorMdfClass : public uqBaseVectorMdfClass<V,M> {
public:
  uqSampledVectorMdfClass(const char*                          prefix,
                          const uqArrayOfOneDGridsClass <V,M>& oneDGrids,
                          const uqArrayOfOneDTablesClass<V,M>& mdfValues);
 ~uqSampledVectorMdfClass();

  void values(const V& paramValues, V& mdfVec) const;
  void print (std::ostream& os)                const;

protected:
  using uqBaseVectorMdfClass<V,M>::m_env;
  using uqBaseVectorMdfClass<V,M>::m_prefix;
  using uqBaseVectorMdfClass<V,M>::m_domainSet;

  const uqArrayOfOneDGridsClass <V,M>& m_oneDGrids;
  const uqArrayOfOneDTablesClass<V,M>& m_mdfValues;
};

template<class V,class M>
uqSampledVectorMdfClass<V,M>::uqSampledVectorMdfClass(
  const char*                          prefix,
  const uqArrayOfOneDGridsClass <V,M>& oneDGrids,
  const uqArrayOfOneDTablesClass<V,M>& mdfValues)
  :
  uqBaseVectorMdfClass<V,M>(prefix,oneDGrids.rowSpace()),
  m_oneDGrids(oneDGrids),
  m_mdfValues(mdfValues)
{
  if ((m_env.subScreenFile()) && (m_env.verbosity() >= 5)) {
    *m_env.subScreenFile() << "Entering uqSampledVectorMdfClass<V,M>::constructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }

  if ((m_env.subScreenFile()) && (m_env.verbosity() >= 5)) {
    *m_env.subScreenFile() << "Leaving uqSampledVectorMdfClass<V,M>::constructor()"
                           << ": prefix = " << m_prefix
                           << std::endl;
  }
}

template<class V,class M>
uqSampledVectorMdfClass<V,M>::~uqSampledVectorMdfClass()
{
}

template<class V, class M>
void
uqSampledVectorMdfClass<V,M>::values(
  const V& paramValues,
        V& mdfVec) const
{
  UQ_FATAL_TEST_MACRO(true,
                      m_env.rank(),
                      "uqSampledVectorMdfClass<V,M>::mdfVec()",
                      "incomplete code");
  return;
}

template <class V, class M>
void
uqSampledVectorMdfClass<V,M>::print(std::ostream& os) const
{
  // Print values *of* grid points
  os << m_oneDGrids;

  // Print *mdf* values *at* grid points
  os << m_mdfValues;

  return;
}

#endif // __UQ_VECTOR_MARGINAL_DENSITY_FUNCTION_H__