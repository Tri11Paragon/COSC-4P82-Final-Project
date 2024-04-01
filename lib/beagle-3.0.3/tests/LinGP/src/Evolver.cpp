/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/src/Evolver.cpp
 *  \brief  Source code of class LinGP::Evolver.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */

#include <string>

#include "beagle/GP.hpp"
#include "beagle/LinGP/Evolver.hpp"
#include "beagle/LinGP/EvaluationOp.hpp"
#include "beagle/LinGP/CrossoverOnePointOp.hpp"
#include "beagle/LinGP/CrossoverTwoPointsOp.hpp"
#include "beagle/LinGP/CrossoverUniformOp.hpp"
#include "beagle/LinGP/MutationOp.hpp"
#include "beagle/LinGP/InitializationOp.hpp"
#include "beagle/IfThenElseOp.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a linear GP evolver.
 */
LinGP::Evolver::Evolver()
{
  addOperator(new LinGP::InitializationOp);
  addOperator(new LinGP::CrossoverOnePointOp);
  addOperator(new LinGP::CrossoverTwoPointsOp);
  addOperator(new LinGP::CrossoverUniformOp);
  addOperator(new LinGP::MutationOp);
}
  

/*!
 *  \brief Construct a linear GP generational evolver.
 *  \param inEvalOp Linear GP evaluation operator.
 */
LinGP::Evolver::Evolver(Beagle::EvaluationOp::Handle inEvalOp)
{
  addOperator(inEvalOp);
  addOperator(new LinGP::InitializationOp);
  addOperator(new LinGP::CrossoverOnePointOp);
  addOperator(new LinGP::CrossoverTwoPointsOp);
  addOperator(new LinGP::CrossoverUniformOp);
  addOperator(new LinGP::MutationOp);

  addBootStrapOp("IfThenElseOp");
  IfThenElseOp::Handle lITE = castHandleT<IfThenElseOp>(getBootStrapSet().back());
  lITE->setConditionTag("ms.restart.file");
  lITE->setConditionValue("");
  lITE->insertPositiveOp("LinGP-InitializationOp", getOperatorMap());
  lITE->insertPositiveOp(inEvalOp->getName(), getOperatorMap());
  lITE->insertPositiveOp("StatsCalcFitnessSimpleOp", getOperatorMap());
  lITE->insertNegativeOp("MilestoneReadOp", getOperatorMap());
  addBootStrapOp("TermMaxGenOp");
  addBootStrapOp("MilestoneWriteOp");

  addMainLoopOp("SelectTournamentOp");
  addMainLoopOp("LinGP-CrossoverOnePointOp");
  addMainLoopOp("LinGP-MutationOp");
  addMainLoopOp(inEvalOp->getName());
  addMainLoopOp("MigrationRandomRingOp");
  addMainLoopOp("StatsCalcFitnessSimpleOp");
  addMainLoopOp("TermMaxGenOp");
  addMainLoopOp("MilestoneWriteOp");
}

