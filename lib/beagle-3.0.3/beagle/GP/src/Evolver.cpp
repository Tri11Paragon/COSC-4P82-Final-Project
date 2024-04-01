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
 *  \file   beagle/GP/src/Evolver.cpp
 *  \brief  Source code of class GP::Evolver.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15.2.1 $
 *  $Date: 2007/05/09 01:51:06 $
 */

#include "beagle/GP.hpp"

#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a GP Generational evolver.
 */
GP::Evolver::Evolver()
{
  Beagle_StackTraceBeginM();
  addBasicOperators();
  Beagle_StackTraceEndM("GP::Evolver::Evolver()");
}


/*!
 *  \brief Construct a GP Generational evolver.
 *  \param inEvalOp GP evaluation operator.
 */
GP::Evolver::Evolver(Beagle::EvaluationOp::Handle inEvalOp)
{
  Beagle_StackTraceBeginM();
  addOperator(inEvalOp);
  addBasicOperators();

  addBootStrapOp("IfThenElseOp");
  IfThenElseOp::Handle lITE = castHandleT<IfThenElseOp>(getBootStrapSet().back());
  lITE->setConditionTag("ms.restart.file");
  lITE->setConditionValue("");
  lITE->insertPositiveOp("GP-InitHalfOp", getOperatorMap());
  lITE->insertPositiveOp(inEvalOp->getName(), getOperatorMap());
  lITE->insertPositiveOp("GP-StatsCalcFitnessSimpleOp", getOperatorMap());
  lITE->insertNegativeOp("MilestoneReadOp", getOperatorMap());
  addBootStrapOp("TermMaxGenOp");
  addBootStrapOp("MilestoneWriteOp");

  addMainLoopOp("SelectTournamentOp");
  addMainLoopOp("GP-CrossoverOp");
  addMainLoopOp("GP-MutationStandardOp");
  addMainLoopOp("GP-MutationShrinkOp");
  addMainLoopOp("GP-MutationSwapOp");
  addMainLoopOp("GP-MutationSwapSubtreeOp");
  addMainLoopOp(inEvalOp->getName());
  addMainLoopOp("MigrationRandomRingOp");
  addMainLoopOp("GP-StatsCalcFitnessSimpleOp");
  addMainLoopOp("TermMaxGenOp");
  addMainLoopOp("MilestoneWriteOp");
  Beagle_StackTraceEndM("GP::Evolver::Evolver(Beagle::EvaluationOp::Handle)");
}

/*!
 *  \brief Add the basic operators into the operator map.
 */
void GP::Evolver::addBasicOperators() 
{
  Beagle_StackTraceBeginM();
  Beagle::Evolver::addBasicOperators();
  addOperator(new GP::InitGrowOp);
  addOperator(new GP::InitFullOp);
  addOperator(new GP::InitHalfOp);
  addOperator(new GP::CrossoverOp);
  addOperator(new GP::MutationStandardOp);
  addOperator(new GP::MutationShrinkOp);
  addOperator(new GP::MutationSwapOp);
  addOperator(new GP::MutationSwapSubtreeOp);
  addOperator(new GP::InitGrowConstrainedOp);
  addOperator(new GP::InitFullConstrainedOp);
  addOperator(new GP::InitHalfConstrainedOp);
  addOperator(new GP::CrossoverConstrainedOp);
  addOperator(new GP::MutationStandardConstrainedOp);
  addOperator(new GP::MutationShrinkConstrainedOp);
  addOperator(new GP::MutationSwapConstrainedOp);
  addOperator(new GP::MutationSwapSubtreeConstrainedOp);
  addOperator(new GP::MutationEphemeralDoubleOp);
  addOperator(new GP::StatsCalcFitnessKozaOp);
  addOperator(new GP::StatsCalcFitnessSimpleOp);
  addOperator(new GP::StatsCalcFitnessSimpleOp("GP-StatsCalcFitnessSimpleMinOp"));
  addOperator(new GP::PrimitiveUsageStatsOp);
  addOperator(new GP::TermMaxHitsOp);
  addOperator(new GP::ModuleCompressOp);
  addOperator(new GP::ModuleExpandOp);
  Beagle_StackTraceEndM("GP::Evolver::addBasicOperators()");
}
