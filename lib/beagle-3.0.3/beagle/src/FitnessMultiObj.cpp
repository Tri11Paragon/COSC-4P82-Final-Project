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
 *  \file   beagle/src/FitnessMultiObj.cpp
 *  \brief  Source code of class FitnessMultiObj.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a multiobjective fitness object.
 *  \brief inSize Size of the multiobjective fitness value.
 *  \brief inValue Default initial value of the multiobjective fitness measures.
 */
FitnessMultiObj::FitnessMultiObj(unsigned int inSize, float inValue) :
  Fitness((inSize != 0)),
  std::vector< float,BEAGLE_STLALLOCATOR<float> >(inSize, inValue)
{ }


/*!
 *  \brief Construct a valid multiobjective fitness object.
 *  \param inFitness Value of the fitness.
 */
FitnessMultiObj::FitnessMultiObj(const std::vector< float,BEAGLE_STLALLOCATOR<float> >& inFitness) :
  Fitness(true),
  std::vector< float,BEAGLE_STLALLOCATOR<float> >(inFitness)
{ }


/*!
 *  \brief Return scaling factors, 1.0 for all objectives by default.
 *  \return Scaling factor of the dimensions of the objective space.
 */
std::vector< float,BEAGLE_STLALLOCATOR<float> > FitnessMultiObj::getScalingFactors() const
{
  Beagle_StackTraceBeginM();
  return std::vector< float,BEAGLE_STLALLOCATOR<float> >(size(), 1.0);
  Beagle_StackTraceEndM("std::vector< float,BEAGLE_STLALLOCATOR<float> > FitnessMultiObj::getScalingFactors() const");
}


/*!
 *  \brief Test whether the actual multiobjective fitness is Pareto-dominated.
 *  \param inRightFitness Second multiobjective fitness to test domination on.
 *  \return True if the actual multiobjective measure is dominated, false if not.
 */
bool FitnessMultiObj::isDominated(const FitnessMultiObj& inRightFitness) const
{
  Beagle_StackTraceBeginM();
  if(isValid() != inRightFitness.isValid()) return false;
  if((isValid()==false) && (inRightFitness.isValid()==false)) return false;
  size_type lMinSize = minOf(size(), inRightFitness.size());
  if(lMinSize == 0) return false;
  bool lNotEqual = false;
  for(size_type i=0; i<lMinSize; ++i) {
    if((*this)[i] > inRightFitness[i]) return false;
    else if((*this)[i] < inRightFitness[i]) lNotEqual = true;
  }
  return lNotEqual;
  Beagle_StackTraceEndM("bool FitnessMultiObj::isDominated(const FitnessMultiObj& inRightFitness) const");
}


/*!
 *  \brief Test equality of two multiobjective fitness measures.
 *  \param inRightObj Right measure to compare.
 *  \return True if the fitness are equal, false if not.
 */
bool FitnessMultiObj::isEqual(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const FitnessMultiObj& lRightFitness = castObjectT<const FitnessMultiObj&>(inRightObj);
  if(isValid() != lRightFitness.isValid()) return false;
  if((isValid()==false) && (lRightFitness.isValid()==false)) return true;
  if(size() != lRightFitness.size()) return false;
  for(size_type i=0; i<size(); ++i) {
    if((*this)[i] != lRightFitness[i]) return false;
  }
  return true;
  Beagle_StackTraceEndM("bool FitnessMultiObj::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief Test order of two multiobjective fitness measures, from first to last objective.
 *  \param inRightObj Right measure to compare.
 *  \return True if the actual fitness is less from the right one, false if not.
 */
bool FitnessMultiObj::isLess(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const FitnessMultiObj& lRightFitness = castObjectT<const FitnessMultiObj&>(inRightObj);
  if(isValid() != lRightFitness.isValid()) return false;
  if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
  size_type lMinSize = minOf(size(), lRightFitness.size());
  for(size_type i=0; i<lMinSize; ++i) {
    if((*this)[i] < lRightFitness[i]) return true;
    else if((*this)[i] > lRightFitness[i]) return false;
  }
  return false;
  Beagle_StackTraceEndM("bool FitnessMultiObj::isLess(const Object& inRightObj) const");
}


/*!
 *  \brief Read a multiobjective fitness from a XML subtree.
 *  \param inIter XML iterator to use to read the fitness value.
 */
void FitnessMultiObj::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Fitness"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Fitness> expected!");

  string lValid = inIter->getAttribute("valid").c_str();
  if(lValid.empty() || (lValid == "yes")) {
    string lType = inIter->getAttribute("type").c_str();
    if(lType != "multiobj")
      throw Beagle_IOExceptionNodeM(*inIter, "fitness type mismatch!");

    clear();
    for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
      if((lChild->getType()!=PACC::XML::eData) || (lChild->getValue()!="Obj")) continue;
      PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
      if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "needed a float value in the <Obj> tag!");
      if(lChild2->getType() != PACC::XML::eString)
        throw Beagle_IOExceptionNodeM(*lChild2, "needed a float value in the <Obj> tag!");
      push_back(str2dbl(lChild2->getValue().c_str()));
    }
    setValid();
  }
  else if(lValid == "no") setInvalid();
  else throw Beagle_IOExceptionNodeM(*inIter, "bad fitness validity value!");
  Beagle_StackTraceEndM("void FitnessMultiObj::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Write a multiobjective fitness into an XML streamer.
 *  \param ioStreamer XML streamer to use to write the fitness value.
 *  \param inIndent Whether XML output should be indented.
 */
void FitnessMultiObj::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Fitness", inIndent);
  ioStreamer.insertAttribute("type", "multiobj");
  if(isValid()) {
    ioStreamer.insertAttribute("size", uint2str(size()));
    for(size_type i=0; i<size(); ++i) {
      ioStreamer.openTag("Obj", false);
      ioStreamer.insertStringContent(dbl2str((*this)[i]).c_str());
      ioStreamer.closeTag();
    }
  }
  else ioStreamer.insertAttribute("valid", "no"); 
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void FitnessMultiObj::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}




