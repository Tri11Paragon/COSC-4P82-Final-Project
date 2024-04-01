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
 *  \file   beagle/src/Vector.cpp
 *  \brief  Source code of class Vector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a Beagle Vector.
 *  \param inSize Size of the vector.
 *  \param inValue Default value of vector elements.
 */
Vector::Vector(unsigned int inSize, double inValue) :
  Beagle::Object(),
  PACC::Vector(inSize, inValue)
{ }


/*!
 *  \brief Construct a Beagle vector from a PACC matrix.
 *  \param inMatrix PACC matrix model.
 */
Vector::Vector(const PACC::Matrix& inMatrix) :
  Beagle::Object(),
  PACC::Vector(inMatrix)
{ }


/*!
 *  \brief Construct a Beagle vector from a PACC vector.
 *  \param inVector PACC vector model.
 */
Vector::Vector(const PACC::Vector& inVector) :
  Beagle::Object(),
  PACC::Vector(inVector)
{ }


/*!
 *  \brief Construct a Beagle vector from a Beagle matrix.
 *  \param inMatrix Beagle matrix model.
 */
Vector::Vector(const Beagle::Matrix& inMatrix) :
  Beagle::Object(),
  PACC::Vector(inMatrix)
{ }


/*!
 *  \brief Operator=, copy a PACC matrix into a Beagle vector.
 *  \param inMatrix PACC matrix to copy.
 *  \return Reference to actual vector.
 */
Vector& Vector::operator=(const PACC::Matrix& inMatrix)
{
  Beagle_StackTraceBeginM();
  Beagle_AssertM(inMatrix.getCols() <= 1);
  PACC::Matrix::operator=(inMatrix);
  return *this;
  Beagle_StackTraceEndM("Vector& Vector::operator=(const PACC::Matrix& inMatrix)");
}


/*!
 *  \brief Operator=, copy a PACC vector into a Beagle vector.
 *  \param inVector PACC vector to copy.
 *  \return Reference to actual vector.
 */
Vector& Vector::operator=(const PACC::Vector& inVector)
{
  Beagle_StackTraceBeginM();
  PACC::Vector::operator=(inVector);
  return *this;
  Beagle_StackTraceEndM("Vector& Vector::operator=(const PACC::Vector& inVector)");
}


/*!
 *  \brief Operator=, copy a Beagle matrix into a Beagle vector.
 *  \param inMatrix Beagle matrix to copy.
 *  \return Reference to actual vector.
 */
Vector& Vector::operator=(const Beagle::Matrix& inMatrix)
{
  Beagle_StackTraceBeginM();
  Beagle_AssertM(inMatrix.getCols() <= 1);
  PACC::Matrix::operator=(inMatrix);
  return *this;
  Beagle_StackTraceEndM("Vector& Vector::operator=(const Beagle::Matrix& inMatrix)");
}


/*!
 *  \brief Read vector from a XML subtree.
 *  \param inIter XML iterator to read the vector from.
 */
void Vector::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if(inIter->getType()!=PACC::XML::eString)
    throw Beagle_IOExceptionNodeM(*inIter,"expected string to read vector!");
  resize(0);
  std::istringstream lISS(inIter->getValue());
  while(lISS.good()) {
    double lValue;
    lISS >> lValue;
    resize(size()+1);
    (*this)[size()-1] = lValue;
    if(lISS.good()==false) break;
    int lDelim=lISS.get();
    if((lISS.good()==false) || (lDelim==-1)) break;
  }
  Beagle_StackTraceEndM("void Vector::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Write a matrix into a XML streamer.
 *  \param ioStreamer XML streamer to use.
 *  \param inIndent Whether XML output should be indented.
 */
void Vector::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  std::ostringstream lOSS;
  const unsigned int lSize = size();
  for(unsigned int i=0; i<lSize; ++i) {
    lOSS << (*this)[i];
    if(i!=(lSize-1)) lOSS << ';';
  }
  ioStreamer.insertStringContent(lOSS.str());
  Beagle_StackTraceEndM("void Vector::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
