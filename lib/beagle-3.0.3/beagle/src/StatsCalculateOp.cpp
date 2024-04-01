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
 *  \file   beagle/src/StatsCalculateOp.cpp
 *  \brief  Source code of class StatsCalculateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.1 $
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
StatsCalculateOp::StatsCalculateOp(Beagle::string inName) :
  Operator(inName),
  mPopSize(NULL),
  mNbDemesCalculated(0),
  mGenerationCalculated(0)
{ }


/*!
 *  \brief  Calculate statistics of a given vivarium.
 *  \param  outStats Computed stats of the deme.
 *  \param  ioVivarium Vivarium to evaluate the statistics.
 *  \param  ioContext Context of the evolution.
 */
void StatsCalculateOp::calculateStatsVivarium(Stats& outStats,
                                              Vivarium& ioVivarium,
                                              Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  outStats.clear();
  outStats.clearItems();

  unsigned int lPopSize = 0;
  unsigned int lProcessed = 0;
  unsigned int lTotalProcessed = 0;
  for(unsigned int i=0; i<ioVivarium.size(); ++i) {
    lPopSize += ioVivarium[i]->getStats()->getPopSize();
    if(ioVivarium[i]->getStats()->existItem("processed")) {
      lProcessed += (unsigned int)ioVivarium[i]->getStats()->getItem("processed");
    }
    if(ioVivarium[i]->getStats()->existItem("total-processed")) {
      lTotalProcessed += (unsigned int)ioVivarium[i]->getStats()->getItem("total-processed");
    }
  }
  outStats.setGenerationValues("vivarium", ioContext.getGeneration(), lPopSize, true);
  outStats.addItem("processed", lProcessed);
  outStats.addItem("total-processed", lTotalProcessed);

  if(ioVivarium.size() == 0) return;

  outStats.resize(ioVivarium[0]->getStats()->size());
  for(unsigned int i=0; i<outStats.size(); ++i) {
    double lSumXi     = 0.0;
    double lSumXiPow2 = 0.0;
    double lMax       = (*ioVivarium[0]->getStats())[i].mMax;
    double lMin       = (*ioVivarium[0]->getStats())[i].mMin;

    for(unsigned int j=0; j<ioVivarium.size(); ++j) {
      Beagle_AssertM(outStats.size() == ioVivarium[j]->getStats()->size());
      const Measure& lMeasure  = (*ioVivarium[j]->getStats())[i];
      unsigned int   lDemeSize = ioVivarium[j]->getStats()->getPopSize();
      if(lDemeSize != 0) {
        double lSumFit  =  lMeasure.mAvg * lDemeSize;
        double lTmpVar1 =  pow2Of<double>(lSumFit) / lDemeSize;
        double lTmpVar2 =  pow2Of<double>(lMeasure.mStd) * (lDemeSize-1);
        lSumXi          += lSumFit;
        lSumXiPow2      += lTmpVar1 + lTmpVar2;
        lMax            =  maxOf<double>(lMax, lMeasure.mMax);
        lMin            =  minOf<double>(lMin, lMeasure.mMin);
      }
    }

    outStats[i].mId  = (*ioVivarium[0]->getStats())[i].mId;
    if(lPopSize == 0) {
      outStats[i].mAvg = 0.0;
      outStats[i].mStd = 0.0;
      outStats[i].mMax = 0.0;
      outStats[i].mMin = 0.0;
    }
    else if(lPopSize == 1) {
      outStats[i].mAvg = lSumXi;
      outStats[i].mStd = 0.0;
      outStats[i].mMax = lMax;
      outStats[i].mMin = lMin;
    }
    else {
      double lAverage  = lSumXi / lPopSize;
      double lVariance =
        lSumXiPow2 - (pow2Of<double>(lAverage*((double)lPopSize)) / ((double)lPopSize));
      double lStdError = sqrt(lVariance / (lPopSize - 1));
      outStats[i].mAvg = lAverage;
      outStats[i].mStd = lStdError;
      outStats[i].mMax = lMax;
      outStats[i].mMin = lMin;
    }
  }
  Beagle_StackTraceEndM("void StatsCalculateOp::calculateStatsVivarium(Stats& outStats, Vivarium& ioVivarium, Context& ioContext) const");
}


/*!
 *  \brief Initialize the calculate statistics operator.
 *  \param ioSystem Reference to the system to use for the initialization.
 */
void StatsCalculateOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ec.pop.size")) {
    mPopSize = castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
  } else {
    mPopSize = new UIntArray(1,100);
    string lLongDescrip("Number of demes and size of each deme of the population. ");
    lLongDescrip += "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
    lLongDescrip += "The size of the UIntArray is the number of demes present in the ";
    lLongDescrip += "vivarium, while each value of the vector is the size of the corresponding ";
    lLongDescrip += "deme.";
    Register::Description lDescription(
      "Vivarium and demes sizes",
      "UIntArray",
      "100",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
  }
  Beagle_StackTraceEndM("void StatsCalculateOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Calculate the statistics of the current deme/generation.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 */
void StatsCalculateOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "stats", "Beagle::StatsCalculateOp",
    string("Calculating stats for the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );

  if(ioContext.getGeneration() != mGenerationCalculated) {
    mGenerationCalculated = ioContext.getGeneration();
    mNbDemesCalculated = 0;
  }

  if(ioDeme.getStats()->isValid() == false) {
    calculateStatsDeme(*ioDeme.getStats(), ioDeme, ioContext);
    ioDeme.getStats()->setValid();
  }
  Beagle_LogObjectM(
    ioContext.getSystem().getLogger(),
    Logger::eStats,
    "stats", "Beagle::StatsCalculateOp",
    *ioDeme.getStats()
  );

  if(++mNbDemesCalculated == mPopSize->size()) {
    Beagle_LogTraceM(
      ioContext.getSystem().getLogger(),
      "stats", "Beagle::StatsCalculateOp",
      "Calculating stats for the vivarium"
    );
    calculateStatsVivarium(*ioContext.getVivarium().getStats(),
                           ioContext.getVivarium(),
                           ioContext);
    ioContext.getVivarium().getStats()->setValid();
    Beagle_LogObjectM(
      ioContext.getSystem().getLogger(),
      Logger::eStats,
      "stats", "Beagle::StatsCalculateOp",
      *ioContext.getVivarium().getStats()
    );
  }
  Beagle_StackTraceEndM("void StatsCalculateOp::operate(Deme& ioDeme, Context& ioContext)");
}

