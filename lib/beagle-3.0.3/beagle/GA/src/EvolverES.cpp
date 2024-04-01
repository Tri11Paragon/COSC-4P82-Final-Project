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
 *  \file   beagle/GA/src/EvolverES.cpp
 *  \brief  Source code of class GA::EvolverES.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.1 $
 *  $Date: 2007/05/09 01:51:02 $
 */

#include "beagle/GA.hpp"

#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a evolution strategy evolver.
 *  \param inInitSize Number of (value,strategy) pairs of the ES vectors.
 */
GA::EvolverES::EvolverES(unsigned int inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(new GA::InitESVecOp(inInitSize));
  addOperator(new GA::CrossoverOnePointESVecOp);
  addOperator(new GA::CrossoverTwoPointsESVecOp);
  addOperator(new GA::CrossoverUniformESVecOp);
  addOperator(new GA::CrossoverBlendESVecOp);
  addOperator(new GA::MutationESVecOp);
  Beagle_StackTraceEndM("GA::EvolverES::EvolverES(unsigned int inInitSize)");
}


/*!
 *  \brief Construct a evolution strategy evolver.
 *  \param inEvalOp Evaluation operator.
 *  \param inInitSize Number of (value,strategy) pairs of the ES vectors.
 */
GA::EvolverES::EvolverES(EvaluationOp::Handle inEvalOp, unsigned int inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(inEvalOp);
  addOperator(new GA::InitESVecOp(inInitSize));
  addOperator(new GA::CrossoverOnePointESVecOp);
  addOperator(new GA::CrossoverTwoPointsESVecOp);
  addOperator(new GA::CrossoverUniformESVecOp);
  addOperator(new GA::CrossoverBlendESVecOp);
  addOperator(new GA::MutationESVecOp);

  addBootStrapOp("IfThenElseOp");
  IfThenElseOp::Handle lITE = castHandleT<IfThenElseOp>(getBootStrapSet().back());
  lITE->setConditionTag("ms.restart.file");
  lITE->setConditionValue("");
  lITE->insertPositiveOp("GA-InitESVecOp", getOperatorMap());
  lITE->insertPositiveOp(inEvalOp->getName(), getOperatorMap());
  lITE->insertPositiveOp("StatsCalcFitnessSimpleOp", getOperatorMap());
  lITE->insertNegativeOp("MilestoneReadOp", getOperatorMap());
  addBootStrapOp("TermMaxGenOp");
  addBootStrapOp("MilestoneWriteOp");

  addMainLoopOp("MuCommaLambdaOp");
  MuCommaLambdaOp::Handle lMcLOp = castHandleT<MuCommaLambdaOp>(getMainLoopSet().back());
  BreederNode::Handle lRootNode = new BreederNode;
  lMcLOp->setRootNode(lRootNode);
  lRootNode->setBreederOp(castHandleT<BreederOp>(inEvalOp->giveReference()));
  BreederNode::Handle lMutNode = new BreederNode;
  lRootNode->setFirstChild(lMutNode);
  lMutNode->setBreederOp(castHandleT<BreederOp>(getOperator("GA-MutationESVecOp")->giveReference()));
  BreederNode::Handle lSelectNode = new BreederNode;
  lMutNode->setFirstChild(lSelectNode);
  lSelectNode->setBreederOp(castHandleT<BreederOp>(getOperator("SelectRandomOp")->giveReference()));
  addMainLoopOp("MigrationRandomRingOp");
  addMainLoopOp("StatsCalcFitnessSimpleOp");
  addMainLoopOp("TermMaxGenOp");
  addMainLoopOp("MilestoneWriteOp");
  Beagle_StackTraceEndM("GA::EvolverES::EvolverES(EvaluationOp::Handle inEvalOp, unsigned int inInitSize)");
}


/*!
 *  \brief Construct a evolution strategy evolver.
 *  \param inInitSize Size of the ES vectors.
 *  \deprecated Use EvolverES(unsigned int) constructor instead.
 *  \throw Beagle::RunTimeException If init size vector has more than one value.
 */
GA::EvolverES::EvolverES(UIntArray inInitSize)
{
  Beagle_StackTraceBeginM();
  if(inInitSize.size()==0) addOperator(new GA::InitESVecOp(0));
  else if(inInitSize.size()==1) addOperator(new GA::InitESVecOp(inInitSize[0]));
  else {
    std::ostringstream lOSS;
    lOSS << "Initialization of ES vector individuals with more than one vector ";
    lOSS << "is no more valid. You should use individuals made of one vector, or ";
    lOSS << "define your own ES vector initialization operator.";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  addOperator(new GA::CrossoverOnePointESVecOp);
  addOperator(new GA::CrossoverTwoPointsESVecOp);
  addOperator(new GA::CrossoverUniformESVecOp);
  addOperator(new GA::CrossoverBlendESVecOp);
  addOperator(new GA::MutationESVecOp);
  Beagle_StackTraceEndM("GA::EvolverES::EvolverES(UIntArray inInitSize)");
}


/*!
 *  \brief Construct a evolution strategy evolver.
 *  \param inEvalOp Evaluation operator.
 *  \param inInitSize Size of the ES vectors.
 *  \deprecated Use EvolverES(EvaluationOp::Handle,unsigned int) constructor instead.
 *  \throw Beagle::RunTimeException If init size vector has more than one value.
 */
GA::EvolverES::EvolverES(EvaluationOp::Handle inEvalOp, UIntArray inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(inEvalOp);
  if(inInitSize.size()==0) addOperator(new GA::InitESVecOp(0));
  else if(inInitSize.size()==1) addOperator(new GA::InitESVecOp(inInitSize[0]));
  else {
    std::ostringstream lOSS;
    lOSS << "Initialization of ES vector individuals with more than one vector ";
    lOSS << "is no more valid. You should use individuals made of one vector, or ";
    lOSS << "define your own ES vector initialization operator.";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  addOperator(new GA::CrossoverOnePointESVecOp);
  addOperator(new GA::CrossoverTwoPointsESVecOp);
  addOperator(new GA::CrossoverUniformESVecOp);
  addOperator(new GA::CrossoverBlendESVecOp);
  addOperator(new GA::MutationESVecOp);

  addBootStrapOp("IfThenElseOp");
  IfThenElseOp::Handle lITE = castHandleT<IfThenElseOp>(getBootStrapSet().back());
  lITE->setConditionTag("ms.restart.file");
  lITE->setConditionValue("");
  lITE->insertPositiveOp("GA-InitESVecOp", getOperatorMap());
  lITE->insertPositiveOp(inEvalOp->getName(), getOperatorMap());
  lITE->insertPositiveOp("StatsCalcFitnessSimpleOp", getOperatorMap());
  lITE->insertNegativeOp("MilestoneReadOp", getOperatorMap());
  addBootStrapOp("TermMaxGenOp");
  addBootStrapOp("MilestoneWriteOp");

  addMainLoopOp("MuCommaLambdaOp");
  MuCommaLambdaOp::Handle lMcLOp = castHandleT<MuCommaLambdaOp>(getMainLoopSet().back());
  BreederNode::Handle lRootNode = new BreederNode;
  lMcLOp->setRootNode(lRootNode);
  lRootNode->setBreederOp(castHandleT<BreederOp>(inEvalOp->giveReference()));
  BreederNode::Handle lMutNode = new BreederNode;
  lRootNode->setFirstChild(lMutNode);
  lMutNode->setBreederOp(castHandleT<BreederOp>(getOperator("GA-MutationESVecOp")->giveReference()));
  BreederNode::Handle lSelectNode = new BreederNode;
  lMutNode->setFirstChild(lSelectNode);
  lSelectNode->setBreederOp(castHandleT<BreederOp>(getOperator("SelectRandomOp")->giveReference()));
  addMainLoopOp("MigrationRandomRingOp");
  addMainLoopOp("StatsCalcFitnessSimpleOp");
  addMainLoopOp("TermMaxGenOp");
  addMainLoopOp("MilestoneWriteOp");
  Beagle_StackTraceEndM("GA::EvolverES::EvolverES(EvaluationOp::Handle inEvalOp, UIntArray inInitSize)");
}
