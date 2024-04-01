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
 *  \file   SymbRegEvalOp.cpp
 *  \brief  Implementation of the class SymbRegEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.1 $
 *  $Date: 2007/05/09 01:51:24 $
 */

#include "SymbRegEvalOp.hpp"

#include <cmath>

using namespace Beagle;

/*!
 *  \brief Construct a new symbolic regression evaluation operator.
 *  \param inName Name of the evaluation operator.
 */
SymbRegEvalOp::SymbRegEvalOp(std::string inName) :
  GP::EvaluationOp(inName.c_str()),
  mX(0),
  mY(0)
{ }


/*!
 *  \brief Evaluate the individual fitness for the symbolic regression problem.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure,
 */
Fitness::Handle SymbRegEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{ 
  using namespace std;
  cout << "Evaluating individual's fitness:" << endl;
  double lSquareError = 0.0;
  for(unsigned int i=0; i<mX.size(); i++) {
    setValue("X", mX[i], ioContext);
    Double lResult;
    inIndividual.run(lResult, ioContext);
    cout << "  Result = " << lResult << endl;
    double lError = mY[i]-lResult;
    lSquareError += (lError*lError);
  }
  double lMSE  = lSquareError / mX.size();
  double lRMSE = sqrt(lMSE);
  double lFitness = (1.0 / (lRMSE + 1.0));
  return new FitnessSimple(lFitness);
}


/*!
 * \brief Post-initialize the operator by sampling the function to regress.
 * \param ioSystem System to use to sample.
 */
void SymbRegEvalOp::postInit(System& ioSystem)
{
  GP::EvaluationOp::postInit(ioSystem);
  
  for(unsigned int i=0; i<1; i++) {
    mX.push_back(ioSystem.getRandomizer().rollUniform(-1.0,1.0));
    std::cout << "case #" << i << ": X = " << mX.back() << std::endl;
    mY.push_back(mX[i]*(mX[i]*(mX[i]*(mX[i]+1.0)+1.0)+1.0));
  }
}
