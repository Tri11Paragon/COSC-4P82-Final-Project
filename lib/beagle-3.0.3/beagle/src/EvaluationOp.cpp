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
 *  \file   beagle/src/EvaluationOp.cpp
 *  \brief  Source code of class EvaluationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

#include <algorithm>
#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a new evaluation operator.
 *  \param inName Name of the operator.
 */
EvaluationOp::EvaluationOp(Beagle::string inName) :
  BreederOp(inName)
{ }


/*!
 *  \brief Apply the evaluation operation on a breeding pool, returning a evaluated bred individual.
 *  \param inBreedingPool Breeding pool to use for the breeding operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the breeding operation.
 *  \return Evaluated bred individual.
 */
Individual::Handle EvaluationOp::breed(Individual::Bag& inBreedingPool,
                                       BreederNode::Handle inChild,
                                       Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(inChild);

  Deme& lDeme = *ioContext.getDemeHandle();
  if(lDeme.getStats()->isValid()) prepareStats(lDeme, ioContext);

  Beagle_NonNullPointerAssertM(inChild);
  Beagle_NonNullPointerAssertM(inChild->getBreederOp());
  Individual::Handle lBredIndividual =
    inChild->getBreederOp()->breed(inBreedingPool, inChild->getFirstChild(), ioContext);

  if((lBredIndividual->getFitness()==NULL) || (lBredIndividual->getFitness()->isValid()==false)) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "evaluation", "Beagle::EvaluationOp",
      "Evaluating the fitness of a new bred individual"
    );

    lBredIndividual->setFitness(evaluate(*lBredIndividual, ioContext));
    lBredIndividual->getFitness()->setValid();

    ioContext.setProcessedDeme(ioContext.getProcessedDeme()+1);
    ioContext.setTotalProcessedDeme(ioContext.getTotalProcessedDeme()+1);
    ioContext.setProcessedVivarium(ioContext.getProcessedVivarium()+1);
    ioContext.setTotalProcessedVivarium(ioContext.getTotalProcessedVivarium()+1);

    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "evaluation", "Beagle::EvaluationOp",
      string("The individual fitness value is: ")+
      lBredIndividual->getFitness()->serialize()
    );

    updateHallOfFameWithIndividual(*lBredIndividual, ioContext);
  }

  return lBredIndividual;
  Beagle_StackTraceEndM("Individual::Handle EvaluationOp::breed(Individual::Bag&,BreederNode::Handle,Context&)");
}


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
float EvaluationOp::getBreedingProba(BreederNode::Handle inChild)
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(inChild);
  Beagle_NonNullPointerAssertM(inChild->getBreederOp());
  return inChild->getBreederOp()->getBreedingProba(inChild->getFirstChild());
  Beagle_StackTraceEndM("float EvaluationOp::getBreedingProba(BreederNode::Handle)");
}


/*!
 *  \brief Initialize the parameters evaluation operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void EvaluationOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  BreederOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("ec.hof.vivasize")) {
    mVivaHOFSize =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.hof.vivasize"));
  } else {
    mVivaHOFSize = new UInt(1);
    string lLongDescript = "Number of individuals kept in vivarium's hall-of-fame ";
    lLongDescript += "(best individuals so far). Note that a hall-of-fame contains only ";
    lLongDescript += "copies of the best individuals so far and is not used by the evolution ";
    lLongDescript += "process.";
    Register::Description lDescription(
      "Vivarium's hall-of-fame size",
      "UInt",
      "1",
      lLongDescript
    );
    ioSystem.getRegister().addEntry("ec.hof.vivasize", mVivaHOFSize, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ec.hof.demesize")) {
    mDemeHOFSize =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.hof.demesize"));
  } else {
    mDemeHOFSize = new UInt(0);
    string lLongDescript = "Number of individuals kept in each deme's hall-of-fame ";
    lLongDescript += "(best individuals so far). Note that a hall-of-fame contains only ";
    lLongDescript += "copies of the best individuals so far and is not used by the evolution ";
    lLongDescript += "process.";
    Register::Description lDescription(
      "Demes' hall-of-fame size",
      "UInt",
      "0",
      lLongDescript
    );
    ioSystem.getRegister().addEntry("ec.hof.demesize", mDemeHOFSize, lDescription);
  }
  Beagle_StackTraceEndM("void EvaluationOp::initialize(System&)");
}


/*!
 *  \brief Apply the evaluation process on the invalid individuals of the deme.
 *  \param ioDeme Deme to process.
 *  \param ioContext Context of the evolution.
 */
void EvaluationOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "evaluation", "Beagle::EvaluationOp",
    string("Evaluating the fitness of the individuals in the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );

  Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
  unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();    

  prepareStats(ioDeme,ioContext);

  for(unsigned int i=0; i<ioDeme.size(); i++) {
    if((ioDeme[i]->getFitness() == NULL) ||
       (ioDeme[i]->getFitness()->isValid() == false)) {

      Beagle_LogVerboseM(   
        ioContext.getSystem().getLogger(),
        "evaluation", "Beagle::EvaluationOp",
        string("Evaluating the fitness of the ")+uint2ordinal(i+1)+
        " individual"
      );

      ioContext.setIndividualIndex(i);
      ioContext.setIndividualHandle(ioDeme[i]);

      ioDeme[i]->setFitness(evaluate(*ioDeme[i], ioContext));
      ioDeme[i]->getFitness()->setValid();

      updateStats(1,ioContext);

      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "evaluation", "Beagle::EvaluationOp",
        string("The individual\'s fitness is: ")+
        ioDeme[i]->getFitness()->serialize()
      );
    }
  }

  ioContext.setIndividualIndex(lOldIndividualIndex);
  ioContext.setIndividualHandle(lOldIndividualHandle);

  updateHallOfFameWithDeme(ioDeme,ioContext);
  Beagle_StackTraceEndM("void EvaluationOp::operate(Deme&,Context&)");
}


/*!
 *  \brief Prepare statistics to be processed.
 *  \param ioDeme Deme for which statistics will be processed.
 *  \param ioContext Evolutionary context.
 */
void EvaluationOp::prepareStats(Deme& ioDeme, Context& ioContext) 
{
  Beagle_StackTraceBeginM();
  ioContext.setProcessedDeme(0);
  if((ioContext.getGeneration()!=0) && (ioDeme.getStats()->existItem("total-processed"))) {
    ioContext.setTotalProcessedDeme((unsigned int)ioDeme.getStats()->getItem("total-processed"));
  }
  else ioContext.setTotalProcessedDeme(0);
  ioDeme.getStats()->setInvalid();

  if(ioContext.getDemeIndex()==0) {
    Stats& lVivaStats = *ioContext.getVivarium().getStats();
    ioContext.setProcessedVivarium(0);
    if((ioContext.getGeneration()!=0) && (lVivaStats.existItem("total-processed"))) {
      ioContext.setTotalProcessedVivarium((unsigned int)lVivaStats.getItem("total-processed"));
    }
    else ioContext.setTotalProcessedVivarium(0);
    lVivaStats.setInvalid();
  }
  Beagle_StackTraceEndM("void EvaluationOp::prepareStats(Deme&,Context&)");
}


/*!
 *  \brief Test the fitness of a given individual.
 *  \param inIndividual Handle to the individual to test.
 *  \param ioSystem Handle to the system to use to test the individual.
 *  \par Note:
 *    This method is provided as a mean to test some individuals after an evolution.
 */
Fitness::Handle EvaluationOp::test(Individual::Handle inIndividual, System::Handle ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle_LogInfoM(
    ioSystem->getLogger(),
    "evaluation", "Beagle::EvaluationOp",
    string("Testing the following individual: ")+inIndividual->serialize()
  );

  Context::Alloc::Handle lContextAlloc =
    castHandleT<Context::Alloc>(ioSystem->getContextAllocatorHandle());
  Context::Handle lContext = castHandleT<Context>(lContextAlloc->allocate());
  lContext->setSystemHandle(ioSystem);
  lContext->setIndividualHandle(inIndividual);
  Fitness::Handle lFitness = evaluate(*inIndividual, *lContext);

  Beagle_LogInfoM(
    ioSystem->getLogger(),
    "evaluation", "Beagle::EvaluationOp",
    string("New fitness of the individual: ")+lFitness->serialize()
  );

  return lFitness;
  Beagle_StackTraceEndM("Fitness::Handle EvaluationOp::test(Individual::Handle, System::Handle)");
}


/*!
 *  \brief Update the hall of fame with current deme.
 *  \param ioDeme Deme to use to update the hall-of-fame.
 *  \param ioContext Evolutionary context.
 */
void EvaluationOp::updateHallOfFameWithDeme(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(mDemeHOFSize->getWrappedValue() > 0) {
    Beagle_LogDetailedM(
      ioContext.getSystem().getLogger(),
      "evaluation", "Beagle::EvaluationOp",
      "Updating the deme's hall-of-fame"
    );
    ioDeme.getHallOfFame().updateWithDeme(mDemeHOFSize->getWrappedValue(), ioDeme, ioContext);
    ioDeme.getHallOfFame().log(Logger::eVerbose, ioContext);
  }
  if(mVivaHOFSize->getWrappedValue() > 0) {
    Beagle_LogDetailedM(
      ioContext.getSystem().getLogger(),
      "evaluation", "Beagle::EvaluationOp",
      "Updating the vivarium's hall-of-fame"
    );
    ioContext.getVivarium().getHallOfFame().updateWithDeme(mVivaHOFSize->getWrappedValue(),
                                                           ioDeme, ioContext);
    ioContext.getVivarium().getHallOfFame().log(Logger::eVerbose, ioContext);
  }
  Beagle_StackTraceEndM("void EvaluationOp::updateHallOfFameWithDeme(Deme&,Context&)");
}


/*!
 *  \brief Update the hall of fame with the current individual.
 *  \param ioIndividual Individual to update the hall-of-fame.
 *  \param ioContext Evolutionary context.
 */
void EvaluationOp::updateHallOfFameWithIndividual(Individual& ioIndividual, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(mDemeHOFSize->getWrappedValue() > 0) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "evaluation", "Beagle::EvaluationOp",
      "Updating the deme hall-of-fame"
    );
    ioContext.getDeme().getHallOfFame().updateWithIndividual(mDemeHOFSize->getWrappedValue(),
                                                             ioIndividual, ioContext);
  }
  if(mVivaHOFSize->getWrappedValue() > 0) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "evaluation", "Beagle::EvaluationOp",
      "Updating the vivarium hall-of-fame"
    );
    ioContext.getVivarium().getHallOfFame().updateWithIndividual(mVivaHOFSize->getWrappedValue(),
                                                                 ioIndividual, ioContext);
  }
  Beagle_StackTraceEndM("void EvaluationOp::updateHallOfFameWithIndividual(Individual&,Context&)");
}


/*!
 *  \brief Update the stats on processed individuals
 *  \param inNumProcessed The number of individuals processed since the last update.
 *  \param ioContext Evolutionary context.
 */
void EvaluationOp::updateStats(unsigned int inNumProcessed, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  ioContext.setProcessedDeme(ioContext.getProcessedDeme()+1);
  ioContext.setTotalProcessedDeme(ioContext.getTotalProcessedDeme()+1);
  ioContext.setProcessedVivarium(ioContext.getProcessedVivarium()+1);
  ioContext.setTotalProcessedVivarium(ioContext.getTotalProcessedVivarium()+1);    
  Beagle_StackTraceEndM("void EvaluationOp::updateStats(unsigned int,Context&)");
}


