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
 *  \file   beagle/src/Deme.cpp
 *  \brief  Source code of class Deme.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16.2.2 $
 *  $Date: 2007/09/10 18:24:11 $
 */

#include "beagle/Beagle.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a deme for an individual allocator.
 *  \param inIndAlloc Handle to the allocator of individuals.
 */
Deme::Deme(Individual::Alloc::Handle inIndAlloc) :
   Individual::Bag(inIndAlloc),
   mHOFAlloc(new HallOfFame::Alloc),
   mHallOfFame(new HallOfFame(inIndAlloc)),
   mMigrationBuffer(new Individual::Bag(inIndAlloc)),
   mStatsAlloc(new Stats::Alloc),
   mStats(new Stats)
{ }


/*!
 *  \brief Construct a deme for an individual allocator and a statistics allocator.
 *  \param inIndAlloc Handle to the allocator of individuals.
 *  \param inStatsAlloc Statistics allocator.
 *  \param inHOFAlloc Hall-of-fame allocator.
 *  \param inN Size of deme.
 */
Deme::Deme(Individual::Alloc::Handle inIndAlloc,
           Stats::Alloc::Handle inStatsAlloc,
           HallOfFame::Alloc::Handle inHOFAlloc,
           size_type inN) :
   Individual::Bag(inIndAlloc, inN),
   mHOFAlloc(inHOFAlloc),
   mHallOfFame(castObjectT<HallOfFame*>(inHOFAlloc->allocate())),
   mMigrationBuffer(new Individual::Bag(inIndAlloc)),
   mStatsAlloc(inStatsAlloc),
   mStats(castObjectT<Stats*>(inStatsAlloc->allocate()))
{
  Beagle_StackTraceBeginM();
  mHallOfFame->setIndivAlloc(inIndAlloc);
  Beagle_StackTraceEndM("Deme::Deme(Individual::Alloc::Handle inIndAlloc, Stats::Alloc::Handle inStatsAlloc, HallOfFame::Alloc::Handle inHOFAlloc, size_type inN)");
}


/*!
 *  \brief Construct a deme with a genotype allocator.
 *  \param inGenotypeAlloc Genotype allocator.
 */
Deme::Deme(Genotype::Alloc::Handle inGenotypeAlloc) :
  Individual::Bag(new Individual::Alloc(inGenotypeAlloc, new FitnessSimple::Alloc)),
  mHOFAlloc(new HallOfFame::Alloc),
  mHallOfFame(new HallOfFame(new Individual::Alloc(inGenotypeAlloc, new FitnessSimple::Alloc))),
  mMigrationBuffer(new Individual::Bag(new Individual::Alloc(inGenotypeAlloc,
                                                             new FitnessSimple::Alloc))),
  mStatsAlloc(new Stats::Alloc),
  mStats(new Stats)
{ }


/*!
 *  \brief Construct a deme with a genotype allocator and a fitness allocator.
 *  \param inGenotypeAlloc Genotype allocator.
 *  \param inFitnessAlloc Fitness allocator.
 */
Deme::Deme(Genotype::Alloc::Handle inGenotypeAlloc, Fitness::Alloc::Handle inFitnessAlloc) :
  Individual::Bag(new Individual::Alloc(inGenotypeAlloc, inFitnessAlloc)),
  mHOFAlloc(new HallOfFame::Alloc),
  mHallOfFame(new HallOfFame(new Individual::Alloc(inGenotypeAlloc, inFitnessAlloc))),
  mMigrationBuffer(new Individual::Bag(new Individual::Alloc(inGenotypeAlloc, inFitnessAlloc))),
  mStatsAlloc(new Stats::Alloc),
  mStats(new Stats)
{ }


/*!
 *  \brief Copy construct a deme.
 *  \param inOriginalDeme Original deme to use as original of the actual deme.
 */
Deme::Deme(const Deme& inOriginalDeme) :
  IndividualBag(),
  mHallOfFame(new HallOfFame(inOriginalDeme.mHallOfFame->getIndivAlloc()))
{
  Beagle_StackTraceBeginM();
  (*this) = inOriginalDeme;
  Beagle_StackTraceEndM("Deme::Deme(const Deme& inOriginalDeme)");
}


/*!
 *  \brief Copy a deme.
 *  \param inOriginalDeme Original deme to use as original of the actual deme.
 */
Deme& Deme::operator=(const Deme& inOriginalDeme)
{
  Beagle_StackTraceBeginM();
  if(this == &inOriginalDeme) return *this;
  Individual::Bag::operator=(inOriginalDeme);
  mHOFAlloc = inOriginalDeme.mHOFAlloc;
  mHallOfFame = castObjectT<HallOfFame*>(mStatsAlloc->clone(*inOriginalDeme.mHallOfFame));
  mMigrationBuffer->copyData(*inOriginalDeme.mMigrationBuffer);
  mStatsAlloc = inOriginalDeme.mStatsAlloc;
  mStats = castObjectT<Stats*>(mStatsAlloc->clone(*inOriginalDeme.mStats));
  return *this;
  Beagle_StackTraceEndM("Deme& Deme::operator=(const Deme& inOriginalDeme)");
}


/*!
 *  \brief Copy a deme and clone the individuals it haves.
 *  \param inOrigContainer Original deme to copy.
 *  \throw BadCastException If the deme received as argument have no type allocator.
 */
void Deme::copyData(const Container& inOrigContainer)
{
  Beagle_StackTraceBeginM();
  const Deme& lOrigDeme = castObjectT<const Deme&>(inOrigContainer);
  if(lOrigDeme.getTypeAlloc() == NULL) {
    string lMessage = "The copyData() method must be call only with as argument a deme";
    lMessage += " that have a type allocator!";
    throw Beagle_InternalExceptionM(lMessage);
  }
  if(this == &lOrigDeme) return;
  (*this) = lOrigDeme;
  clear();
  for(size_type i=0; i<lOrigDeme.size(); ++i) {
    Beagle_NonNullPointerAssertM(lOrigDeme[i]);
    push_back(getTypeAlloc()->cloneData(*lOrigDeme[i]));
  }
  Beagle_StackTraceEndM("void Deme::copyData(const Container& inOrigContainer)");
}


/*!
 *  \brief Reading a deme with method read is undefined. Use readWithContext instead.
 */
void Deme::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("read","Deme",getName());
  Beagle_StackTraceEndM("void Deme::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Read a deme from an XML iterator.
 *  \param inIter XML iterator to read the deme from.
 *  \param ioContext Evolutionary context to use to read the deme.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Deme::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Deme"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Deme> expected!");
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      if(lChild->getValue() == "Stats") mStats->read(lChild);
      else if(lChild->getValue() == "HallOfFame") mHallOfFame->readWithContext(lChild, ioContext);
      else if(lChild->getValue() == "MigrationBuffer") readMigrationBuffer(lChild, ioContext);
      else if(lChild->getValue() == "Population") readPopulation(lChild, ioContext);
    }
  }
  Beagle_StackTraceEndM("void Deme::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)");
}


/*!
 *  \brief Read a deme migration buffer from an XML iterator.
 *  \param inIter XML iterator to read the deme migration buffer from.
 *  \param ioContext Evolutionary context to use to read the deme migration buffer.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Deme::readMigrationBuffer(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="MigrationBuffer"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <MigrationBuffer> expected!");
  Individual::Bag::size_type lSize=0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Individual")) ++lSize;
  }
  if((mMigrationBuffer->getTypeAlloc()==NULL) && (lSize>mMigrationBuffer->size())) {
    std::ostringstream lMessageOSS;
    lMessageOSS << "Deme migration buffer size (" << lSize << ") is bigger than the actual size (";
    lMessageOSS << mMigrationBuffer->size() << "), and there is no type allocator for resizing!";
    throw Beagle_IOExceptionNodeM(*inIter, lMessageOSS.str().c_str());
  }
  mMigrationBuffer->resize(lSize);
  unsigned int lOldIndivIndex = ioContext.getIndividualIndex();
  Individual::Handle lOldIndivHandle = ioContext.getIndividualHandle();
  unsigned int lIndex = 0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Individual")) {
      Beagle_NonNullPointerAssertM((*mMigrationBuffer)[lIndex]);
      ioContext.setIndividualIndex(lIndex);
      ioContext.setIndividualHandle((*mMigrationBuffer)[lIndex]);
      (*mMigrationBuffer)[lIndex]->readWithContext(lChild, ioContext);
      ++lIndex;
    }
  }
  ioContext.setIndividualHandle(lOldIndivHandle);
  ioContext.setIndividualIndex(lOldIndivIndex);
  Beagle_StackTraceEndM("void Deme::readMigrationBuffer(PACC::XML::ConstIterator inIter, Context& ioContext)");
}


/*!
 *  \brief Read a deme population from an XML iterator.
 *  \param inIter XML iterator to read the deme population from.
 *  \param ioContext Evolutionary context to use to read the deme population.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Deme::readPopulation(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Population"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Population> expected!");
  size_type lSize=0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Individual")) ++lSize;
  }
  if((getTypeAlloc()==NULL) && (lSize>size())) {
    std::ostringstream lMessageOSS;
    lMessageOSS << "Deme population size (" << lSize << ") is bigger than the actual size (";
    lMessageOSS << size() << "), and there is no type allocator for resizing!";
    throw Beagle_IOExceptionNodeM(*inIter, lMessageOSS.str().c_str());
  }
  resize(lSize);
  unsigned int lOldIndivIndex = ioContext.getIndividualIndex();
  Individual::Handle lOldIndivHandle = ioContext.getIndividualHandle();
  unsigned int lIndex=0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Individual")) {
      Beagle_NonNullPointerAssertM((*this)[lIndex]);
      ioContext.setIndividualIndex(lIndex);
      ioContext.setIndividualHandle((*this)[lIndex]);
      (*this)[lIndex]->readWithContext(lChild, ioContext);
      ++lIndex;
    }
  }
  ioContext.setIndividualHandle(lOldIndivHandle);
  ioContext.setIndividualIndex(lOldIndivIndex);
  Beagle_StackTraceEndM("void Deme::readPopulation(PACC::XML::ConstIterator inIter, Context& ioContext)");
}


/*!
 *  \brief Write the actual deme in a XML streamer.
 *  \param ioStreamer XML streamer to write the deme into.
 *  \param inIndent Whether XML output should be indented.
 */
void Deme::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Deme", inIndent);
  mStats->write(ioStreamer, inIndent);
  mHallOfFame->write(ioStreamer, inIndent);
  writeMigrationBuffer(ioStreamer, inIndent);
  writePopulation(ioStreamer, inIndent);
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Deme::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

/*!
 *  \brief Write the actual deme migration buffer in a XML streamer.
 *  \param ioStreamer XML streamer to write the deme migration buffer into.
 *  \param inIndent Whether XML output should be indented.
 */
void Deme::writeMigrationBuffer(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("MigrationBuffer", inIndent);
  ioStreamer.insertAttribute("size", uint2str(mMigrationBuffer->size()));
  for(Individual::Bag::size_type i=0; i<mMigrationBuffer->size(); ++i) {
    Beagle_NonNullPointerAssertM((*mMigrationBuffer)[i]);
    (*mMigrationBuffer)[i]->write(ioStreamer, inIndent);
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Deme::writeMigrationBuffer(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


/*!
 *  \brief Write the actual deme population in a XML streamer.
 *  \param ioStreamer XML streamer to write the deme population into.
 *  \param inIndent Whether XML output should be indented.
 */
void Deme::writePopulation(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Population", inIndent);
  ioStreamer.insertAttribute("size", uint2str(size()));
  for(size_type i=0; i<size(); ++i) {
    Beagle_NonNullPointerAssertM((*this)[i]);
    (*this)[i]->write(ioStreamer, inIndent);
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Deme::writePopulation(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

