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
 *  \file   beagle/src/StatsCalcFitnessSimpleOp.cpp
 *  \brief  Source code of class StatsCalcFitnessSimpleOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Beagle.hpp"

#include <cmath>
#include <sstream>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a calculate stats operator.
 *  \param inName Name of the operator.
 */
StatsCalcFitnessSimpleOp::StatsCalcFitnessSimpleOp(Beagle::string inName) :
  StatsCalculateOp(inName)
{ }


/*!
 *  \brief Calculate statistics of a given deme.
 *  \param outStats Evaluated statistics.
 *  \param ioDeme Deme to evalute the statistics.
 *  \param ioContext Context of the evolution.
 */
void StatsCalcFitnessSimpleOp::calculateStatsDeme(Stats& outStats,
                                                  Deme& ioDeme,
                                                  Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  outStats.clear();
  outStats.clearItems();
  outStats.addItem("processed", ioContext.getProcessedDeme());
  outStats.addItem("total-processed", ioContext.getTotalProcessedDeme());

  if(ioDeme.size() == 0) {
    outStats.setGenerationValues(string("deme")+uint2str(ioContext.getDemeIndex()+1),
                                 ioContext.getGeneration(), 0, true);
    outStats.resize(1);
    outStats.back().mId  = "fitness";
    outStats.back().mAvg = 0.0;
    outStats.back().mStd = 0.0;
    outStats.back().mMax = 0.0;
    outStats.back().mMin = 0.0;
    return;
  }

  const FitnessSimple::Handle lFirstIndivFitness =
    castHandleT<FitnessSimple>(ioDeme[0]->getFitness());

  if(ioDeme.size() == 1) {
    outStats.setGenerationValues(string("deme")+uint2str(ioContext.getDemeIndex()+1),
                                 ioContext.getGeneration(), 1, true);
    outStats.resize(1);
    outStats.back().mId  = "fitness";
    outStats.back().mAvg = lFirstIndivFitness->getValue();
    outStats.back().mStd = 0.0;
    outStats.back().mMax = lFirstIndivFitness->getValue();
    outStats.back().mMin = lFirstIndivFitness->getValue();
    return;
  }

  double lSum = (double)lFirstIndivFitness->getValue();
  double lPow2Sum = pow2Of<double>((double)lFirstIndivFitness->getValue());
  double lMax = lFirstIndivFitness->getValue();
  double lMin = lFirstIndivFitness->getValue();

  for(unsigned int i=1; i<ioDeme.size(); i++) {
    const FitnessSimple::Handle lIndivFitness =
      castHandleT<FitnessSimple>(ioDeme[i]->getFitness());
    lSum     += (double)lIndivFitness->getValue();
    lPow2Sum += pow2Of<double>((double)lIndivFitness->getValue());
    lMax     =  maxOf<double>(lMax, (double)lIndivFitness->getValue());
    lMin     =  minOf<double>(lMin, (double)lIndivFitness->getValue());
  }

  double lAverage  = lSum / ioDeme.size();
  double lStdError = (lPow2Sum - (pow2Of<double>(lSum)/ioDeme.size())) / (ioDeme.size()-1);
  lStdError        = sqrt(lStdError);

  outStats.setGenerationValues(string("deme")+uint2str(ioContext.getDemeIndex()+1),
                               ioContext.getGeneration(),
                               ioDeme.size(),
                               true);
  outStats.resize(1);
  outStats.back().mId  = "fitness";
  outStats.back().mAvg = lAverage;
  outStats.back().mStd = lStdError;
  outStats.back().mMax = lMax;
  outStats.back().mMin = lMin;
  Beagle_StackTraceEndM("void StatsCalcFitnessSimpleOp::calculateStatsDeme(Stats& outStats, Deme& ioDeme, Context& ioContext) const");
}


