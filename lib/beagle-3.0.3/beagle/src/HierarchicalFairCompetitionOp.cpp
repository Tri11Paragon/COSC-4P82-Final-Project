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
 *  \file   beagle/src/HierarchicalFairCompetitionOp.cpp
 *  \brief  Source code of class HierarchicalFairCompetitionOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <cmath>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Build generational operator.
 *  \param inName Name of the generational operator.
 */
HierarchicalFairCompetitionOp::HierarchicalFairCompetitionOp(Beagle::string inName) :
  ReplacementStrategyOp(inName)
{ }


/*!
 *  \brief Initialize the operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void HierarchicalFairCompetitionOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  ReplacementStrategyOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("ec.hfc.percentile")) {
    mHFCPercentile =
      castHandleT<Float>(ioSystem.getRegister().getEntry("ec.hfc.percentile"));
  } else {
    mHFCPercentile = new Float(0.85f);
    string lLongDescrip = "Percentile of fitness measure to use as HFC migration threshold ";
    lLongDescrip += "of next deme. For example, a threshold of 0.85 means that the fitness ";
    lLongDescrip += "used as threshold to accept migrant into following deme is taken as the ";
    lLongDescrip += "fitness of the individual that is better than 85% of the other individuals ";
    lLongDescrip += "in its deme.";
    Register::Description lDescrip(
      "HFC percentile",
      "Float",
      "0.85",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.hfc.percentile", mHFCPercentile, lDescrip);
  }

  if(ioSystem.getRegister().isRegistered("ec.hfc.interval")) {
    mMigrationInterval =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.hfc.interval"));
  } else {
    mMigrationInterval = new UInt(1);
    string lLongDescrip = "Interval between each hierarchical fair competition ";
    lLongDescrip += "migration, in number of generations. ";
    lLongDescrip += "An interval of 0 disables HFC migrations.";
    Register::Description lIntervalDescrip(
      "Interval between HFC migrations",
      "UInt",
      "1",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.hfc.interval", mMigrationInterval, lIntervalDescrip);
  }

  if(ioSystem.getRegister().isRegistered("ec.hfc.size")) {
    mNumberMigrants =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.hfc.size"));
  } else {
    mNumberMigrants = new UInt(5);
    string lLongDescrip = "Number of individuals migrating between each deme, at a each ";
    lLongDescrip += "hierarchical fair competition migration.";
    Register::Description lSizeDescrip(
      "Size of each HFC migration",
      "UInt",
      "5",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.hfc.size", mNumberMigrants, lSizeDescrip);
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
  Beagle_StackTraceEndM("void HierarchicalFairCompetitionOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the operation on a deme in the given context.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void HierarchicalFairCompetitionOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  // Verify fitness threshold values.
  Beagle_AssertM(mPopSize->size()!=0);
  if(ioContext.getGeneration()==0) mFitnessThresholds.resize(0);
  if(mFitnessThresholds.size()!=(mPopSize->size()-1)) mFitnessThresholds.resize(mPopSize->size()-1);

  // Situations where HFC migration should not be applied.
  if(ioDeme.size() == 0) return;
  if(mPopSize->size() < 2) return;
  if(mMigrationInterval->getWrappedValue() == 0) return;
  if((ioContext.getGeneration() % mMigrationInterval->getWrappedValue()) != 0) return;

  // Log some information.
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "migration", "Beagle::HierarchicalFairCompetitionOp",
    string("Applying HFC migration to the ")+uint2ordinal(ioContext.getDemeIndex())+
    string(" deme")
  );

  // If the deme is changed.
  bool lChanged = false;

  // Update fitness threshold.
  if(ioContext.getDemeIndex() != 0) {
    Beagle_AssertM(mHFCPercentile->getWrappedValue() < 1.0);
    const unsigned int lThresholdIndex =
      (unsigned int)std::ceil((1.0-mHFCPercentile->getWrappedValue()) * float(ioDeme.size()-1));
    std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
    for(unsigned int i=0; i<lThresholdIndex; ++i) {
      std::pop_heap(ioDeme.begin(), ioDeme.end()-i, IsLessPointerPredicate());
    }
    mFitnessThresholds[ioContext.getDemeIndex()-1] =
      castHandleT<Fitness>(ioDeme[0]->getFitnessAlloc()->clone(*ioDeme[0]->getFitness()));
  }

  // Insert migrating individuals from previous deme.
  if(ioContext.getDemeIndex() != 0) {
    Individual::Bag& lInMigBuffer =
      ioContext.getVivarium()[ioContext.getDemeIndex()-1]->getMigrationBuffer();
    for(unsigned int i=0; i<lInMigBuffer.size(); ++i) {
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "migration", "Beagle::HierarchicalFairCompetitionOp",
        string("Individual migrated into the ")+uint2ordinal(ioContext.getDemeIndex())+
        string(" deme: ")+lInMigBuffer[i]->serialize()
      );
      ioDeme.push_back(lInMigBuffer[i]);
      lChanged = true;
    }
    lInMigBuffer.resize(0);
  }

  // Migrating individual out of this deme.
  if((ioContext.getDemeIndex() != (mPopSize->size()-1)) &&
     (mFitnessThresholds[ioContext.getDemeIndex()] != NULL)) {
    Individual::Bag& lOutMigBuffer = ioDeme.getMigrationBuffer();
    lOutMigBuffer.resize(0);
    Fitness::Handle lThreshold = mFitnessThresholds[ioContext.getDemeIndex()];
    std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
    while(ioDeme.size() > 0) {
      if(ioDeme[0]->getFitness()->isLess(*lThreshold)) break;
      std::pop_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
      lOutMigBuffer.push_back(castHandleT<Individual>(ioDeme.back()));
      ioDeme.pop_back();
      lChanged = true;
    }
  }

  // Fill the population with randomly generated individuals, if the population is too small.
  if(ioDeme.size() < (*mPopSize)[ioContext.getDemeIndex()]) {
    const unsigned int lNbNewInd = (*mPopSize)[ioContext.getDemeIndex()] - ioDeme.size();
    Individual::Bag lNewIndividuals = generateIndividuals(lNbNewInd, ioDeme, ioContext);
    ioDeme.insert(ioDeme.end(), lNewIndividuals.begin(), lNewIndividuals.end());
    lChanged = true;
  }

  // Delete worse individuals if the population is too big.
  if(ioDeme.size() > (*mPopSize)[ioContext.getDemeIndex()]) {
    const unsigned int lNbDeletedInd = ioDeme.size() - (*mPopSize)[ioContext.getDemeIndex()];
    std::make_heap(ioDeme.begin(), ioDeme.end(), IsMorePointerPredicate());
    for(unsigned int i=0; i<lNbDeletedInd; ++i) {
      std::pop_heap(ioDeme.begin(), ioDeme.end(), IsMorePointerPredicate());
      Beagle_LogDebugM(
        ioContext.getSystem().getLogger(),
        "migration", "Beagle::HierarchicalFairCompetitionOp",
        string("Individual erased from the last deme: ")+
        ioDeme.back()->serialize()
      );
      ioDeme.pop_back();
    }
    lChanged = true;
  }

  // If changed, reset statistics.
  if(lChanged) {
    ioDeme.getStats()->setInvalid();
    ioContext.getVivarium().getStats()->setInvalid();
  }
  Beagle_StackTraceEndM("void HierarchicalFairCompetitionOp::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Generate a new individuals from the HFC breeder tree.
 *  \param inN Number of new individuals generated.
 *  \param ioDeme Evolved deme used to generate new individuals.
 *  \param ioContext Evolutionary context.
 *  \return New generated individuals from breeder tree.
 */
Individual::Bag
HierarchicalFairCompetitionOp::generateIndividuals(unsigned int inN,
                                                   Deme& ioDeme, Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(getRootNode());
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::HierarchicalFairCompetitionOp",
    string("Processing using HFC replacement strategy the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  Beagle_LogObjectM(
    ioContext.getSystem().getLogger(),
    Logger::eTrace,
    "replacement-strategy", "Beagle::HierarchicalFairCompetitionOp",
    (*this)
  );

  RouletteT<unsigned int> lRoulette;
  buildRoulette(lRoulette, ioContext);
  Individual::Bag lNewIndividuals;
  for(unsigned int i=0; i<inN; ++i) {
    unsigned int lIndexBreeder = lRoulette.select(ioContext.getSystem().getRandomizer());
    BreederNode::Handle lSelectedBreeder=getRootNode();
    for(unsigned int j=0; j<lIndexBreeder; ++j)
      lSelectedBreeder=lSelectedBreeder->getNextSibling();
    Beagle_NonNullPointerAssertM(lSelectedBreeder);
    Beagle_NonNullPointerAssertM(lSelectedBreeder->getBreederOp());
    Individual::Handle lBredIndiv =
      lSelectedBreeder->getBreederOp()->breed(ioDeme, lSelectedBreeder->getFirstChild(), ioContext);
    lNewIndividuals.push_back(lBredIndiv);
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "migration", "Beagle::HierarchicalFairCompetitionOp",
      string("Randomly generated individual to be inserted in first deme: ")+
      lBredIndiv->serialize()
    );
  }
  return lNewIndividuals;
  Beagle_StackTraceEndM("Individual::Bag HierarchicalFairCompetitionOp::generateIndividuals(unsigned int inN, Deme& ioDeme, Context& ioContext) const");
}


