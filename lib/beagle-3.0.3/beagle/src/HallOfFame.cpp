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
 *  \file   beagle/src/HallOfFame.cpp
 *  \brief  Source code of class HallOfFame.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <algorithm>

using namespace Beagle;



/*!
 *  \brief Construct a member item of the hall-of-fame.
 *  \param inIndividual Handle to the individual in the hall-of-fame.
 *  \param inGeneration Generation number the individual is added to the hall-of-fame.
 *  \param inDemeIndex Index in which the individual was at its insertion in the hall-of-fame.
 */
HallOfFame::Member::Member(Individual::Handle inIndividual,
                           unsigned int inGeneration,
                           unsigned int inDemeIndex) :
  mIndividual(inIndividual),
  mGeneration(inGeneration),
  mDemeIndex(inDemeIndex)
{ }


/*!
 *  \brief Compare equality of two members of the hall-of-fame.
 *  \param inRightMember Other member to compare with actual member.
 *  \return True if members are equal, false if they are not.
 */
bool HallOfFame::Member::operator==(const HallOfFame::Member& inRightMember) const
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(mIndividual);
  Beagle_NonNullPointerAssertM(inRightMember.mIndividual);
  return mIndividual->isEqual(*inRightMember.mIndividual);
  Beagle_StackTraceEndM("bool HallOfFame::Member::operator==(const HallOfFame::Member& inRightMember) const");
}


/*!
 *  \brief Compare order of two members of the hall-of-fame.
 *  \param inRightMember Other member to compare with actual member.
 *  \return True if left members is less than right one, false if not.
 */
bool HallOfFame::Member::operator<(const HallOfFame::Member& inRightMember) const
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(mIndividual);
  Beagle_NonNullPointerAssertM(inRightMember.mIndividual);
  return mIndividual->isLess(*inRightMember.mIndividual);
  Beagle_StackTraceEndM("bool HallOfFame::Member::operator<(const HallOfFame::Member& inRightMember) const");
}


/*!
 *  \brief Compare order of two members of the hall-of-fame.
 *  \param inRightMember Other member to compare with actual member.
 *  \return True if left members is less than right one, false if not.
 */
bool HallOfFame::Member::operator>(const HallOfFame::Member& inRightMember) const
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(mIndividual);
  Beagle_NonNullPointerAssertM(inRightMember.mIndividual);
  return inRightMember.mIndividual->isLess(*mIndividual);
  Beagle_StackTraceEndM("bool HallOfFame::Member::operator>(const HallOfFame::Member& inRightMember) const");
}


/*!
 *  \brief Construct an hall-of-fame.
 *  \param inIndivAlloc Allocator of individuals in the hall-of-fame.
 */
HallOfFame::HallOfFame(Individual::Alloc::Handle inIndivAlloc) :
  mIndivAllocator(inIndivAlloc)
{ }


/*!
 *  \brief Log individuals inserted in the hall-of-fame.
 *  \param inLogLevel Minimal log level needed to log the hall-of-fame.
 *  \param ioContext Context to use.
 */
void HallOfFame::log(Logger::LogLevel inLogLevel, Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  for(unsigned int i=0; i<size(); ++i) {
    ioContext.getSystem().getLogger().log(
      inLogLevel, "hall-of-fame", "Beagle::HallOfFame",
      string("Top ")+uint2str(i+1)+string(" of the hall-of-fame: ")+
      (*this)[i].mIndividual->serialize()
    );
  }
  Beagle_StackTraceEndM("void HallOfFame::log(Logger::LogLevel inLogLevel, Context& ioContext) const");
}


/*!
 *  \brief Reading a hall-of-fame with method read is undefined. Use readWithContext instead.
 */
void HallOfFame::read(PACC::XML::ConstIterator)
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("read","HallOfFame",getName());
  Beagle_StackTraceEndM("void HallOfFame::read(PACC::XML::ConstIterator)");
}


/*!
 *  \brief Read a deme population from an XML subtree.
 *  \param inIter XML iterator to read the deme population from.
 *  \param ioContext Evolutionary context to use to read the deme population.
 *  \throw Beagle::IOException If the format is not respected.
 */
void HallOfFame::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="HallOfFame"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <HallOfFame> expected!");
  unsigned int lSize=0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Member")) ++lSize;
  }
  if((getIndivAlloc()==NULL) && (lSize>size())) {
    std::ostringstream lMessageOSS;
    lMessageOSS << "Hall-of-fame size (" << lSize << ") is bigger than the actual size (";
    lMessageOSS << size() << "), and there is no type allocator for resizing!";
    throw Beagle_IOExceptionNodeM(*inIter, lMessageOSS.str().c_str());
  }
  resize(lSize);
  Individual::Handle lOldIndivHandle = ioContext.getIndividualHandle();
  unsigned int lIndex=0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Member")) {
      string lGenerationStr = lChild->getAttribute("generation").c_str();
      if(lGenerationStr.empty()) (*this)[lIndex].mGeneration = 0;
      else (*this)[lIndex].mGeneration = str2uint(lGenerationStr);
      string lDemeStr = lChild->getAttribute("deme").c_str();
      if(lDemeStr.empty()) (*this)[lIndex].mDemeIndex = 0;
      else (*this)[lIndex].mDemeIndex = str2uint(lDemeStr);
      PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
      if(lChild2 && (lChild2->getType()==PACC::XML::eData) && (lChild2->getValue()=="Individual")) {
        Beagle_NonNullPointerAssertM((*this)[lIndex].mIndividual);
        ioContext.setIndividualHandle((*this)[lIndex].mIndividual);
        (*this)[lIndex].mIndividual->readWithContext(lChild2, ioContext);
      }
      ++lIndex;
    }
  }
  ioContext.setIndividualHandle(lOldIndivHandle);
  Beagle_StackTraceEndM("void HallOfFame::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)");
}


/*!
 *  \brief Resize hall-of-fame.
 *  \param inNewSize New size of hall-of-fame.
 */
void HallOfFame::resize(unsigned int inNewSize)
{
  Beagle_StackTraceBeginM();
  unsigned int lActualSize = size();
  mMembers.resize(inNewSize);
  if((mIndivAllocator != NULL) && (inNewSize > lActualSize))  {
    for(unsigned int i=lActualSize; i<inNewSize; ++i) {
      (*this)[i].mIndividual = castHandleT<Individual>(mIndivAllocator->allocate());
      (*this)[i].mGeneration = 0;
      (*this)[i].mDemeIndex  = 0;
    }
  }
  Beagle_StackTraceEndM("void HallOfFame::resize(unsigned int inNewSize)");
}


/*!
 *  \brief Resize hall-of-fame and, if necessary, allocate new elements using model clones.
 *  \param inNewSize New size of hall-of-fame.
 *  \param inModel Individual to use for cloning.
 */
void HallOfFame::resize(unsigned int inNewSize, const HallOfFame::Member& inModel)
{
  Beagle_StackTraceBeginM();
  unsigned int lActualSize = size();
  mMembers.resize(inNewSize);
  if((mIndivAllocator != NULL) && (inNewSize > lActualSize))  {
    for(unsigned int i=lActualSize; i<inNewSize; ++i) {
      if(inModel.mIndividual == NULL) (*this)[i].mIndividual = NULL;
      else (*this)[i].mIndividual =
        castHandleT<Individual>(mIndivAllocator->cloneData(*inModel.mIndividual));
      (*this)[i].mGeneration = inModel.mGeneration;
      (*this)[i].mDemeIndex  = inModel.mDemeIndex;
    }
  }
  Beagle_StackTraceEndM("void HallOfFame::resize(unsigned int inNewSize, const HallOfFame::Member& inModel)");
}


/*!
 *  \brief Sort an hall-of-fame from the best to the worse individual.
 */
void HallOfFame::sort()
{
  Beagle_StackTraceBeginM();
  std::sort(mMembers.begin(), mMembers.end(), std::greater<HallOfFame::Member>());
  Beagle_StackTraceEndM("void HallOfFame::sort()");
}


/*!
 *  \brief Update the hall-of-fame with the actual deme's individuals, if necessary.
 *  \param inSizeHOF The desired size of the hall-of-fame.
 *  \param inDeme Deme to add to the hall-of-fame.
 *  \param ioContext Evolution context.
 *  \return True if the HOF had been modified, false if not.
 */
bool HallOfFame::updateWithDeme(unsigned int inSizeHOF, const Deme& inDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  // If new HOF size is zero, so empty the HOF and return.
  if(inSizeHOF == 0) {
    if(inSizeHOF != mMembers.size()) {
      mMembers.clear();
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "hall-of-fame", "Beagle::HallOfFame",
        "Hall-of-fame cleared"
      );
      return true;
    }
    else return false;
  }

  bool lHOFModified = false;             // Is the HOF modified?
  unsigned int lAddedIndividuals = 0;    // How many new individuals added to the HOF.

  // Make the hall-of-fame a heap with the worse individuals on the top.
  std::make_heap(mMembers.begin(), mMembers.end(), std::greater<HallOfFame::Member>());

  // If the heap is too big, resize it.
  while(mMembers.size() > inSizeHOF) {
    std::pop_heap(mMembers.begin(), mMembers.end(), std::greater<HallOfFame::Member>());
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "hall-of-fame", "Beagle::HallOfFame",
       string("Individual removed from hall-of-fame: ")+
       mMembers.back().mIndividual->serialize()
    );
    mMembers.pop_back();
    lHOFModified = true;
  }

  // Temporary buffer of individuals.
  Individual::Bag lTempPop;
  // Insert pointer of all the individuals of the deme in the buffer.
  lTempPop.insert(lTempPop.end(), inDeme.begin(), inDeme.end());
  // Make the buffer a STL heap with the fittest individual on the top.
  std::make_heap(lTempPop.begin(), lTempPop.end(), IsLessPointerPredicate());

  // If the old HOF is smaller than the desired new size,
  // fill with the best individuals of the temp buffer.
  while((lTempPop.empty() == false) && (mMembers.size() < inSizeHOF)) {

    // Test if individual on the top of the heap is identical to one
    // individual in the heap.
    bool lIsIdentical = false;
    for(unsigned int i=0; i<mMembers.size(); i++) {
      if(lTempPop[0]->isIdentical(*mMembers[i].mIndividual)) {
        lIsIdentical = true;
        break;
      }
    }

    // If the individual on the top of the heap is not identical, add it to the
    // new HOF.
    if(lIsIdentical == false) {
      HallOfFame::Member lMember(castHandleT<Individual>(getIndivAlloc()->cloneData(*lTempPop[0])),
                                 ioContext.getGeneration(),
                                 ioContext.getDemeIndex());
      mMembers.push_back(lMember);
      std::push_heap(mMembers.begin(), mMembers.end(), std::greater<HallOfFame::Member>());
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "hall-of-fame", "Beagle::HallOfFame",
        string("Individual added to hall-of-fame: ")+lTempPop[0]->serialize()
      );
      lAddedIndividuals++;
      lHOFModified = true;
    }

    // Remove it from the temporary buffer.
    std::pop_heap(lTempPop.begin(), lTempPop.end(), IsLessPointerPredicate());
    lTempPop.pop_back();
  }

  // Ok, now the new HOF is of the good size. Let's see if any individuals that are
  // still in the temporary buffer can be added to the new HOF.

  // While there is individuals in the temporary buffer that can be added to the new HOF.
  while((lTempPop.empty() == false) && (mMembers[0].mIndividual->isLess(*lTempPop[0]))) {

    // Test if individual on the top of the heap is identical to one
    // individual in the heap.
    bool lIsIdentical = false;
    for(unsigned int i=0; i<mMembers.size(); i++) {
      if(lTempPop[0]->isIdentical(*mMembers[i].mIndividual)) {
        lIsIdentical = true;
        break;
      }
    }

    // If the individual on the top of the heap is not identical, add it to the
    // new HOF.
    if(lIsIdentical == false) {
      // STL heap pop of the worse individual of the HOF
      std::pop_heap(mMembers.begin(), mMembers.end(), std::greater<HallOfFame::Member>());

      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "hall-of-fame", "Beagle::HallOfFame",
        string("Individual removed from hall-of-fame: ")+
        mMembers.back().mIndividual->serialize()
      );

      // Replace the popped individual with the best of the temporary buffer.
      mMembers.back().mIndividual =
        castHandleT<Individual>(getIndivAlloc()->cloneData(*lTempPop[0]));
      mMembers.back().mGeneration = ioContext.getGeneration();
      mMembers.back().mDemeIndex  = ioContext.getDemeIndex();

      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "hall-of-fame", "Beagle::HallOfFame",
        string("Individual added to hall-of-fame: ")+lTempPop[0]->serialize()
      );

      // STL push heap of the newly added individual.
      std::push_heap(mMembers.begin(), mMembers.end(), std::greater<HallOfFame::Member>());

      // A new individual is added to the HOF.
      lAddedIndividuals++;
      lHOFModified = true;
    }

    // STL heap pop of the best individual of the temporary buffer.
    std::pop_heap(lTempPop.begin(), lTempPop.end(), IsLessPointerPredicate());
    lTempPop.pop_back();
  }

  if(lHOFModified) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "hall-of-fame", "Beagle::HallOfFame",
      string("There is ")+uint2str(lAddedIndividuals)+
      string(" individuals added to the hall-of-fame")
    );
  }

  // Exit return whether the HOF has been modified.
  return lHOFModified;
  Beagle_StackTraceEndM("bool HallOfFame::updateWithDeme(unsigned int inSizeHOF, const Deme& inDeme, Context& ioContext)");
}


/*!
 *  \brief Check if an individual should be added to the hall-of-fame, and add it if necessary.
 *  \param inSizeHOF The desired size of the hall-of-fame.
 *  \param inIndividual Individual that is checked for addition to the hall-of-fame.
 *  \param ioContext Evolution context.
 *  \return True if the HOF had been modified, false if not.
 */
bool HallOfFame::updateWithIndividual(unsigned int inSizeHOF,
                                      const Individual& inIndividual,
                                      Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(inSizeHOF == 0) {
    if(inSizeHOF != mMembers.size()) {
      mMembers.clear();
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "hall-of-fame", "Beagle::HallOfFame",
        "Hall-of-fame cleared"
      );
      return true;
    }
    else return false;
  }

  std::make_heap(mMembers.begin(), mMembers.end(), std::greater<HallOfFame::Member>());

  bool lHOFModified = false;
  if(mMembers.empty() || mMembers[0].mIndividual->isLess(inIndividual)) {
    bool lIsIdentical = false;
    for(unsigned int i=0; i<mMembers.size(); ++i) {
      if(inIndividual.isIdentical(*mMembers[i].mIndividual)) {
        lIsIdentical = true;
        break;
      }
    }
    if(lIsIdentical == false) {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "hall-of-fame", "Beagle::HallOfFame",
        "An individual is added to the hall-of-fame"
      );
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "hall-of-fame", "Beagle::Deme",
        string("Individual added to hall-of-fame: ")+inIndividual.serialize()
      );
      HallOfFame::Member lMember(castHandleT<Individual>(getIndivAlloc()->cloneData(inIndividual)),
                                 ioContext.getGeneration(),
                                 ioContext.getDemeIndex());
      mMembers.push_back(lMember);
      std::push_heap(mMembers.begin(), mMembers.end(), std::greater<HallOfFame::Member>());
      lHOFModified = true;
    }
  }

  while(mMembers.size() > inSizeHOF) {
    std::pop_heap(mMembers.begin(), mMembers.end(), std::greater<HallOfFame::Member>());
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "hall-of-fame", "Beagle::HallOfFame",
       string("Individual removed from hall-of-fame: ")+
       mMembers.back().mIndividual->serialize()
    );
    mMembers.pop_back();
    lHOFModified = true;
  }

  return lHOFModified;
  Beagle_StackTraceEndM("bool HallOfFame::updateWithIndividual(unsigned int inSizeHOF, const Individual& inIndividual, Context& ioContext");
}


/*!
 *  \brief Write the actual hall-of-fame in a XML streamer.
 *  \param ioStreamer XML streamer to write the hall-of-fame into.
 *  \param inIndent Whether XML output should be indented.
 */
void HallOfFame::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  std::vector< HallOfFame::Member,BEAGLE_STLALLOCATOR<HallOfFame::Member> > lTemp(mMembers.begin(), mMembers.end());
  std::sort(lTemp.begin(), lTemp.end(), std::greater<HallOfFame::Member>());
  ioStreamer.openTag("HallOfFame", inIndent);
  ioStreamer.insertAttribute("size", uint2str(lTemp.size()));
  for(unsigned int i=0; i<lTemp.size(); ++i) {
    ioStreamer.openTag("Member", inIndent);
    ioStreamer.insertAttribute("generation", uint2str(lTemp[i].mGeneration));
    ioStreamer.insertAttribute("deme", uint2str(lTemp[i].mDemeIndex));
    Beagle_NonNullPointerAssertM(lTemp[i].mIndividual);
    lTemp[i].mIndividual->write(ioStreamer, inIndent);
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void HallOfFame::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}





