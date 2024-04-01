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
 *  \file   TrainSetEvalOp.cpp
 *  \brief  Implementation of the class TrainSetEvalOp.
 *  \author Jiachuan Wang <jiacwang@ecs.umass.edu>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.9.2.1 $
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
 *  \brief Construct the training set individual evaluation operator for the
 *    co-evolutionary symbolic regression problem.
 */
TrainSetEvalOp::TrainSetEvalOp() :
  CoSymEvalOp("TrainSetEvalOp")
{ }


/*!
 *  \brief Make evaluation sets to evaluate fitness on TrainSet.
 *  \param ioIndivBag Bag of individuals to use for evaluation.
 *  \param ioContext Evaluationary context.
 */
void TrainSetEvalOp::makeSets(Individual::Bag& ioIndivBag, Context::Handle ioContext)
{
  // If last generation best individual is NULL handle, choose a random individual.
  if(mLastGenBestIndividual == NULL) {
    unsigned int lRandomIndex =
      ioContext->getSystem().getRandomizer().rollInteger(0, ioContext->getDeme().size()-1);
    Individual::Handle lIndivToCopy = ioContext->getDeme()[lRandomIndex];
    Individual::Alloc::Handle lIndivAlloc = ioContext->getDeme().getTypeAlloc();
    mLastGenBestIndividual =
      castHandleT<Individual>(lIndivAlloc->cloneData(*lIndivToCopy));
  }

  // Eval set for best training set
  Individual::Bag lBestTrainSetIndiv;
  lBestTrainSetIndiv.push_back(mLastGenBestIndividual);
  EvalSet lBestTrainEvalSet(lBestTrainSetIndiv, ioContext, 3);
  addSet(lBestTrainEvalSet, false);

  // Eval set for training sets
  EvalSet lTrainEvalSet(ioIndivBag, ioContext, 2);
  addSet(lTrainEvalSet, true);

  // Get a copy of the best individual for next generation
  unsigned int lBestIndivIndex = 0;
  float lBestIndivIndexFits =
    castHandleT<FitnessSimple>(ioContext->getDeme()[0]->getFitness())->getValue();
  for(unsigned int i=1; i<ioContext->getDeme().size(); ++i) {
    float lFitness =
      castHandleT<FitnessSimple>(ioContext->getDeme()[i]->getFitness())->getValue();
    if(lFitness > lBestIndivIndexFits) {
      lBestIndivIndexFits = lFitness;
      lBestIndivIndex = i;
    }
  }
  Individual::Handle lIndivToCopy = ioContext->getDeme()[lBestIndivIndex];
  Individual::Alloc::Handle lIndivAlloc = ioContext->getDeme().getTypeAlloc();
  mLastGenBestIndividual =
    castHandleT<Individual>(lIndivAlloc->cloneData(*lIndivToCopy));
}

