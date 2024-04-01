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
 *  \file   beagle/src/ParetoFrontCalculateOp.cpp
 *  \brief  Source code of class ParetoFrontCalculateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11.2.1 $
 *  $Date: 2007/05/09 01:51:21 $
 */

#include "beagle/Beagle.hpp"

#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a Pareto front extractor operator.
 */
ParetoFrontCalculateOp::ParetoFrontCalculateOp(Beagle::string inName) :
  Operator(inName),
  mMilestonePrefix(NULL),
  mWritingInterval(NULL),
  mPerDemeMilestone(NULL),
  mPopSize(NULL)
{ }


/*!
 *  \brief Initialize the Pareto front extractor operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void ParetoFrontCalculateOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ms.write.prefix")) {
    mMilestonePrefix =
      castHandleT<String>(ioSystem.getRegister().getEntry("ms.write.prefix"));
  } else {
    mMilestonePrefix = new String("beagle");
    string lLongDescrip = "Prefix used to name the evolution milestone files. ";
    lLongDescrip += "An empty string means no milestone.";
    Register::Description lPrefixDescription(
      "Milestone filename prefix",
      "String",
      "\"beagle\"",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ms.write.prefix", mMilestonePrefix, lPrefixDescription);
  }

  if(ioSystem.getRegister().isRegistered("ms.write.interval")) {
    mWritingInterval =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ms.write.interval"));
  } else {
    mWritingInterval = new UInt(0);
    string lLongDescrip = "Milestone saving interval (in number of generations). ";
    lLongDescrip += "When zero, only the last generation milestone is saved.";
    Register::Description lIntervalDescription(
      "Milestone saving interval",
      "UInt",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ms.write.interval", mWritingInterval, lIntervalDescription);
  }

  if(ioSystem.getRegister().isRegistered("ms.write.perdeme")) {
    mPerDemeMilestone =
      castHandleT<Bool>(ioSystem.getRegister().getEntry("ms.write.perdeme"));
  } else {
    mPerDemeMilestone = new Bool(false);
    string lLongDescrip = "If true, this flag indicates that separate milestones ";
    lLongDescrip += "should be written after each demes processing. Otherwise milestones are ";
    lLongDescrip += "written after the processing of a complete populations.";
    Register::Description lPerdemeDescription(
      "Milestone per deme saving flag",
      "Bool",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ms.write.perdeme", mPerDemeMilestone, lPerdemeDescription);
  }

  if(ioSystem.getRegister().isRegistered("ec.pop.size")) {
    mPopSize = castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
  } else {
    mPopSize = new UIntArray(1,100);
    string lLongDescrip("Number of demes and size of each deme of the population. ");
    lLongDescrip += "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
    lLongDescrip += "The size of the UIntArray is the number of demes present in the ";
    lLongDescrip += "vivarium, while each value of the vector is the size of the corresponding ";
    lLongDescrip += "deme.";
    Register::Description lDescription(
      "Vivarium and demes sizes",
      "UIntArray",
      "100",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
  }
  Beagle_StackTraceEndM("void ParetoFrontCalculateOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the characteristic operation.
 *  \param ioDeme Deme to use to write the milestone.
 *  \param ioContext Context of the evolution.
 */
void ParetoFrontCalculateOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(mMilestonePrefix->getWrappedValue().empty()) return;
  if(ioContext.getContinueFlag() == true) {
    if(mWritingInterval->getWrappedValue() == 0) return;
    if((ioContext.getGeneration() % mWritingInterval->getWrappedValue()) != 0) return;
    if((mPerDemeMilestone->getWrappedValue() == false) &&
       (ioContext.getDemeIndex() != (mPopSize->size()-1))) return;
  }

  unsigned int lOldDemeIndex  = ioContext.getDemeIndex();
  Deme::Handle lOldDemeHandle = ioContext.getDemeHandle();

  for(unsigned int i=0; i<ioContext.getVivarium().size(); ++i) {
    Beagle_LogInfoM(
      ioContext.getSystem().getLogger(),
      "hall-of-fame", "Beagle::ParetoFrontCalculateOp",
      string("Computing Pareto front for the ")+uint2ordinal(i+1)+" deme"
    );
    ioContext.setDemeIndex(i);
    ioContext.setDemeHandle(ioContext.getVivarium()[i]);
    ParetoFrontHOF::Handle lParetoFront = new ParetoFrontHOF;
    lParetoFront->setIndivAlloc(ioContext.getVivarium()[i]->getTypeAlloc());
    lParetoFront->updateWithDeme(1, *ioContext.getVivarium()[i], ioContext);
    ioContext.getVivarium()[i]->setHallOfFameAlloc(new ParetoFrontHOF::Alloc);
    ioContext.getVivarium()[i]->setHallOfFame(lParetoFront);
  }

  Beagle_LogInfoM(
    ioContext.getSystem().getLogger(),
    "hall-of-fame", "Beagle::ParetoFrontCalculateOp",
    "Computing Pareto front for the whole vivarium"
  );
  ParetoFrontHOF::Handle lParetoFrontViva = new ParetoFrontHOF;
  lParetoFrontViva->setIndivAlloc(
    ioContext.getVivarium().getTypeAlloc()->getContainerTypeAlloc());
  for(unsigned int j=0; j<ioContext.getVivarium().size(); ++j) {
    ioContext.setDemeIndex(j);
    ioContext.setDemeHandle(ioContext.getVivarium()[j]);
    lParetoFrontViva->updateWithDeme(1, *ioContext.getVivarium()[j], ioContext);
  }
  ioContext.getVivarium().setHallOfFameAlloc(new ParetoFrontHOF::Alloc);
  ioContext.getVivarium().setHallOfFame(lParetoFrontViva);

  ioContext.setDemeIndex(lOldDemeIndex);
  ioContext.setDemeHandle(lOldDemeHandle);
  Beagle_StackTraceEndM("void ParetoFrontCalculateOp::operate(Deme& ioDeme, Context& ioContext)");
}
