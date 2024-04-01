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
 *  \file   beagle/GA/src/EvolverIntegerVector.cpp
 *  \brief  Source code of class GA::EvolverIntegerVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.2.2.1 $
 *  $Date: 2007/05/09 01:51:02 $
 */

#include "beagle/GA.hpp"

#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a integer-valued GA evolver.
 *  \param inInitSize Size of the GA integer vectors.
 */
GA::EvolverIntegerVector::EvolverIntegerVector(unsigned int inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(new GA::InitIndicesIntVecOp(inInitSize));
  addOperator(new GA::InitIntVecOp(inInitSize));
  addOperator(new GA::CrossoverIndicesIntVecOp);
  addOperator(new GA::CrossoverOnePointIntVecOp);
  addOperator(new GA::CrossoverTwoPointsIntVecOp);
  addOperator(new GA::CrossoverUniformIntVecOp);
  addOperator(new GA::MutationShuffleIntVecOp);
  addOperator(new GA::MutationUniformIntVecOp);
  Beagle_StackTraceEndM("GA::EvolverIntegerVector::EvolverIntegerVector(unsigned int inInitSize)");
}


/*!
 *  \brief Construct a integer-valued GA evolver.
 *  \param inEvalOp Evaluation operator.
 *  \param inInitSize Size of the GA integer vectors.
 */
GA::EvolverIntegerVector::EvolverIntegerVector(EvaluationOp::Handle inEvalOp, unsigned int inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(inEvalOp);
  addOperator(new GA::InitIndicesIntVecOp(inInitSize));
  addOperator(new GA::InitIntVecOp(inInitSize));
  addOperator(new GA::CrossoverIndicesIntVecOp);
  addOperator(new GA::CrossoverOnePointIntVecOp);
  addOperator(new GA::CrossoverTwoPointsIntVecOp);
  addOperator(new GA::CrossoverUniformIntVecOp);
  addOperator(new GA::MutationShuffleIntVecOp);
  addOperator(new GA::MutationUniformIntVecOp);

  addBootStrapOp("IfThenElseOp");
  IfThenElseOp::Handle lITE = castHandleT<IfThenElseOp>(getBootStrapSet().back());
  lITE->setConditionTag("ms.restart.file");
  lITE->setConditionValue("");
  lITE->insertPositiveOp("GA-InitIntVecOp", getOperatorMap());
  lITE->insertPositiveOp(inEvalOp->getName(), getOperatorMap());
  lITE->insertPositiveOp("StatsCalcFitnessSimpleOp", getOperatorMap());
  lITE->insertNegativeOp("MilestoneReadOp", getOperatorMap());
  addBootStrapOp("TermMaxGenOp");
  addBootStrapOp("MilestoneWriteOp");

  addMainLoopOp("SelectTournamentOp");
  addMainLoopOp("GA-CrossoverUniformIntVecOp");
  addMainLoopOp("GA-MutationUniformIntVecOp");
  addMainLoopOp(inEvalOp->getName());
  addMainLoopOp("MigrationRandomRingOp");
  addMainLoopOp("StatsCalcFitnessSimpleOp");
  addMainLoopOp("TermMaxGenOp");
  addMainLoopOp("MilestoneWriteOp");
  Beagle_StackTraceEndM("GA::EvolverIntegerVector::EvolverIntegerVector(EvaluationOp::Handle inEvalOp, unsigned int inInitSize)");
}
