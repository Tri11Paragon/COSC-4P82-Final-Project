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
 *  \file   KnapsackEvalOp.cpp
 *  \brief  Implementation of the class KnapsackEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/GA.hpp"
#include "KnapsackEvalOp.hpp"
#include <cmath>

#define BEAGLE_KNAPSACK_NBOBJECTS 24


using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator for the Knapsack problem.
 */
KnapsackEvalOp::KnapsackEvalOp() :
  EvaluationOp("KnapsackEvalOp"),
  mSumWeight(0.0)
{ }


/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 */
Fitness::Handle KnapsackEvalOp::evaluate(Individual& inIndividual, Context& ioContext)
{
  Beagle_AssertM(inIndividual.size() == 1);
  GA::BitString::Handle lBitString = castHandleT<GA::BitString>(inIndividual[0]);
  float lValue  = 0.0;
  float lWeight = 0.0;
  for(unsigned int i=0; i<lBitString->size(); ++i) {
    if((*lBitString)[i]) {
      lValue  += (*mObjectValues)[i];
      lWeight += (*mObjectWeights)[i];
    }
  }
  FitnessMultiObj::Handle lFitness = new FitnessMultiObj(2);
  (*lFitness)[0] = lValue;
  (*lFitness)[1] = (mSumWeight - lWeight);
  return lFitness;
}


/*!
 *  \brief Initialize knapsack evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void KnapsackEvalOp::initialize(System& ioSystem)
{
  Beagle::EvaluationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("ks.object.values")) {
    mObjectValues = castHandleT<Vector>(ioSystem.getRegister().getEntry("ks.object.values"));
  } else {
    mObjectValues = new Vector(0);
    Beagle::string lLongDescrip("Values of the available objects that can be put into the knapsack. ");
    lLongDescrip += "If the object values is not specified, it will be randomly generated ";
    lLongDescrip += "at the initialization time.";
    Register::Description lDescription(
      "Knapsack object values",
      "Vector",
      "",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ks.object.values", mObjectValues, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ks.object.weights")) {
    mObjectWeights = castHandleT<Vector>(ioSystem.getRegister().getEntry("ks.object.weights"));
  } else {
    mObjectWeights = new Vector(0);
    Beagle::string lLongDescrip("Weights of the available objects that can be put into the knapsack. ");
    lLongDescrip += "If the object weights is not specified, it will be randomly generated ";
    lLongDescrip += "at the initialization time.";
    Register::Description lDescription(
      "Knapsack object weights",
      "Vector",
      "",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ks.object.weights", mObjectWeights, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.init.vectorsize")) {
    mIntVectorSize = castHandleT<UInt>(ioSystem.getRegister()["ga.init.vectorsize"]);
  } else {
    mIntVectorSize = new UInt(0);
    Register::Description lDescription(
      "Initial integer vectors sizes",
      "UInt",
      "0",
      "Integer vector size of initialized individuals."
    );
    ioSystem.getRegister().addEntry("ga.init.vectorsize", mIntVectorSize, lDescription);
  }
}


/*!
 *  \brief Post-initialize knapsack evaluation operator by generating object's value/weight.
 *  \param ioSystem Evolutionary system.
 */
void KnapsackEvalOp::postInit(System& ioSystem)
{
  Beagle::EvaluationOp::postInit(ioSystem);

  float lSumValue  = 0.0;
  if((mObjectValues->size()==0) && (mObjectWeights->size()==0)) {
    mObjectValues->resize(BEAGLE_KNAPSACK_NBOBJECTS);
    mObjectWeights->resize(BEAGLE_KNAPSACK_NBOBJECTS);
    mSumWeight = 0.0;
    for(unsigned int i=0; i<BEAGLE_KNAPSACK_NBOBJECTS; ++i) {
      lSumValue  += (*mObjectValues)[i]  = ioSystem.getRandomizer().rollUniform(1.0, 10.0);
      mSumWeight += (*mObjectWeights)[i] = ioSystem.getRandomizer().rollUniform(1.0, 10.0);
    }
  }
  else if(mObjectValues->size() != mObjectWeights->size()) {
    std::ostringstream lOSS;
    lOSS << "Objects value and weight of evaluation operator (registered as ";
    lOSS << "'ks.objects.value' and 'ks.objects.weight' parameters) must be of the same size!";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  else {
    mSumWeight = 0.0;
    for(unsigned int i=0; i<mObjectValues->size(); ++i) {
      lSumValue  += (*mObjectValues)[i];
      mSumWeight += (*mObjectWeights)[i];
    }
  }

  mIntVectorSize->getWrappedValue() = mObjectValues->size();

  Beagle_LogInfoM(
    ioSystem.getLogger(),
    "evaluation", "KnapsackEvalOp",
    Beagle::string("Sum of possible knapsack value is ")+dbl2str(lSumValue)+
    Beagle::string(", while sum of possible knapsack weight is ")+dbl2str(mSumWeight)
  );
  Beagle_LogInfoM(
    ioSystem.getLogger(),
    "evaluation", "KnapsackEvalOp",
    Beagle::string("Objects value that can be put knapsack: ")+mObjectValues->serialize()
  );
  Beagle_LogInfoM(
    ioSystem.getLogger(),
    "evaluation", "KnapsackEvalOp",
    Beagle::string("Objects weight that can be put knapsack: ")+mObjectWeights->serialize()
  );
}

