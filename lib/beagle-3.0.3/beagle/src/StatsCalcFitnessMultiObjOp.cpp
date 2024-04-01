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
 *  \file   beagle/src/StatsCalcFitnessMultiObjOp.cpp
 *  \brief  Source code of class StatsCalcFitnessMultiObjOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Beagle.hpp"

#include <cmath>
#include <sstream>

using namespace Beagle;


/*!
 *  \brief Construct a calculate stats operator.
 *  \param inName Name of the operator.
 */
StatsCalcFitnessMultiObjOp::StatsCalcFitnessMultiObjOp(Beagle::string inName) :
  StatsCalculateOp(inName)
{ }


/*!
 *  \brief Calculate statistics of a given deme.
 *  \param outStats Evaluated statistics.
 *  \param ioDeme Deme to evalute the statistics.
 *  \param ioContext Context of the evolution.
 */
void StatsCalcFitnessMultiObjOp::calculateStatsDeme(Stats& outStats,
                                                    Deme& ioDeme,
                                                    Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  outStats.clear();
  outStats.clearItems();
  outStats.addItem("processed", ioContext.getProcessedDeme());
  outStats.addItem("total-processed", ioContext.getTotalProcessedDeme());

  if(ioDeme.size() == 0) {
    outStats.clear();
    outStats.setGenerationValues(string("deme")+uint2str(ioContext.getDemeIndex()+1),
                                 ioContext.getGeneration(), 0, true);
    return;
  }

  outStats.clear();
  outStats.setGenerationValues(string("deme")+uint2str(ioContext.getDemeIndex()+1),
                               ioContext.getGeneration(), ioDeme.size(), true);

  const FitnessMultiObj::Handle lFirstIndivFitness =
    castHandleT<FitnessMultiObj>(ioDeme[0]->getFitness());

  for(unsigned int j=0; j<lFirstIndivFitness->size(); ++j) {
    if(ioDeme.size() == 1) {
      outStats.push_back(Measure(string("objective")+uint2str(j),
                                 (*lFirstIndivFitness)[j],
                                 0.0,
                                 (*lFirstIndivFitness)[j],
                                 (*lFirstIndivFitness)[j]));
    }
    else {
      double lSum = (double)(*lFirstIndivFitness)[j];
      double lPow2Sum = pow2Of<double>((double)(*lFirstIndivFitness)[j]);
      double lMax = (*lFirstIndivFitness)[j];
      double lMin = (*lFirstIndivFitness)[j];
      for(unsigned int i=1; i<ioDeme.size(); i++) {
        const FitnessMultiObj::Handle lIndivFitness =
          castHandleT<FitnessMultiObj>(ioDeme[i]->getFitness());
        Beagle_AssertM(lFirstIndivFitness->size() == lIndivFitness->size());
        lSum     += (double)(*lIndivFitness)[j];
        lPow2Sum += pow2Of<double>((double)(*lIndivFitness)[j]);
        lMax     =  maxOf<double>(lMax, (double)(*lIndivFitness)[j]);
        lMin     =  minOf<double>(lMin, (double)(*lIndivFitness)[j]);
      }
      double lAverage  = lSum / ioDeme.size();
      double lStdError = (lPow2Sum - (pow2Of<double>(lSum)/ioDeme.size())) / (ioDeme.size()-1);
      lStdError        = sqrt(lStdError);
      outStats.push_back(Measure(string("objective")+uint2str(j),
                                 lAverage, lStdError, lMax, lMin));
    }
  }
  Beagle_StackTraceEndM("void StatsCalcFitnessMultiObjOp::calculateStatsDeme(Stats& outStats, Deme& ioDeme, Context& ioContext) const");
}
