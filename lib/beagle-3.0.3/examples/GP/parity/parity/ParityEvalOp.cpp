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
 *  \file   ParityEvalOp.cpp
 *  \brief  Implementation of the class ParityEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.2 $
 *  $Date: 2007/05/11 20:04:33 $
 */

#include "beagle/GP.hpp"
#include "ParityEvalOp.hpp"

#include <cmath>

using namespace Beagle;


/*!
 *  \brief Construct a new even-6 parity evaluation operator.
 */
ParityEvalOp::ParityEvalOp() :
  GP::EvaluationOp("ParityEvalOp"),
  mInputs(ParitySizeM),
  mOutputs(ParitySizeM)
{ }


/*!
 * \brief Initialize the even-6 parity evaluation operator by creating the test tables.
 * \param ioSystem System to use to initialize the operator.
 */
void ParityEvalOp::postInit(System& ioSystem)
{
  GP::EvaluationOp::initialize(ioSystem);
  mInputs.resize(ParitySizeM);
  mOutputs.resize(ParitySizeM);
  for(unsigned int i=0; i<ParitySizeM; i++) {
    mInputs[i].resize(ParityFanInM);
    unsigned int lValue = i;
    unsigned int lDividor = ParitySizeM;
    bool lParity  = true;
    for(unsigned int j=0; j<ParityFanInM; j++) {
      lDividor /= 2;
      if(lValue >= lDividor) {
        mInputs[i][j] = true;
        lParity = !lParity;
        lValue  -= lDividor;
      }
      else mInputs[i][j] = false;
    }
    mOutputs[i] = lParity;
  }
}


/*!
 *  \brief Evaluate the individual fitness for the even-6 parity problem.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure,
 */
Fitness::Handle ParityEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{
  unsigned int lGood = 0;
  for(unsigned int i=0; i<ParitySizeM; i++) {
    for(unsigned int j=0; j<ParityFanInM; j++) {
      Beagle::string lName = "IN";
      lName += uint2str(j);
      setValue(lName, mInputs[i][j], ioContext);
    }
    Bool lResult;
    inIndividual.run(lResult, ioContext);
    if(lResult.getWrappedValue() == mOutputs[i].getWrappedValue()) lGood++;
  }
  double lFitness = double(lGood) / ParitySizeM;
  return new FitnessSimple(lFitness);
}
