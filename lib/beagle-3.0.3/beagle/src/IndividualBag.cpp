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
 *  \file   beagle/src/IndividualBag.cpp
 *  \brief  Implementation of the class IndividualBag.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct individual bag.
 *  \param inIndivAlloc Allocator of individuals.
 *  \param inN Size of the bag.
 */
IndividualBag::IndividualBag(Individual::Alloc::Handle inIndivAlloc, unsigned int inN) :
  ContainerT<Individual,ContainerT<Genotype,Object::Bag>::Bag>(inIndivAlloc,inN)
{ }


/*!
 *  \brief Construct individual bag.
 *  \param inIndivAlloc Allocator of individuals.
 *  \param inN Size of the bag.
 *  \param inModel Individual model.
 */
IndividualBag::IndividualBag(Individual::Alloc::Handle inIndivAlloc, 
                             unsigned int inN, 
                             const Individual& inModel) :
  ContainerT<Individual,ContainerT<Genotype,Object::Bag>::Bag>(inIndivAlloc,inN,inModel)
{ }


/*!
 *  \brief Extract an individual bag from a XML subtree.
 *  \throw Beagle::InternalException You should use readWithContext instead.
 */
void IndividualBag::read(PACC::XML::ConstIterator)
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("read","IndividualBag",getName());
  Beagle_StackTraceEndM("void IndividualBag::read(PACC::XML::ConstIterator)");
}


/*!
 *  \brief Read an individual bag from a XML file.
 *  \param inFileName Filename to read individual bag from.
 *  \param ioSystem Evolutionary system used to read individual.
 *  \param inBagTag Name of the XML tag containing the list of individuals to read.
 *  \return True if an individual of the given name was found and read from file.
 *    false if nothing was found.
 */
bool IndividualBag::readFromFile(Beagle::string inFileName, 
								 System& ioSystem, 
								 Beagle::string inBagTag)
{
  Beagle_StackTraceBeginM();
  std::ifstream lIFS(inFileName.c_str());
  PACC::XML::Document lParser(lIFS, inFileName.c_str());
  lIFS.close();
  PACC::XML::ConstFinder lBagFinder = (PACC::XML::ConstIterator)lParser.getFirstDataTag();
  PACC::XML::ConstIterator lIndivTag = lBagFinder.find(std::string("//")+inBagTag.c_str());
  if(!lIndivTag) return false;

  Context::Handle lContext = castHandleT<Context>(ioSystem.getContextAllocator().allocate());
  lContext->setSystemHandle(&ioSystem);
  resize(0);
  Beagle_LogInfoM(
    ioSystem.getLogger(),
    "individual", "Beagle::IndividualBag",
    string("Reading an individual bag from file \"")+inFileName+
    string("\" that is in-between the XML tags \"")+inBagTag+string("\"")
  );

  for(PACC::XML::ConstIterator lIter=lIndivTag; lIter; ++lIter) {
    if((lIter->getType()!=PACC::XML::eData) || (lIter->getValue()!="Individual")) continue;
    const unsigned int lBackIndex = size();
    resize(lBackIndex+1);
    lContext->setIndividualHandle((*this)[lBackIndex]);
    lContext->setIndividualIndex(lBackIndex);
    (*this)[lBackIndex]->readWithContext(lIter, *lContext);
    Beagle_LogInfoM(
      ioSystem.getLogger(),
      "individual", "Beagle::IndividualBag",
      string("Read individual: ")+(*this)[lBackIndex]->serialize()
    );
  }

  return true;
  Beagle_StackTraceEndM("bool IndividualBag::readFromFile(string inFileName, System& ioSystem, string inBagTag)");
}


/*!
 *  \brief Read an individual bag with a context.
 *  \param inIter XML iterator to read the bag from.
 *  \param ioContext Evolutionary context.
 */
void IndividualBag::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  // Check for correct XML tag
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Bag"))
    throw Beagle_IOExceptionNodeM(*inIter, string("tag <Bag> expected!"));

  // Get size of the bag
  PACC::XML::ConstIterator lChild = inIter->getFirstChild();
  size_type lSize;
  for(lSize = 0; lChild; ++lSize) ++lChild;

  // Resize bag
  if((getTypeAlloc()==NULL) && (lSize>size())) {
    std::ostringstream lMessageOSS;
    lMessageOSS << "Bag size (" << lSize << ") is bigger than the actual size (";
    lMessageOSS << size() << "), and there is no type allocator to resize the container!";
    throw Beagle_IOExceptionNodeM(*inIter, lMessageOSS.str().c_str());
  }
  resize(lSize);

  // Read individuals with context
  lChild = inIter->getFirstChild();
  for(size_type j=0; lChild; ++j) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="NullHandle")) (*this)[j] = NULL;
    else {
      Beagle_NonNullPointerAssertM((*this)[j]);
      (*this)[j]->readWithContext(lChild,ioContext);
    }
    ++lChild;
  }
  Beagle_StackTraceEndM("void IndividualBag::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)");
}
