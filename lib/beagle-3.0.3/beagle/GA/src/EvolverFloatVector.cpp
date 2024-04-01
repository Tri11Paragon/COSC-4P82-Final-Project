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
 *  \file   beagle/GA/src/EvolverFloatVector.cpp
 *  \brief  Source code of class GA::EvolverFloatVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18.2.3 $
 *  $Date: 2007/09/10 18:24:10 $
 */

#include "beagle/GA.hpp"

#include <string>

using namespace Beagle;


/*!
 *  \brief Construct a real-valued GA evolver.
 *  \param inInitSize Size of the GA float vectors.
 */
GA::EvolverFloatVector::EvolverFloatVector(unsigned int inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(new GA::InitFltVecOp(inInitSize));
  addOperator(new GA::InitCMAFltVecOp(inInitSize));
  addOperator(new GA::CrossoverBlendFltVecOp);
  addOperator(new GA::CrossoverSBXFltVecOp);
  addOperator(new GA::CrossoverOnePointFltVecOp);
  addOperator(new GA::CrossoverTwoPointsFltVecOp);
  addOperator(new GA::CrossoverUniformFltVecOp);
  addOperator(new GA::MutationGaussianFltVecOp);
  addOperator(new GA::MutationCMAFltVecOp);
  addOperator(new GA::MuWCommaLambdaCMAFltVecOp);
  addOperator(new GA::MuWCommaLambdaCMAFltVecOp("ga.cmaes.mulambdaratio",
                                                "GA-MuWCommaLambdaCMAFltVecOp-2"));
  addOperator(new GA::TermCMAOp);
  Beagle_StackTraceEndM("GA::EvolverFloatVector::EvolverFloatVector(unsigned int inInitSize)");
}


/*!
 *  \brief Construct a real-valued GA evolver.
 *  \param inEvalOp Evaluation operator.
 *  \param inInitSize Size of the GA float vectors.
 */
GA::EvolverFloatVector::EvolverFloatVector(EvaluationOp::Handle inEvalOp, unsigned int inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(inEvalOp);
  addOperator(new GA::InitFltVecOp(inInitSize));
  addOperator(new GA::InitCMAFltVecOp(inInitSize));
  addOperator(new GA::CrossoverBlendFltVecOp);
  addOperator(new GA::CrossoverSBXFltVecOp);
  addOperator(new GA::CrossoverOnePointFltVecOp);
  addOperator(new GA::CrossoverTwoPointsFltVecOp);
  addOperator(new GA::CrossoverUniformFltVecOp);
  addOperator(new GA::MutationGaussianFltVecOp);
  addOperator(new GA::MutationCMAFltVecOp);
  addOperator(new GA::MuWCommaLambdaCMAFltVecOp);
  addOperator(new GA::MuWCommaLambdaCMAFltVecOp("ga.cmaes.mulambdaratio",
                                                "GA-MuWCommaLambdaCMAFltVecOp-2"));
  addOperator(new GA::TermCMAOp);

  addBootStrapOp("IfThenElseOp");
  IfThenElseOp::Handle lITE = castHandleT<IfThenElseOp>(getBootStrapSet().back());
  lITE->setConditionTag("ms.restart.file");
  lITE->setConditionValue("");
  lITE->insertPositiveOp("GA-InitFltVecOp", getOperatorMap());
  lITE->insertPositiveOp(inEvalOp->getName(), getOperatorMap());
  lITE->insertPositiveOp("StatsCalcFitnessSimpleOp", getOperatorMap());
  lITE->insertNegativeOp("MilestoneReadOp", getOperatorMap());
  addBootStrapOp("TermMaxGenOp");
  addBootStrapOp("MilestoneWriteOp");

  addMainLoopOp("SelectTournamentOp");
  addMainLoopOp("GA-CrossoverBlendFltVecOp");
  addMainLoopOp("GA-MutationGaussianFltVecOp");
  addMainLoopOp(inEvalOp->getName());
  addMainLoopOp("MigrationRandomRingOp");
  addMainLoopOp("StatsCalcFitnessSimpleOp");
  addMainLoopOp("TermMaxGenOp");
  addMainLoopOp("MilestoneWriteOp");
  Beagle_StackTraceEndM("GA::EvolverFloatVector::EvolverFloatVector(EvaluationOp::Handle inEvalOp, unsigned int inInitSize)");
}


/*!
 *  \brief Construct a real-valued GA evolver.
 *  \param inInitSize Size of the GA float vectors.
 *  \deprecated Use EvolverFloatVector(unsigned int) constructor instead.
 *  \throw Beagle::RunTimeException If init size vector has more than one value.
 */
GA::EvolverFloatVector::EvolverFloatVector(UIntArray inInitSize)
{
  Beagle_StackTraceBeginM();
  if(inInitSize.size()==0) {
    addOperator(new GA::InitFltVecOp(0));
    addOperator(new GA::InitCMAFltVecOp(0));
  }
  else if(inInitSize.size()==1) {
    addOperator(new GA::InitFltVecOp(inInitSize[0]));
    addOperator(new GA::InitCMAFltVecOp(inInitSize[0]));
  }
  else {
    std::ostringstream lOSS;
    lOSS << "Initialization of float vector individuals with more than one float vector ";
    lOSS << "is no more valid. You should use individuals made of one float vector, or ";
    lOSS << "define your own float vector initialization operator.";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  addOperator(new GA::CrossoverBlendFltVecOp);
  addOperator(new GA::CrossoverSBXFltVecOp);
  addOperator(new GA::CrossoverOnePointFltVecOp);
  addOperator(new GA::CrossoverTwoPointsFltVecOp);
  addOperator(new GA::CrossoverUniformFltVecOp);
  addOperator(new GA::MutationGaussianFltVecOp);
  addOperator(new GA::MutationCMAFltVecOp);
  addOperator(new GA::MuWCommaLambdaCMAFltVecOp);
  addOperator(new GA::MuWCommaLambdaCMAFltVecOp("ga.cmaes.mulambdaratio",
                                                "GA-MuWCommaLambdaCMAFltVecOp-2"));
  addOperator(new GA::TermCMAOp);

  Beagle_StackTraceEndM("GA::EvolverFloatVector::EvolverFloatVector(UIntArray inInitSize)");
}


/*!
 *  \brief Construct a real-valued GA evolver.
 *  \param inEvalOp Evaluation operator.
 *  \param inInitSize Size of the GA float vectors.
 *  \deprecated Use EvolverFloatVector(EvaluationOp::Handle,unsigned int) constructor instead.
 *  \throw Beagle::RunTimeException If init size vector has more than one value.
 */
GA::EvolverFloatVector::EvolverFloatVector(EvaluationOp::Handle inEvalOp, UIntArray inInitSize)
{
  Beagle_StackTraceBeginM();
  addOperator(inEvalOp);
  if(inInitSize.size()==0) {
    addOperator(new GA::InitFltVecOp(0));
    addOperator(new GA::InitCMAFltVecOp(0));
  }
  else if(inInitSize.size()==1) {
    addOperator(new GA::InitFltVecOp(inInitSize[0]));
    addOperator(new GA::InitCMAFltVecOp(inInitSize[0]));
  }
  else {
    std::ostringstream lOSS;
    lOSS << "Initialization of float vector individuals with more than one float vector ";
    lOSS << "is no more valid. You should use individuals made of one float vector, or ";
    lOSS << "define your own float vector initialization operator.";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  addOperator(new GA::CrossoverBlendFltVecOp);
  addOperator(new GA::CrossoverSBXFltVecOp);
  addOperator(new GA::CrossoverOnePointFltVecOp);
  addOperator(new GA::CrossoverTwoPointsFltVecOp);
  addOperator(new GA::CrossoverUniformFltVecOp);
  addOperator(new GA::MutationGaussianFltVecOp);
  addOperator(new GA::MutationCMAFltVecOp);
  addOperator(new GA::MuWCommaLambdaCMAFltVecOp);
  addOperator(new GA::MuWCommaLambdaCMAFltVecOp("ga.cmaes.mulambdaratio",
                                                "GA-MuWCommaLambdaCMAFltVecOp-2"));
  addOperator(new GA::TermCMAOp);

  addBootStrapOp("IfThenElseOp");
  IfThenElseOp::Handle lITE = castHandleT<IfThenElseOp>(getBootStrapSet().back());
  lITE->setConditionTag("ms.restart.file");
  lITE->setConditionValue("");
  lITE->insertPositiveOp("GA-InitFltVecOp", getOperatorMap());
  lITE->insertPositiveOp(inEvalOp->getName(), getOperatorMap());
  lITE->insertPositiveOp("StatsCalcFitnessSimpleOp", getOperatorMap());
  lITE->insertNegativeOp("MilestoneReadOp", getOperatorMap());
  addBootStrapOp("TermMaxGenOp");
  addBootStrapOp("MilestoneWriteOp");

  addMainLoopOp("SelectTournamentOp");
  addMainLoopOp("GA-CrossoverBlendFltVecOp");
  addMainLoopOp("GA-MutationGaussianFltVecOp");
  addMainLoopOp(inEvalOp->getName());
  addMainLoopOp("MigrationRandomRingOp");
  addMainLoopOp("StatsCalcFitnessSimpleOp");
  addMainLoopOp("TermMaxGenOp");
  addMainLoopOp("MilestoneWriteOp");
  Beagle_StackTraceEndM("GA::EvolverFloatVector::EvolverFloatVector(EvaluationOp::Handle inEvalOp, UIntArray inInitSize)");
}


/*!
 *  \brief Initialize the evolver, its operators and the system.
 *  \param ioSystem Handle to the system of the evolution.
 *  \param ioArgc Number of elements on the command-line.
 *  \param ioArgv Element on the command-line.
 */
void GA::EvolverFloatVector::initialize(Beagle::System::Handle ioSystem, int ioArgc, char** ioArgv)
{
  Beagle_StackTraceBeginM();
  ioSystem->addComponent(new GA::CMAHolder);
  Beagle::Evolver::initialize(ioSystem, ioArgc, ioArgv);
  Beagle_StackTraceEndM("void GA::EvolverFloatVector::initialize(System::Handle ioSystem, int, char**)");
}


/*!
 *  \brief Initialize the evolver, its operators and the system.
 *  \param ioSystem Handle to the system of the evolution.
 *  \param inConfigFilename Configuration file from which system and evolver are read.
 */
void GA::EvolverFloatVector::initialize(Beagle::System::Handle ioSystem, std::string inConfigFilename)
{
  Beagle_StackTraceBeginM();
  ioSystem->addComponent(new GA::CMAHolder);
  Beagle::Evolver::initialize(ioSystem, inConfigFilename);
  Beagle_StackTraceEndM("void GA::EvolverFloatVector::initialize(System::Handle ioSystem, std::string)");
}
