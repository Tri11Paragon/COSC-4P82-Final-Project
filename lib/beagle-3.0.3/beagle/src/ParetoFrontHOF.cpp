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
 *  \file   beagle/src/ParetoFrontHOF.cpp
 *  \brief  Source code of class ParetoFrontHOF.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:21 $
 */

#include "beagle/Beagle.hpp"

#include <algorithm>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct an pareto front hall-of-fame.
 *  \param inIndivAlloc Allocator of individuals in the hall-of-fame.
 */
ParetoFrontHOF::ParetoFrontHOF(Individual::Alloc::Handle inIndivAlloc) :
  HallOfFame(inIndivAlloc)
{ }


/*!
 *  \brief Update the Pareto front hall-of-fame with the actual deme's individuals.
 *  \param inSizeHOF Value of 0 mean no Pareto front hall-of-fame, otherwize ignored.
 *  \param inDeme Deme to add to the hall-of-fame.
 *  \param ioContext Evolution context.
 *  \return True if the Pareto front has been modified, false if not.
 */
bool ParetoFrontHOF::updateWithDeme(unsigned int inSizeHOF, const Deme& inDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(inSizeHOF == 0) {
    if(mMembers.size() != 0) {
      mMembers.clear();
      return true;
    }
    return false;
  }

  unsigned int lIndividualsAdded=0;
  unsigned int lIndividualsRemoved=0;

  for(unsigned int i=0; i<inDeme.size(); ++i) {
    bool lIsDominated=false;
    FitnessMultiObj::Handle lIndivFitness =
      castHandleT<FitnessMultiObj>(inDeme[i]->getFitness());
    for(unsigned int j=0; j<mMembers.size(); ++j) {
      FitnessMultiObj::Handle lMemberFitness =
        castHandleT<FitnessMultiObj>(mMembers[j].mIndividual->getFitness());
      if(lIndivFitness->isDominated(*lMemberFitness)) {
        lIsDominated=true;
        break;
      }
      else if(lMemberFitness->isDominated(*lIndivFitness)) {
        Beagle_LogDebugM(
          ioContext.getSystem().getLogger(),
          "hall-of-fame", "Beagle::ParetoFrontHOF",
          string("Individual removed from Pareto front: ")+
          mMembers[j].mIndividual->serialize()
        );
        mMembers.erase(mMembers.begin()+j);
        --j;
        ++lIndividualsRemoved;
      }
    }

    if(lIsDominated==false) {
      // Test if the individual is unique, and if so add it to the Pareto front.
      bool lIsIdentical = false;
      for(unsigned int k=0; k<mMembers.size(); ++k) {
        if(inDeme[i]->isIdentical(*mMembers[k].mIndividual)) {
          lIsIdentical = true;
          break;
        }
      }
      if(lIsIdentical == false) {
        HallOfFame::Member lMember(castHandleT<Individual>(getIndivAlloc()->cloneData(*inDeme[i])),
                                   ioContext.getGeneration(), ioContext.getDemeIndex());
        mMembers.push_back(lMember);
        Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
          "hall-of-fame", "Beagle::ParetoFrontHOF",
          string("Individual added to Pareto front: ")+inDeme[i]->serialize()
        );
        ++lIndividualsAdded;
      }
    }
  }

  if(lIndividualsAdded != 0) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "hall-of-fame", "Beagle::ParetoFrontHOF",
      string("There is ")+uint2str(lIndividualsAdded)+
      string(" individuals added to the Pareto front")
    );
  }

  if(lIndividualsRemoved != 0) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "hall-of-fame", "Beagle::ParetoFrontHOF",
      string("There is ")+uint2str(lIndividualsRemoved)+
      string(" individuals removed from the Pareto front")
    );
  }

  // Return if Pareto front has changed.
  return (lIndividualsAdded == 0) && (lIndividualsRemoved == 0);
  Beagle_StackTraceEndM("bool ParetoFrontHOF::updateWithDeme(unsigned int inSizeHOF, const Deme& inDeme, Context& ioContext)");
}


/*!
 *  \brief Check if an individual should be added to the Pareto front hall-of-fame, and add it if 
 *    necessary.
 *  \param inSizeHOF Value of 0 mean no Pareto front hall-of-fame, otherwize ignored.
 *  \param inIndividual Individual that is checked for addition to the Pareto front hall-of-fame.
 *  \param ioContext Evolution context.
 *  \return True if the Pareto front had been modified, false if not.
 */
bool ParetoFrontHOF::updateWithIndividual(unsigned int inSizeHOF,
                                          const Individual& inIndividual,
                                          Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(inSizeHOF == 0) {
    if(mMembers.size() != 0) {
      mMembers.clear();
      return true;
    }
    return false;
  }

  bool lParetoFrontModified=false;
  bool lIsDominated=false;
  FitnessMultiObj::Handle lIndivFitness = castHandleT<FitnessMultiObj>(inIndividual.getFitness());
  for(unsigned int i=0; i<mMembers.size(); ++i) {
    FitnessMultiObj::Handle lMemberFitness =
      castHandleT<FitnessMultiObj>(mMembers[i].mIndividual->getFitness());
    if(lIndivFitness->isDominated(*lMemberFitness)) {
      lIsDominated=true;
      break;
    }
    else if(lMemberFitness->isDominated(*lIndivFitness)) {
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "hall-of-fame", "Beagle::ParetoFrontHOF",
        string("Individual removed from Pareto front: ")+
        mMembers[i].mIndividual->serialize()
      );
      mMembers.erase(mMembers.begin()+i);
      lParetoFrontModified = true;
    }
  }
  if(lIsDominated==false) {
    // Test if the individual is unique, and if so add it to the Pareto front.
    bool lIsIdentical = false;
    for(unsigned int j=0; j<mMembers.size(); ++j) {
      if(inIndividual.isIdentical(*mMembers[j].mIndividual)) {
        lIsIdentical = true;
        break;
      }
    }
    if(lIsIdentical == false) {
      HallOfFame::Member lMember(castHandleT<Individual>(getIndivAlloc()->cloneData(inIndividual)),
                                 ioContext.getGeneration(), ioContext.getDemeIndex());
      mMembers.push_back(lMember);
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "hall-of-fame", "Beagle::ParetoFrontHOF",
        string("Individual added to Pareto front: ")+inIndividual.serialize()
      );
      lParetoFrontModified = true;
    }
  }

  // Return if Pareto front has been modified.
  return lParetoFrontModified;
  Beagle_StackTraceEndM("bool ParetoFrontHOF::updateWithIndividual(unsigned int inSizeHOF, const Individual& inIndividual, Context& ioContext)");
}


/*!
 *  \brief Write the actual Pareto front hall-of-fame in a XML streamer.
 *  \param ioStreamer XML streamer to write the Pareto front hall-of-fame into.
 *  \param inIndent Whether XML output should be indented.
 */
void ParetoFrontHOF::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  std::vector< ParetoFrontHOF::Member,BEAGLE_STLALLOCATOR<ParetoFrontHOF::Member> >
    lTemp(mMembers.begin(), mMembers.end());
  std::sort(lTemp.begin(), lTemp.end(), std::greater<HallOfFame::Member>());
  ioStreamer.openTag("HallOfFame", inIndent);
  ioStreamer.insertAttribute("type", "paretofront");
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
  Beagle_StackTraceEndM("void ParetoFrontHOF::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}





