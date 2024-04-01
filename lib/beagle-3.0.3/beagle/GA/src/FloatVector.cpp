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
 *  \file   beagle/GA/src/FloatVector.cpp
 *  \brief  Source code of class GA::FloatVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17.2.1 $
 *  $Date: 2007/05/09 01:51:03 $
 */

#include "beagle/GA.hpp"

#include <algorithm>
#include <cfloat>
#include <float.h>    // To get FLT_MAX on Borland C++ Builder

using namespace Beagle;


/*!
 *  \brief Construct a GA float vector.
 *  \param inSize Size of the vector.
 *  \param inModel Default values of the floats.
 */
GA::FloatVector::FloatVector(unsigned int inSize, float inModel) :
  std::vector< double,BEAGLE_STLALLOCATOR<double> >(inSize, inModel)
{ }


/*!
 *  \brief Return size (length) of genotype.
 *  \return Size (length) of genotype.
 */
unsigned int GA::FloatVector::getSize() const
{
  Beagle_StackTraceBeginM();
  return size();
  Beagle_StackTraceEndM("unsigned int GA::FloatVector::getSize() const");
}


/*!
 *  \brief  Test if two float vectors are equals.
 *  \param  inRightObj Second float vector used for the comparison.
 *  \return True if float vectors are equal, false if not.
 *  \par Note:
 *    Returns true if the range [ 0, min(end(),inRightObj.end()) )
 *    are identical when compared element-by-element, and otherwise returns false.
 */
bool GA::FloatVector::isEqual(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const GA::FloatVector& lRightVector = castObjectT<const GA::FloatVector&>(inRightObj);
  if(size()!=lRightVector.size()) return false;
  const std::vector< double,BEAGLE_STLALLOCATOR<double> >& lLeft = *this;
  const std::vector< double,BEAGLE_STLALLOCATOR<double> >& lRight = lRightVector;
  return (lLeft == lRight);
  Beagle_StackTraceEndM("bool GA::FloatVector::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief  Test the ordering of two float vectors.
 *  \param  inRightObj Second float vector used for the ordering test.
 *  \return True if the actual float vector is before the second, false if not.
 *  \par Note:
 *    Returns true if the actual float vector is lexicographically less than the right object
 *    in the range [ 0, min(end(),inRightObj.end()) ) when compared element-by-element,
 *    and otherwise returns false.
 */
bool GA::FloatVector::isLess(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const GA::FloatVector& lRightVector = castObjectT<const GA::FloatVector&>(inRightObj);
  if(size()!=lRightVector.size()) return false;
  const std::vector< double,BEAGLE_STLALLOCATOR<double> >& lLeft = *this;
  const std::vector< double,BEAGLE_STLALLOCATOR<double> >& lRight = lRightVector;
  return (lLeft < lRight);
  Beagle_StackTraceEndM("bool GA::FloatVector::isLess(const Object& inRightObj) const");
}


/*!
 *  \brief Read a float vector from a XML subtree.
 *  \param inIter XML iterator from which the float vector is read.
 *  \param ioContext Evolutionary context to use to read the float vector.
 *  \throw Beagle::IOException If a reading error occured (generally bad file format)!
 */
void GA::FloatVector::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Genotype"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Genotype> expected!");

  string lType = inIter->getAttribute("type").c_str();
  if(lType.empty())
    throw Beagle_IOExceptionNodeM(*inIter, "float vector type of the genotype is not present!");
  if(lType != "floatvector")
    throw Beagle_IOExceptionNodeM(*inIter, "type of genotype mismatch, expected \"floatvector\"!");

  PACC::XML::ConstIterator lChild = inIter->getFirstChild();
  if((!lChild) || (lChild->getType()!=PACC::XML::eString))
    throw Beagle_IOExceptionNodeM(*lChild, "expected content for the float vector!");

  resize(0);
  std::istringstream lISS(lChild->getValue());
  while(lISS.good()) {
    double lValue=DBL_MAX;
    lISS >> lValue;
    push_back(lValue);
    if(lISS.good()==false) break;
    int lDelim=lISS.get();
    if((lISS.good()==false) || (lDelim==-1)) break;
  }
  Beagle_StackTraceEndM("void GA::FloatVector::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)");
}


/*!
 *  \brief Write a float vector into a XML streamer.
 *  \param ioStreamer XML streamer into which the float vector is written.
 *  \param inIndent Whether XML output should be indented.
 */
void GA::FloatVector::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Genotype", false);
  ioStreamer.insertAttribute("type", "floatvector");
  ioStreamer.insertAttribute("size", uint2str(size()));
  std::ostringstream lOSS;
  for(size_type i=0; i<size(); ++i) {
    lOSS << (*this)[i];
    if(i!=(size()-1)) lOSS << ';';
  }
  ioStreamer.insertStringContent(lOSS.str());
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void GA::FloatVector::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
