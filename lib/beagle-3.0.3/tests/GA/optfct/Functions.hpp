/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   Functions.hpp
 *  \brief  Definition of the differents functions that can be optimized.
 *  \author Christian Gagne
 *  $Revision: 1.5.2.1 $
 *  $Date: 2007/05/11 19:13:09 $
 */

#ifndef Functions_hpp
#define Functions_hpp

#include <cmath>
#include <vector>


/*!
 *  \brief Sphere function (F1).
 */
class SphereFunct {

public:

  /*!
   *  \brief Construct a Sphere function object.
   */
  SphereFunct() { }

  /*!
   *  \brief Evaluate the function on a given point.
   *  \param inX Point to evaluate the function value.
   *  \return Value of function at the given point.
   */
  inline double operator()(const std::vector<double>& inX) const
  {
    double lSum=0.0;
//     for(unsigned i=0; i<inX.size(); ++i) {
//       lSum += (inX[i] * inX[i]);
//     }
    for(unsigned i=0; i<inX.size(); ++i) {
      const double lXtrans = inX[i] - double(i)*3.;
      lSum += (lXtrans * lXtrans);
    }
    return lSum;
  }

};


/*!
 *  \brief Schwefel's function (F2).
 */
class SchwefelFunct {

public:

  /*!
   *  \brief Construct a Schwefel's function object.
   */
  SchwefelFunct() { }

  /*!
   *  \brief Evaluate the function on a given point.
   *  \param inX Point to evaluate the function value.
   *  \return Value of function at the given point.
   */
  inline double operator()(const std::vector<double>& inX) const
  {
    double lSum1=0.0, lSum2=0.0;
    for(unsigned i=0; i<inX.size(); ++i) {
      lSum2=0.0;
      for(unsigned int j=0; j<=i; ++j) {
        lSum2 += inX[j];
      }
      lSum1 += (lSum2 * lSum2);
    }
    return lSum1;
  }

};


/*!
 *  \brief F3 function.
 */
class F3Funct {

public:

  /*!
   *  \brief Construct a F3 function object.
   */
  F3Funct() { }

  /*!
   *  \brief Evaluate the function on a given point.
   *  \param inX Point to evaluate the function value.
   *  \return Value of function at the given point.
   */
  inline double operator()(const std::vector<double>& inX) const
  {
    double lSum=0.0;
    for(unsigned i=0; i<inX.size(); ++i) {
      lSum += (inX[i] * inX[i] * std::pow(1.0e6, double(i)/(double(inX.size())-1.0)));
    }
    return lSum;
  }

};


/*!
 *  \brief Rosenbrock's function (F6).
 */
class RosenbrockFunct {

public:

  /*!
   *  \brief Construct a Rosenbrock's function object.
   */
  RosenbrockFunct() { }

  /*!
   *  \brief Evaluate the function on a given point.
   *  \param inX Point to evaluate the function value.
   *  \return Value of function at the given point.
   */
  inline double operator()(const std::vector<double>& inX) const
  {
    double lSum=0.0;
    for(unsigned int i=0; i<(inX.size()-1); ++i) {
      lSum += (100.0 * std::pow(((inX[i]*inX[i])-inX[i+1]), 2.0)) +
              std::pow(inX[i], 2.0);
    }
    return lSum;
  }

};


/*!
 *  \brief Rastrigin's function (F9).
 */
class RastriginFunct {

public:

  /*!
   *  \brief Construct a Rastrigin's function object.
   */
  RastriginFunct() { }

  /*!
   *  \brief Evaluate the function on a given point.
   *  \param inX Point to evaluate the function value.
   *  \return Value of function at the given point.
   */
  inline double operator()(const std::vector<double>& inX) const
  {
    double lSum=0.0;
    for(unsigned i=0; i<inX.size(); ++i) {
      lSum += (inX[i] * inX[i]) - (10.0 * std::cos(2.0*M_PI*inX[i])) + 10.0;
    }
    return lSum;
  }

};


#endif // Functions_hpp
