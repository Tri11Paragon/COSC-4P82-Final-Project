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
 *  \file   beagle/src/Vivarium.cpp
 *  \brief  Source code of class Vivarium.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15.2.2 $
 *  $Date: 2007/09/10 18:24:13 $
 */

#include "beagle/Beagle.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a vivarium from a deme allocator.
 *  \param inDemeAlloc Allocator of demes.
 */
Vivarium::Vivarium(Deme::Alloc::Handle inDemeAlloc) :
  Deme::Bag(inDemeAlloc),
  mHOFAlloc(new HallOfFame::Alloc),
  mHallOfFame(new HallOfFame(inDemeAlloc->getContainerTypeAlloc())),
  mStatsAlloc(new Stats::Alloc),
  mStats(new Stats)
{ }


/*!
 *  \brief Construct a vivarium from a deme allocator and a statistics allocator.
 *  \param inDemeAlloc Allocator of demes.
 *  \param inStatsAlloc Statistics allocator.
 *  \param inHOFAlloc Hall-of-fame allocator.
 *  \param inN Number of demes of the vivarium.
 */
Vivarium::Vivarium(Deme::Alloc::Handle inDemeAlloc,
                   Stats::Alloc::Handle inStatsAlloc,
                   HallOfFame::Alloc::Handle inHOFAlloc,
                   size_type inN) :
  Deme::Bag(inDemeAlloc, inN),
  mHOFAlloc(inHOFAlloc),
  mHallOfFame(castObjectT<HallOfFame*>(inHOFAlloc->allocate())),
  mStatsAlloc(inStatsAlloc),
  mStats(castObjectT<Stats*>(inStatsAlloc->allocate()))
{
  Beagle_StackTraceBeginM();
  mHallOfFame->setIndivAlloc(inDemeAlloc->getContainerTypeAlloc());
  Beagle_StackTraceEndM("Vivarium::Vivarium(Deme::Alloc::Handle inDemeAlloc, Stats::Alloc::Handle inStatsAlloc, HallOfFame::Alloc::Handle inHOFAlloc, size_type inN)");
}


/*!
 *  \brief Construct a vivarium from a genotype allocator.
 *  \param inGenotypeAlloc Genotype allocator.
 */
Vivarium::Vivarium(Genotype::Alloc::Handle inGenotypeAlloc) :
  Deme::Bag(new Deme::Alloc(new Individual::Alloc(inGenotypeAlloc,new FitnessSimple::Alloc),
                            new Stats::Alloc, new HallOfFame::Alloc)),
  mHOFAlloc(new HallOfFame::Alloc), 
  mHallOfFame(new HallOfFame(new Individual::Alloc(inGenotypeAlloc, new FitnessSimple::Alloc))),
  mStatsAlloc(new Stats::Alloc),
  mStats(new Stats)
{ }


/*!
 *  \brief Construct a vivarium with a genotype allocator and a fitness allocator.
 *  \param inGenotypeAlloc Genotype allocator.
 *  \param inFitnessAlloc Fitness allocator.
 */
Vivarium::Vivarium(Genotype::Alloc::Handle inGenotypeAlloc, Fitness::Alloc::Handle inFitnessAlloc) :
  Deme::Bag(new Deme::Alloc(new Individual::Alloc(inGenotypeAlloc,inFitnessAlloc),
                            new Stats::Alloc, new HallOfFame::Alloc)),
  mHOFAlloc(new HallOfFame::Alloc),
  mHallOfFame(new HallOfFame(new Individual::Alloc(inGenotypeAlloc, inFitnessAlloc))),
  mStatsAlloc(new Stats::Alloc),
  mStats(new Stats)
{ }


/*!
 *  \brief Copy-construct a vivarium.
 *  \param inOrigVivarium Vivarium to copy.
 */
Vivarium::Vivarium(const Vivarium& inOrigVivarium) :
  Deme::Bag()
{
  Beagle_StackTraceBeginM();
  (*this) = inOrigVivarium;
  Beagle_StackTraceEndM("Vivarium::Vivarium(const Vivarium& inOrigVivarium)");
}


/*!
 *  \brief Copy a vivarium.
 *  \param inOrigVivarium Vivarium to copy.
 */
Vivarium& Vivarium::operator=(const Vivarium& inOrigVivarium)
{
  Beagle_StackTraceBeginM();
  if(this == &inOrigVivarium) return *this;
  Deme::Bag::operator=(inOrigVivarium);
  mHOFAlloc = inOrigVivarium.mHOFAlloc;
  mHallOfFame = castObjectT<HallOfFame*>(mHOFAlloc->clone(*inOrigVivarium.mHallOfFame));
  mStatsAlloc = inOrigVivarium.mStatsAlloc;
  mStats = castObjectT<Stats*>(mStatsAlloc->clone(*inOrigVivarium.mStats));
  return *this;
  Beagle_StackTraceEndM("Vivarium& Vivarium::operator=(const Vivarium& inOrigVivarium)");
}


/*!
 *  \brief Copy a vivarium and clone the demes that it haves.
 *  \param inOrigContainer Original vivarium to copy.
 *  \throw BadCastException If the vivarium received as argument have no type allocator.
 */
void Vivarium::copyData(const Container& inOrigContainer)
{
  Beagle_StackTraceBeginM();
  const Vivarium& lRightVivarium = castObjectT<const Vivarium&>(inOrigContainer);
  if(lRightVivarium.getTypeAlloc() == NULL) {
    string lMessage = "The copyData() method must be call only with as argument a vivarium";
    lMessage += " that have a type allocator!";
    throw Beagle_InternalExceptionM(lMessage);
  }
  if(this == &lRightVivarium) return;
  (*this) = lRightVivarium;
  clear();
  for(size_type i=0; i<lRightVivarium.size(); ++i) {
    Beagle_NonNullPointerAssertM(lRightVivarium[i]);
    push_back(getTypeAlloc()->cloneData(*lRightVivarium[i]));
  }
  Beagle_StackTraceEndM("void Vivarium::copyData(const Container& inOrigContainer)");
}


/*!
 *  \brief Reading a vivarium with method read is undefined. Use readWithContext instead.
 */
void Vivarium::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("read","Vivarium",getName());
  Beagle_StackTraceEndM("void Vivarium::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Read a vivarium from an XML node.
 *  \param inIter XML iterator to read the vivarium from.
 *  \param ioContext Evolutionary context to use to read the vivarium.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Vivarium::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Vivarium"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Vivarium> expected!");
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType()==PACC::XML::eData) {
      if(lChild->getValue() == "Stats") mStats->read(lChild);
      else if(lChild->getValue() == "HallOfFame") mHallOfFame->readWithContext(lChild, ioContext);
      else if(lChild->getValue() == "Population") readPopulation(lChild, ioContext);
    }
  }
  Beagle_StackTraceEndM("void Vivarium::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)");
}


/*!
 *  \brief Read a vivarium population from an XML node.
 *  \param inIter XML iterator to read the vivarium population from.
 *  \param ioContext Evolutionary context to use to read the vivarium population.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Vivarium::readPopulation(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Population"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Population> expected!");
  size_type lSize=0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Deme")) ++lSize;
  }
  if((getTypeAlloc()==NULL) && (lSize>size())) {
    std::ostringstream lMessageOSS;
    lMessageOSS << "Vivarium population size (" << lSize << ") is bigger than the actual size (";
    lMessageOSS << size() << "), and there is no type allocator for resizing!";
    throw Beagle_IOExceptionNodeM(*inIter, lMessageOSS.str().c_str());
  }
  resize(lSize);
  unsigned int lOldDemeIndex = ioContext.getDemeIndex();
  Deme::Handle lOldDemeHandle = ioContext.getDemeHandle();
  unsigned int lIndex=0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Deme")) {
      Beagle_NonNullPointerAssertM((*this)[lIndex]);
      ioContext.setDemeIndex(lIndex);
      ioContext.setDemeHandle((*this)[lIndex]);
      (*this)[lIndex]->readWithContext(lChild, ioContext);
      ++lIndex;
    }
  }
  ioContext.setDemeHandle(lOldDemeHandle);
  ioContext.setDemeIndex(lOldDemeIndex);
  Beagle_StackTraceEndM("void Vivarium::readPopulation(PACC::XML::ConstIterator inIter, Context& ioContext)");
}


/*!
 *  \brief Write the actual vivarium in a XML streamer.
 *  \param ioStreamer XML streamer to write the vivarium into.
 *  \param inIndent Whether XML output should be indented.
 */
void Vivarium::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Vivarium", inIndent);
  mStats->write(ioStreamer, inIndent);
  mHallOfFame->write(ioStreamer, inIndent);
  writePopulation(ioStreamer, inIndent);
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Vivarium::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


/*!
 *  \brief Write the vivarium population in a XML streamer.
 *  \param ioStreamer XML streamer to write the vivarium population into.
 *  \param inIndent Whether XML output should be indented.
 */
void Vivarium::writePopulation(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Population", inIndent);
  ioStreamer.insertAttribute("size", uint2str(size()));
  for(size_type i=0; i<size(); ++i) {
    Beagle_NonNullPointerAssertM((*this)[i]);
    (*this)[i]->write(ioStreamer, inIndent);
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Vivarium::writePopulation(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


