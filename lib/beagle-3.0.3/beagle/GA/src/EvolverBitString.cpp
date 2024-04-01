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
 *  \file   beagle/GA/src/EvolverBitString.cpp
 *  \brief  Source code of class GA::EvolverBitString.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14.2.1 $
 *  $Date: 2007/05/09 01:51:02 $
 */

#include "beagle/GA.hpp"

#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a GA Generational evolver.
 *  \param inInitSize Number of bits in the GA bit strings
 */
GA::EvolverBitString::EvolverBitString(unsigned int inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(new GA::InitBitStrOp(inInitSize));
  addOperator(new GA::CrossoverOnePointBitStrOp);
  addOperator(new GA::CrossoverTwoPointsBitStrOp);
  addOperator(new GA::CrossoverUniformBitStrOp);
  addOperator(new GA::MutationFlipBitStrOp);
  Beagle_StackTraceEndM("GA::EvolverBitString::EvolverBitString(unsigned int inInitSize)");
}


/*!
 *  \brief Construct a GA Generational evolver.
 *  \param inEvalOp Evaluation operator.
 *  \param inInitSize Number of bits in the GA bit strings.
 */
GA::EvolverBitString::EvolverBitString(EvaluationOp::Handle inEvalOp, unsigned int inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(inEvalOp);
  addOperator(new GA::InitBitStrOp(inInitSize));
  addOperator(new GA::CrossoverOnePointBitStrOp);
  addOperator(new GA::CrossoverTwoPointsBitStrOp);
  addOperator(new GA::CrossoverUniformBitStrOp);
  addOperator(new GA::MutationFlipBitStrOp);

  addBootStrapOp("IfThenElseOp");
  IfThenElseOp::Handle lITE = castHandleT<IfThenElseOp>(getBootStrapSet().back());
  lITE->setConditionTag("ms.restart.file");
  lITE->setConditionValue("");
  lITE->insertPositiveOp("GA-InitBitStrOp", getOperatorMap());
  lITE->insertPositiveOp(inEvalOp->getName(), getOperatorMap());
  lITE->insertPositiveOp("StatsCalcFitnessSimpleOp", getOperatorMap());
  lITE->insertNegativeOp("MilestoneReadOp", getOperatorMap());
  addBootStrapOp("TermMaxGenOp");
  addBootStrapOp("MilestoneWriteOp");

  addMainLoopOp("SelectTournamentOp");
  addMainLoopOp("GA-CrossoverOnePointBitStrOp");
  addMainLoopOp("GA-MutationFlipBitStrOp");
  addMainLoopOp(inEvalOp->getName());
  addMainLoopOp("MigrationRandomRingOp");
  addMainLoopOp("StatsCalcFitnessSimpleOp");
  addMainLoopOp("TermMaxGenOp");
  addMainLoopOp("MilestoneWriteOp");
  Beagle_StackTraceEndM("GA::EvolverBitString::EvolverBitString(EvaluationOp::Handle inEvalOp, unsigned int inInitSize)");
}


/*!
 *  \brief Construct a GA Generational evolver.
 *  \param inInitSize Size of the GA bit strings.
 *  \deprecated Use EvolverBitString(EvaluationOp::Handle,unsigned int) constructor instead.
 *  \throw Beagle::RunTimeException If init size vector has more than one value.
 */
GA::EvolverBitString::EvolverBitString(UIntArray inInitSize)
{
  Beagle_StackTraceBeginM();
  if(inInitSize.size()==0) addOperator(new GA::InitBitStrOp(0));
  else if(inInitSize.size()==1) addOperator(new GA::InitBitStrOp(inInitSize[0]));
  else {
    std::ostringstream lOSS;
    lOSS << "Initialization of bit string individuals with more than one bit string ";
    lOSS << "is no more valid. You should use individuals made of one bit string, or ";
    lOSS << "define your own bit string initialization operator.";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  addOperator(new GA::CrossoverOnePointBitStrOp);
  addOperator(new GA::CrossoverTwoPointsBitStrOp);
  addOperator(new GA::CrossoverUniformBitStrOp);
  addOperator(new GA::MutationFlipBitStrOp);
  Beagle_StackTraceEndM("GA::EvolverBitString::EvolverBitString(UIntArray inInitSize)");
}


/*!
 *  \brief Construct a GA Generational evolver.
 *  \param inEvalOp Evaluation operator.
 *  \param inInitSize Size of the GA bit strings.
 *  \deprecated Use EvolverBitString(EvaluationOp::Handle,unsigned int) constructor instead.
 *  \throw Beagle::RunTimeException If init size vector has more than one value.
 */
GA::EvolverBitString::EvolverBitString(EvaluationOp::Handle inEvalOp, UIntArray inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(inEvalOp);
  if(inInitSize.size()==0) addOperator(new GA::InitBitStrOp(0));
  else if(inInitSize.size()==1) addOperator(new GA::InitBitStrOp(inInitSize[0]));
  else {
    std::ostringstream lOSS;
    lOSS << "Initialization of bit string individuals with more than one bit string ";
    lOSS << "is no more valid. You should use individuals made of one bit string, or ";
    lOSS << "define your own bit string initialization operator.";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  addOperator(new GA::CrossoverOnePointBitStrOp);
  addOperator(new GA::CrossoverTwoPointsBitStrOp);
  addOperator(new GA::CrossoverUniformBitStrOp);
  addOperator(new GA::MutationFlipBitStrOp);

  addBootStrapOp("IfThenElseOp");
  IfThenElseOp::Handle lITE = castHandleT<IfThenElseOp>(getBootStrapSet().back());
  lITE->setConditionTag("ms.restart.file");
  lITE->setConditionValue("");
  lITE->insertPositiveOp("GA-InitBitStrOp", getOperatorMap());
  lITE->insertPositiveOp(inEvalOp->getName(), getOperatorMap());
  lITE->insertPositiveOp("StatsCalcFitnessSimpleOp", getOperatorMap());
  lITE->insertNegativeOp("MilestoneReadOp", getOperatorMap());
  addBootStrapOp("TermMaxGenOp");
  addBootStrapOp("MilestoneWriteOp");

  addMainLoopOp("SelectTournamentOp");
  addMainLoopOp("GA-CrossoverOnePointBitStrOp");
  addMainLoopOp("GA-MutationFlipBitStrOp");
  addMainLoopOp(inEvalOp->getName());
  addMainLoopOp("MigrationRandomRingOp");
  addMainLoopOp("StatsCalcFitnessSimpleOp");
  addMainLoopOp("TermMaxGenOp");
  addMainLoopOp("MilestoneWriteOp");
  Beagle_StackTraceEndM("GA::EvolverBitString::EvolverBitString(EvaluationOp::Handle inEvalOp, UIntArray inInitSize)");
}

