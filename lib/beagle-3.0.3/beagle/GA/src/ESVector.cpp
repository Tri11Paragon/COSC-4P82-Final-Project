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
 *  \file   beagle/GA/src/ESVector.cpp
 *  \brief  Source code of class GA::ESVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15.2.1 $
 *  $Date: 2007/05/09 01:51:02 $
 */

#include "beagle/GA.hpp"

#include <algorithm>
#include <cfloat>
#include <float.h>    // To get FLT_MAX on Borland C++ Builder

using namespace Beagle;


/*!
 *  \brief Construct a ES pair element.
 *  \param inValue Value of the ES pair.
 *  \param inStrategy Strategy parameter of the ES pair.
 */
GA::ESPair::ESPair(double inValue, double inStrategy) :
  mValue(inValue),
  mStrategy(inStrategy)
{ }


/*!
 *  \brief Compare equality of two ES pair.
 *  \param inRightPair Second ES pair to compare with the actual one.
 *  \return True if both value and strategy of pairs are equal, false if not.
 */
bool GA::ESPair::operator==(const GA::ESPair& inRightPair) const
{
  Beagle_StackTraceBeginM();
  return ((mValue == inRightPair.mValue) && (mStrategy == inRightPair.mStrategy));
  Beagle_StackTraceEndM("bool GA::ESPair::operator==(const GA::ESPair& inRightPair) const");
}


/*!
 *  \brief Construct an ES vector.
 *  \param inSize Size of the vector.
 *  \param inModel Default ES pair value.
 */
GA::ESVector::ESVector(unsigned int inSize, GA::ESPair inModel) :
  std::vector< GA::ESPair,BEAGLE_STLALLOCATOR<GA::ESPair> >(inSize, inModel)
{ }


/*!
 *  \brief Return size (length) of genotype.
 *  \return Size (length) of genotype.
 */
unsigned int GA::ESVector::getSize() const
{
  Beagle_StackTraceBeginM();
  return size();
  Beagle_StackTraceEndM("unsigned int GA::ESVector::getSize() const");
}


/*!
 *  \brief  Test if two ES vectors are equals.
 *  \param  inRightObj Second ES vector used for the comparison.
 *  \return True if ES vectors are equal, false if not.
 *  \par Note:
 *    Returns true if the range [ 0, min(end(),inRightObj.end()) )
 *    are identical when compared element-by-element, and otherwise returns false.
 */
bool GA::ESVector::isEqual(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const GA::ESVector& lRightESV = castObjectT<const GA::ESVector&>(inRightObj);
  unsigned int lSizeCompared = minOf<unsigned int>(size(), lRightESV.size());
  return std::equal(begin(), begin()+lSizeCompared, lRightESV.begin());
  Beagle_StackTraceEndM("bool GA::ESVector::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief Read an ES vector from a XML subtree.
 *  \param inIter XML iterator from which the ES vector is read.
 *  \param ioContext Evolutionary context to use to read the ES vector.
 *  \throw IOException If a reading error occured (generally bad file format)!
 */
void GA::ESVector::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Genotype"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Genotype> expected!");

  string lType = inIter->getAttribute("type").c_str();
  if(lType.empty())
    throw Beagle_IOExceptionNodeM(*inIter, "ES vector type of the genotype is not present!");
  if((lType != "ESVector") && (lType != "esvector"))
    throw Beagle_IOExceptionNodeM(*inIter, "type of genotype mismatch, expected \"ESVector\"!");

  PACC::XML::ConstIterator lChild = inIter->getFirstChild();
  if((!lChild) || (lChild->getType()!=PACC::XML::eString))
    throw Beagle_IOExceptionNodeM(*lChild, "expected content for the ES vector!");

  clear();
  std::istringstream lISS(lChild->getValue());
  do {
    char lC1='\0', lC2='\0', lC3='\0';
    double lValue=DBL_MAX, lStrategy=DBL_MAX;
    lISS >> lC1 >> lValue >> lC2 >> lStrategy >> lC3;
    if((lC1!='(') || (lC2!=',') || (lC3!=')'))
      throw Beagle_IOExceptionNodeM(*lChild, "bad format of ES vector!");
    if((lValue==DBL_MAX) || (lStrategy==DBL_MAX))
      throw Beagle_IOExceptionNodeM(*lChild, "bad format of ES vector!");
    push_back(ESPair(lValue, lStrategy));
  } while(lISS.get()==int('/'));
  Beagle_StackTraceEndM("void GA::ESVector::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)");
}


/*!
 *  \brief Write an ES vector into a XML streamer.
 *  \param ioStreamer XML streamer into which the ES vector is written.
 *  \param inIndent Whether XML output should be indented.
 */
void GA::ESVector::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Genotype", false);
  ioStreamer.insertAttribute("type", "esvector");
  ioStreamer.insertAttribute("size", uint2str(size()));
  std::ostringstream lOSS;
  for(size_type i=0; i<size(); i++) {
    lOSS << '(' << (*this)[i].mValue << ',' << (*this)[i].mStrategy << ')';
    if(i!=(size()-1)) lOSS << '/';
  }
  ioStreamer.insertStringContent(lOSS.str());
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void GA::ESVector::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

