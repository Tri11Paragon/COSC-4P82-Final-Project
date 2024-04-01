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
 *  \file   beagle/src/FitnessMultiObjMin.cpp
 *  \brief  Source code of class FitnessMultiObjMin.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a minimization multiobjective fitness object.
 *  \brief inSize Size of the multiobjective fitness value.
 *  \brief inValue Default initial value of the multiobjective fitness measures.
 */
FitnessMultiObjMin::FitnessMultiObjMin(unsigned int inSize, float inValue) :
  FitnessMultiObj(inSize, inValue)
{ }


/*!
 *  \brief Construct a valid minimization multiobjective fitness object.
 *  \param inFitness Value of the fitness.
 */
FitnessMultiObjMin::FitnessMultiObjMin(const std::vector< float,BEAGLE_STLALLOCATOR<float> >& inFitness) :
  FitnessMultiObj(inFitness)
{ }


/*!
 *  \brief Test whether the actual minimization multiobjective fitness is Pareto-dominated.
 *  \param inRightFitness Second multiobjective fitness to test domination on.
 *  \return True if the actual multiobjective measure is dominated, false if not.
 */
bool FitnessMultiObjMin::isDominated(const FitnessMultiObj& inRightFitness) const
{
  Beagle_StackTraceBeginM();
  if(isValid() != inRightFitness.isValid()) return false;
  if((isValid()==false) && (inRightFitness.isValid()==false)) return false;
  size_type lMinSize = minOf(size(), inRightFitness.size());
  if(lMinSize == 0) return false;
  bool lNotEqual = false;
  for(size_type i=0; i<lMinSize; ++i) {
    if((*this)[i] < inRightFitness[i]) return false;
    else if((*this)[i] > inRightFitness[i]) lNotEqual = true;
  }
  return lNotEqual;
  Beagle_StackTraceEndM("bool FitnessMultiObjMin::isDominated(const FitnessMultiObj& inRightFitness) const");
}


/*!
 *  \brief Test order of two minimization multiobjective fitness measures.
 *  \param inRightObj Right measure to compare.
 *  \return True if the actual fitness is less from the right one, false if not.
 */
bool FitnessMultiObjMin::isLess(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const FitnessMultiObjMin& lRightFitness = castObjectT<const FitnessMultiObjMin&>(inRightObj);
  if(isValid() != lRightFitness.isValid()) return false;
  if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
  size_type lMinSize = minOf(size(), lRightFitness.size());
  for(size_type i=0; i<lMinSize; ++i) {
    if((*this)[i] > lRightFitness[i]) return true;
    else if((*this)[i] < lRightFitness[i]) return false;
  }
  return false;
  Beagle_StackTraceEndM("bool FitnessMultiObjMin::isLess(const Object& inRightObj) const");
}

