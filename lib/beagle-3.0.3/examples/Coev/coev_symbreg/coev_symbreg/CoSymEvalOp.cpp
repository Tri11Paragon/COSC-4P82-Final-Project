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
 *  \file   CoSymEvalOp.cpp
 *  \brief  Implementation of the class CoSymEvalOp.
 *  \author Jiachuan Wang <jiacwang@ecs.umass.edu>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Coev.hpp"
#include "CoSymEvalOp.hpp"
#include "TrainSetEvalOp.hpp"
#include "TrainSetThread.hpp"
#include "SymGPEvalOp.hpp"
#include "SymGPThread.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator for the co-evolutionary symbolic
 *    regression problem.
 *  \param inName Name of the operator.
 */
CoSymEvalOp::CoSymEvalOp(Beagle::string inName) :
  Coev::GPEvaluationOp(4, inName)
{ }


/*!
 *  \brief Evaluate RMS value fitness of a given pair of training set / symbolic regression
 *    expression.
 *  \param inTrainSet Train set real-valued GA float vector.
 *  \param inExpression GP symbolic regression expression.
 *  \param ioContext GP evolutionary context.
 *  \return Root-mean square error of the GP expression on the train set.
 */
double CoSymEvalOp::evaluateRMS(GA::FloatVector& inTrainSet,
                                GP::Individual& inExpression,
                                GP::Context& ioContext) const
{
  inExpression[0]->setContextToNode(0, ioContext);
  GP::Individual::Handle lOldIndivHandle = ioContext.getIndividualHandle();
  ioContext.setIndividualHandle(&inExpression);
  
  double lSquareError = 0.0;
  for(unsigned int i=0; i<inTrainSet.size(); ++i) {
    Double lX = inTrainSet[i];
    if(lX > 1.0)  lX = 1.0;
    if(lX < -1.0) lX = -1.0;
    Double lY = (lX*(lX*(lX*(lX+1.0)+1.0)+1.0));
    setValue("X", lX, ioContext);

    Double lResult;
    inExpression.run(lResult, ioContext);
    // (*lSymGpIndiv[0])[0].mPrimitive->execute(lResult, lContext);

    double lError = std::fabs(lY - lResult);
    lSquareError += (lError*lError);
  }
  ioContext.setIndividualHandle(lOldIndivHandle);
  double lMSE = lSquareError / inTrainSet.size();
  return std::sqrt(lMSE); 
}


/*!
 *  \brief Evaluate fitness with the given evaluation sets.
 *  \param ioSets Sets to evaluate.
 */
void CoSymEvalOp::evaluateSets(Beagle::Coev::GPEvaluationOp::EvalSetVector& ioSets)
{
  Beagle_AssertM(ioSets.size() == 4);

  // mID: GPSet(0), lBestGPSet(1), TrainSet(2), BestTrainSet(3)
  unsigned int lGPSet=0;            // Index for GP set in ioSets
  unsigned int lTrainSet=0;         // Index for training set in ioSets
  unsigned int lBestTrainSet=0;     // Index for best training set in ioSets  
  unsigned int lBestGPSet=0;        // Index for best GP set in ioSets
  switch(ioSets[0].mID) {
    case 0: lGPSet = 0; break;
    case 1: lBestGPSet = 0; break;
    case 2: lTrainSet = 0; break;
    case 3: lBestTrainSet = 0; break;
    default: throw Beagle_RunTimeExceptionM("Undefined eval set ID!");
  }
  switch(ioSets[1].mID) {
    case 0: lGPSet = 1; break;
    case 1: lBestGPSet = 1; break;
    case 2: lTrainSet = 1; break;
    case 3: lBestTrainSet = 1; break;
    default: throw Beagle_RunTimeExceptionM("Undefined eval set ID!");
  }
  switch(ioSets[2].mID) {
    case 0: lGPSet = 2; break;
    case 1: lBestGPSet = 2; break;
    case 2: lTrainSet = 2; break;
    case 3: lBestTrainSet = 2; break;
    default: throw Beagle_RunTimeExceptionM("Undefined eval set ID!");
  }
  switch(ioSets[3].mID) {
    case 0: lGPSet = 3; break;
    case 1: lBestGPSet = 3; break;
    case 2: lTrainSet = 3; break;
    case 3: lBestTrainSet = 3; break;
    default: throw Beagle_RunTimeExceptionM("Undefined eval set ID!");
  }

  Beagle_AssertM(ioSets[lGPSet].mIndividuals.size() > 0);  
  Beagle_AssertM(ioSets[lTrainSet].mIndividuals.size() > 0);
  Beagle_AssertM(ioSets[lBestTrainSet].mIndividuals.size() == 1);
  Beagle_AssertM(ioSets[lBestGPSet].mIndividuals.size() == 1);

  // Match GP equations set with best training set
  Individual& lBestTrainIndiv = *ioSets[lBestTrainSet].mIndividuals[0];
  Beagle_AssertM(lBestTrainIndiv.size() == 1);
  GA::FloatVector::Handle lFVBestTrain = castHandleT<GA::FloatVector>(lBestTrainIndiv[0]);
  Beagle_AssertM(lFVBestTrain->size() == 20);
  GP::Context& lGPContext = castObjectT<GP::Context&>(*ioSets[lGPSet].mContext);

  unsigned int lGPSetSize = ioSets[lGPSet].mIndividuals.size();
  for(unsigned int i=0; i<lGPSetSize; ++i) {
    GP::Individual& lGPIndividual =
      castObjectT<GP::Individual&>(*ioSets[lGPSet].mIndividuals[i]);
    double lRMS = evaluateRMS(*lFVBestTrain, lGPIndividual, lGPContext);
    assignFitness(new FitnessSimpleMin(lRMS), lGPIndividual, lGPContext);
  }

  // Match training set with best symbolic regression function
  GP::Individual& lBestGPIndiv =
    castObjectT<GP::Individual&>(*ioSets[lBestGPSet].mIndividuals[0]);
  GP::Context& lGPContext2 = castObjectT<GP::Context&>(*ioSets[lBestGPSet].mContext);

  unsigned int lTrainSetSize = ioSets[lTrainSet].mIndividuals.size();
  for(unsigned int i=0; i<lTrainSetSize; ++i) {
    Individual& lTrainIndiv = *ioSets[lTrainSet].mIndividuals[i];
    Beagle_AssertM(lTrainIndiv.size() == 1);
    GA::FloatVector::Handle lFVTrain = castHandleT<GA::FloatVector>(lTrainIndiv[0]);
    Beagle_AssertM(lFVTrain->size() == 20);

    double lRMS = evaluateRMS(*lFVTrain, lBestGPIndiv, lGPContext2);
    assignFitness(new FitnessSimple(lRMS), lTrainIndiv, *ioSets[lTrainSet].mContext);
  }
}



