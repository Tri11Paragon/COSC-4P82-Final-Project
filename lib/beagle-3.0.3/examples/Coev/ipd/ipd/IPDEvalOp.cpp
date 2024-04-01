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
 *  \file   IPDEvalOp.cpp
 *  \brief  Implementation of the class IPDEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Coev.hpp"
#include "IPDEvalOp.hpp"

#include <cmath>
#include <algorithm>

using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator for the IPD problem.
 */
IPDEvalOp::IPDEvalOp() :
  Coev::EvaluationOp(2, "IPDEvalOp")
{ }


/*!
 *  \brief Evaluate fitness with the given evaluation sets.
 *  \param ioSets Sets to evaluate.
 */
void IPDEvalOp::evaluateSets(Beagle::Coev::EvaluationOp::EvalSetVector& ioSets)
{
  Beagle_AssertM(ioSets.size() == 2);
  Beagle_AssertM(ioSets[0].mIndividuals.size() > 0);
  Beagle_AssertM(ioSets[1].mIndividuals.size() > 0);
  // Shuffle first population.
  std::random_shuffle(ioSets[0].mIndividuals.begin(), ioSets[0].mIndividuals.end(),
                      ioSets[0].mContext->getSystem().getRandomizer());
  // Shuffle second population if its size is > than first population.
  if(ioSets[1].mIndividuals.size() > ioSets[0].mIndividuals.size()) {
    std::random_shuffle(ioSets[1].mIndividuals.begin(), ioSets[1].mIndividuals.end(),
                        ioSets[1].mContext->getSystem().getRandomizer());
  }
  // Mate individuals one by one, until all individuals are evaluated.
  unsigned int lMaxSize = maxOf(ioSets[0].mIndividuals.size(), ioSets[1].mIndividuals.size());
  for(unsigned int i=0; i<lMaxSize; ++i) {
    unsigned int lIndex1 = i % ioSets[0].mIndividuals.size();
    Individual& lIndividual1 = *ioSets[0].mIndividuals[lIndex1];
    unsigned int lScore1 = 0;
    unsigned int lIndex2 = i % ioSets[1].mIndividuals.size();
    Individual& lIndividual2 = *ioSets[1].mIndividuals[lIndex2]; 
    unsigned int lScore2 = 0;
    playIPD(lIndividual1, lIndividual2, lScore1, lScore2);
    if(i < ioSets[0].mIndividuals.size()) {
      assignFitness(new FitnessSimple(float(lScore1)), lIndividual1, *ioSets[0].mContext);
    }
    if(i < ioSets[1].mIndividuals.size()) {
      assignFitness(new FitnessSimple(float(lScore2)), lIndividual2, *ioSets[1].mContext);
    }
  }
}


/*!
 *  \brief Make evaluation sets to evaluate fitness on IPD.
 *  \param ioIndivBag Bag of individuals to use for evaluation.
 *  \param ioContext Evaluationary context.
 */
void IPDEvalOp::makeSets(Individual::Bag& ioIndivBag, Context::Handle ioContext)
{
  EvalSet lEvalSet(ioIndivBag, ioContext);
  addSet(lEvalSet);
}


/*!
 *  \brief Play iterated prisoner's dilemma with two individuals.
 *  \param inInd1 First individual to play IPD.
 *  \param inInd2 Second individual to play IPD.
 *  \param outScoreInd1 Score of first individual.
 *  \param outScoreInd2 Score of second individual.
 */
void IPDEvalOp::playIPD(const Beagle::Individual& inInd1, const Beagle::Individual& inInd2,
                        unsigned int& outScoreInd1, unsigned int& outScoreInd2) const
{
  Beagle_AssertM(inInd1.size() == 1);
  Beagle_AssertM(inInd2.size() == 1);
  const GA::BitString& lBS1 = castObjectT<const GA::BitString&>(*inInd1[0]);
  const GA::BitString& lBS2 = castObjectT<const GA::BitString&>(*inInd2[0]);
  bool lMove1 = lBS1[0];
  bool lMove2 = lBS2[0];
  if(lMove1) {
    if(lMove2) {        // All cooperate
      outScoreInd1 = 5;
      outScoreInd2 = 5;
    }
    else {              // 1: cooperate, 2: defect
      outScoreInd1 = 0;
      outScoreInd2 = 3;
    }
  }
  else {
    if(lMove2) {        // 1: defect, 2: cooperate
      outScoreInd1 = 3;
      outScoreInd2 = 0;
    }
    else {              // All defect
      outScoreInd1 = 1;
      outScoreInd2 = 1;
    }
  }
  for(unsigned int i=0; i<7; ++i) {
    bool lLastMove1 = lMove1;
    if(lMove2) lMove1 = lBS1[1];
    else lMove1 = lBS1[2];
    if(lLastMove1) lMove2 = lBS2[1];
    else lMove2 = lBS2[2];
    if(lMove1) {
      if(lMove2) {        // All cooperate
        outScoreInd1 += 5;
        outScoreInd2 += 5;
      }
      else {              // 1: cooperate, 2: defect
        outScoreInd2 += 3;
      }
    }
    else {
      if(lMove2) {        // 1: defect, 2: cooperate
        outScoreInd1 += 3;
      }
      else {              // All defect
        outScoreInd1 += 1;
        outScoreInd2 += 1;
      }
    }
  }
}


