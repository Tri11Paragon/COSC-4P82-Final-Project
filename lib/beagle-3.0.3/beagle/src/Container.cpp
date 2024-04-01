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
 *  \file   beagle/src/Container.cpp
 *  \brief  Source code of class Container.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Default constructor.
 *  \param inTypeAlloc Allocator for elements.
 *  \param inN Initial number of elements.
 */
Container::Container(Object::Alloc::Handle inTypeAlloc, unsigned int inN) :
  std::vector< Object::Handle, BEAGLE_STLALLOCATOR<Object::Handle> >(inN),
  mTypeAlloc(inTypeAlloc)
{
  Beagle_StackTraceBeginM();
  if(mTypeAlloc != NULL) {
    for(unsigned int i=0; i<inN; i++) (*this)[i] = mTypeAlloc->allocate();
  }
  Beagle_StackTraceEndM("Container::Container(Object::Alloc::Handle inTypeAlloc, unsigned int inN)");
}


/*!
 *  \brief Constructor with cloning initializer.
 *  \param inTypeAlloc Allocator for elements.
 *  \param inN Initial number of elements.
 *  \param inModel Cloning object.
 *  \par Note:
 *    Container elements are initialized with a clone of object inModel.
 */
Container::Container(Object::Alloc::Handle inTypeAlloc, unsigned int inN, const Object& inModel) :
  std::vector< Object::Handle,BEAGLE_STLALLOCATOR<Object::Handle> >(inN),
  mTypeAlloc(inTypeAlloc)
{
  Beagle_StackTraceBeginM();
  if(mTypeAlloc != NULL) {
    for(unsigned int i=0; i<inN; i++) (*this)[i] = mTypeAlloc->clone(inModel);
  }
  Beagle_StackTraceEndM("Container::Container(Object::Alloc::Handle inTypeAlloc, unsigned int inN, const Object& inModel)");
}


/*!
 *  \brief Copy a container and clone the values that it haves.
 *  \param inOriginalContainer Original container to copy.
 *  \throw BadCastException If the deme received as argument have no type allocator.
 */
void Container::copyData(const Container& inOriginalContainer)
{
  Beagle_StackTraceBeginM();
  if(inOriginalContainer.mTypeAlloc == NULL) {
    string lMessage = "The copyData() method must be call only with as argument a container";
    lMessage += " that have a type allocator!";
    throw Beagle_InternalExceptionM(lMessage);
  }
  if(this == &inOriginalContainer) return;
  (*this) = inOriginalContainer;
  clear();
  for(unsigned int i=0; i<inOriginalContainer.size(); ++i) {
    Beagle_NonNullPointerAssertM(inOriginalContainer[i]);
    push_back(mTypeAlloc->clone(*inOriginalContainer[i]));
  }
  Beagle_StackTraceEndM("void Container::copyData(const Container& inOriginalContainer)");
}


/*!
 *  \brief Compare equality of two containers.
 *  \param inRightObj Container to compare with *this.
 *  \return True if containers are equals, false otherwise.
 *  \throw BadCastException If compared containers are not of the same type.
 *  \par Note:
 *    Returns false if the container does not have the same size, returns true if the
 *    range [ 0, size()) are identical when compared element-by-element, and otherwise returns false.
 */
bool Container::isEqual(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const Container& lRightContainer = castObjectT<const Container&>(inRightObj);
  if(size() != lRightContainer.size()) return false;
  Container::const_iterator lFirstIter1 = begin();
  Container::const_iterator lLastIter1  = begin() + size();
  Container::const_iterator lFirstIter2 = lRightContainer.begin();
  return std::equal(lFirstIter1, lLastIter1, lFirstIter2, IsEqualPointerPredicate());
  Beagle_StackTraceEndM("bool Container::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief Evaluate lexicographical order of two containers.
 *  \param inRightObj Container to compare with *this.
 *  \return True if left container is less that the right one, false otherwise.
 *  \throw BadCastException If compared containers are not of the same type.
 *  \par Note:
 *    Returns true if the actual object is lexicographically less than the right object
 *    in the range [ 0, min(end(),inRightObj.end()) ) when compared element-by-element,
 *    and otherwise returns false.
 */
bool Container::isLess(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const Container& lRightContainer = castObjectT<const Container&>(inRightObj);
  unsigned int lSizeCompared = minOf<unsigned int>(size(), lRightContainer.size());
  Container::const_iterator lFirstIter1 = begin();
  Container::const_iterator lLastIter1  = begin() + lSizeCompared;
  Container::const_iterator lFirstIter2 = lRightContainer.begin();
  Container::const_iterator lLastIter2  = lRightContainer.begin() + lSizeCompared;
  return std::lexicographical_compare(lFirstIter1,
                                      lLastIter1,
                                      lFirstIter2,
                                      lLastIter2,
                                      IsLessPointerPredicate());
  Beagle_StackTraceEndM("bool Container::isLess(const Object& inRightObj) const");
}


/*!
 *  \brief Read container from XML iterator.
 *  \param inIter XML iterator to read container from.
 *  \throw Beagle::IOException If any read error occur.
 */
void Container::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Bag"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Bag> expected!");
  PACC::XML::ConstIterator lChild = inIter->getFirstChild();
  unsigned int lSize;
  for(lSize = 0; lChild; ++lSize) ++lChild;
  if((!mTypeAlloc) && (lSize > size())) {
    std::ostringstream lMessageOSS;
    lMessageOSS << "Bag size (" << lSize << ") is bigger than the actual size (";
    lMessageOSS << size() << "), and there is no type allocator to resize the container!";
    throw Beagle_IOExceptionNodeM(*inIter, lMessageOSS.str().c_str());
  }
  resize(lSize);
  lChild = inIter->getFirstChild();
  for(unsigned int j=0; lChild; ++j) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="NullHandle")) {
      (*this)[j] = NULL;
    }
    else {
      Beagle_NonNullPointerAssertM((*this)[j]);
      (*this)[j]->read(lChild);
    }
    ++lChild;
  }
  Beagle_StackTraceEndM("void Container::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Resize container and, if necessary, allocate new elements.
 *  \param inN New number of elements.
 */
void Container::resize(unsigned int inN)
{
  Beagle_StackTraceBeginM();
  unsigned int lActualSize = size();
  std::vector< Object::Handle,BEAGLE_STLALLOCATOR<Object::Handle> >::resize(inN);
  if(mTypeAlloc != NULL && inN > lActualSize)  {
    for(unsigned int i=lActualSize; i<inN; ++i) (*this)[i] = mTypeAlloc->allocate();
  }
  Beagle_StackTraceEndM("void Container::resize(unsigned int inN)");
}

/*!
 *  \brief Resize container and, if necessary, allocate new elements using object clones.
 *  \param inN New number of elements.
 *  \param inModel cloning object.
 */
void Container::resize(unsigned int inN, const Object& inModel)
{
  Beagle_StackTraceBeginM();
  unsigned int lActualSize = size();
  std::vector< Object::Handle,BEAGLE_STLALLOCATOR<Object::Handle> >::resize(inN);
  if(mTypeAlloc != NULL && inN > lActualSize)  {
    for(unsigned int i=lActualSize; i<inN; ++i) (*this)[i] = mTypeAlloc->clone(inModel);
  }
  Beagle_StackTraceEndM("void Container::resize(unsigned int inN, const Object& inModel)");
}

/*!
 *  \brief Write Container into XML streamer.
 *  \param ioStreamer XML streamer.
 *  \param inIndent Whether XML output should be indented.
 */
void Container::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Bag", inIndent);
  for(unsigned int i=0; i<size(); ++i) {
    if((*this)[i] == NULL) {
      ioStreamer.openTag("NullHandle");
      ioStreamer.closeTag();
    }
    else (*this)[i]->write(ioStreamer, inIndent);
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Container::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

