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
 *  \file   beagle/src/FitnessSimple.cpp
 *  \brief  Source code of class FitnessSimple.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <cmath>
#include <cfloat>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Default construct a simple fitness object.
 */
FitnessSimple::FitnessSimple() :
  Fitness(false)
{ }


/*!
 *  \brief Construct a valid simple fitness object.
 *  \param inFitness Value of the fitness.
 */
FitnessSimple::FitnessSimple(float inFitness) :
  Fitness(true)
{
  Beagle_StackTraceBeginM();
  setValue(inFitness);
  Beagle_StackTraceEndM("FitnessSimple::FitnessSimple(float inFitness)");
}


/*!
 *  \brief Test equality of two simple fitness measure.
 *  \param inRightObj Right measure to compare.
 *  \return True if the fitness are equal, false if not.
 */
bool FitnessSimple::isEqual(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const FitnessSimple& lRightFitness = castObjectT<const FitnessSimple&>(inRightObj);
  if(isValid() != lRightFitness.isValid()) return false;
  if((isValid()==false) && (lRightFitness.isValid()==false)) return true;
  return mFitness == lRightFitness.mFitness;
  Beagle_StackTraceEndM("bool FitnessSimple::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief Test order of two simple fitness measure.
 *  \param inRightObj Right measure to compare.
 *  \return True if the actual fitness is less from the right one, false if not.
 */
bool FitnessSimple::isLess(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const FitnessSimple& lRightFitness = castObjectT<const FitnessSimple&>(inRightObj);
  if(isValid() != lRightFitness.isValid()) return false;
  if((isValid()==false) && (lRightFitness.isValid()==false)) return false;
  return mFitness < lRightFitness.mFitness;
  Beagle_StackTraceEndM("bool FitnessSimple::isLess(const Object& inRightObj) const");
}


/*!
 *  \brief Read a simple fitness from a XML subtree.
 *  \param inIter XML iterator to use to read the fitness value.
 */
void FitnessSimple::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Fitness"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Fitness> expected!");

  string lValid = inIter->getAttribute("valid").c_str();
  if(lValid.empty() || (lValid == "yes")) {
    string lType = inIter->getAttribute("type").c_str();
    if(lType != "simple")
      throw Beagle_IOExceptionNodeM((*inIter), "fitness type mismatch!");

    PACC::XML::ConstIterator lChild = inIter->getFirstChild();
    if(!lChild) throw Beagle_IOExceptionNodeM(*inIter, "no fitness value present!");
    if(lChild->getType() != PACC::XML::eString)
      throw Beagle_IOExceptionNodeM(*inIter, "no fitness value present!");
    mFitness = str2dbl(lChild->getValue().c_str());
    setValid();
  }
  else if(lValid == "no") setInvalid();
  else throw Beagle_IOExceptionNodeM((*inIter), "bad fitness validity value!");
  Beagle_StackTraceEndM("void FitnessSimple::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Set the fitness value and validity.
 *  \param inFitness New fitness value.
 */
void FitnessSimple::setValue(float inFitness)
{
  Beagle_StackTraceBeginM();
  if(isFinite(inFitness)) mFitness = inFitness;
  else mFitness = FLT_MIN;
  setValid();
  Beagle_StackTraceEndM("void FitnessSimple::setValue(float inFitness)");
}


/*!
 *  \brief Write a simple fitness into an XML streamer.
 *  \param ioStreamer XML streamer to use to write the fitness value.
 *  \param inIndent Whether XML output should be indented.
 */
void FitnessSimple::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Fitness", false);
  ioStreamer.insertAttribute("type", "simple");
  if(isValid()) ioStreamer.insertStringContent(dbl2str(mFitness).c_str());
  else ioStreamer.insertAttribute("valid", "no");
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void FitnessSimple::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

