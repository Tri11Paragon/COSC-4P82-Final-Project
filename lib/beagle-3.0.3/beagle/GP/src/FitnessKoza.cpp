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
 *  \file   beagle/GP/src/FitnessKoza.cpp
 *  \brief  Source code of class FitnessKoza.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.1 $
 *  $Date: 2007/05/09 01:51:06 $
 */

#include "beagle/GP.hpp"

#include <cmath>
#include <cfloat>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Default construct a Koza's fitness object.
 */
GP::FitnessKoza::FitnessKoza() :
  FitnessSimple(),
  mAdjustedFitness(0.),
  mStandardizedFitness(0.),
  mRawFitness(0.),
  mHits(0)
{ }


/*!
 *  \brief Construct a valid Koza's fitness object.
 *  \param inNormalizedFitness Normalized fitness value.
 *  \param inAdjustedFitness Adjusted fitness value.
 *  \param inStandardizedFitness Standardized fitness value.
 *  \param inRawFitness Raw fitness value.
 *  \param inHits Number of hits.
 */
GP::FitnessKoza::FitnessKoza(float inNormalizedFitness,
                             float inAdjustedFitness,
                             float inStandardizedFitness,
                             float inRawFitness,
                             unsigned int inHits)
{
  Beagle_StackTraceBeginM();
  setFitness(inNormalizedFitness,
             inAdjustedFitness,
             inStandardizedFitness,
             inRawFitness,
             inHits);
  Beagle_StackTraceEndM("GP::FitnessKoza::FitnessKoza(float inNormalizedFitness, float inAdjustedFitness, float inStandardizedFitness, float inRawFitness, unsigned int inHits)");
}


/*!
 *  \brief Calculate statistics of a given deme.
 *  \param ioDeme Deme to evalute the statistics.
 *  \param ioContext Context of the evolution.
 *  \return Handle to a generationnal statistics of the given deme.
 */
void GP::FitnessKoza::calculateStats(Beagle::Deme& ioDeme, Beagle::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  if(ioDeme.size() == 0) {
    Stats& lStats = *ioDeme.getStats();
    lStats.setGenerationValues(string("deme")+uint2str(ioContext.getDemeIndex()+1),
                               ioContext.getGeneration(),
                               0,
                               true);

    lStats.resize(7);
    lStats[0].mId = "normalized";
    lStats[0].mAvg = 0.0;
    lStats[0].mStd = 0.0;
    lStats[0].mMax = 0.0;
    lStats[0].mMin = 0.0;

    lStats[1].mId = "adjusted";
    lStats[1].mAvg = 0.0;
    lStats[1].mStd = 0.0;
    lStats[1].mMax = 0.0;
    lStats[1].mMin = 0.0;

    lStats[2].mId = "standardized";
    lStats[2].mAvg = 0.0;
    lStats[2].mStd = 0.0;
    lStats[2].mMax = 0.0;
    lStats[2].mMin = 0.0;

    lStats[3].mId = "raw";
    lStats[3].mAvg = 0.0;
    lStats[3].mStd = 0.0;
    lStats[3].mMax = 0.0;
    lStats[3].mMin = 0.0;

    lStats[4].mId = "hits";
    lStats[4].mAvg = 0.0;
    lStats[4].mStd = 0.0;
    lStats[4].mMax = 0.0;
    lStats[4].mMin = 0.0;

    lStats[5].mId = "treedepth";
    lStats[5].mAvg = 0.0;
    lStats[5].mStd = 0.0;
    lStats[5].mMax = 0.0;
    lStats[5].mMin = 0.0;

    lStats[6].mId = "treesize";
    lStats[6].mAvg = 0.0;
    lStats[6].mStd = 0.0;
    lStats[6].mMax = 0.0;
    lStats[6].mMin = 0.0;
    return;
  }

  const GP::Deme&    lGPDeme    = castObjectT<const GP::Deme&>(ioDeme);
  const GP::FitnessKoza::Handle lFirstIndivFitness =
    castHandleT<GP::FitnessKoza>(lGPDeme[0]->getFitness());
  double lSumNrm         = (double)lFirstIndivFitness->getNormalizedFitness();
  double lPow2SumNrm     = pow2Of<double>(lSumNrm);
  double lMaxNrm         = (double)lFirstIndivFitness->getNormalizedFitness();
  double lMinNrm         = (double)lFirstIndivFitness->getNormalizedFitness();
  double lSumAdj         = (double)lFirstIndivFitness->getAdjustedFitness();
  double lPow2SumAdj     = pow2Of<double>(lSumAdj);
  double lMaxAdj         = (double)lFirstIndivFitness->getAdjustedFitness();
  double lMinAdj         = (double)lFirstIndivFitness->getAdjustedFitness();
  double lSumStd         = (double)lFirstIndivFitness->getStandardizedFitness();
  double lPow2SumStd     = pow2Of<double>(lSumStd);
  double lMaxStd         = (double)lFirstIndivFitness->getStandardizedFitness();
  double lMinStd         = (double)lFirstIndivFitness->getStandardizedFitness();
  double lSumRaw         = (double)lFirstIndivFitness->getRawFitness();
  double lPow2SumRaw     = pow2Of<double>(lSumRaw);
  double  lMaxRaw        = (double)lFirstIndivFitness->getRawFitness();
  double lMinRaw         = (double)lFirstIndivFitness->getRawFitness();
  double lSumHit         = (double)lFirstIndivFitness->getHits();
  double lPow2SumHit     = pow2Of<double>(lSumHit);
  unsigned int lMaxHit   = lFirstIndivFitness->getHits();
  unsigned int lMinHit   = lFirstIndivFitness->getHits();

  unsigned int lMaxDepth = lGPDeme[0]->getMaxTreeDepth();
  unsigned int lMinDepth = lMaxDepth;
  double lSumDepth       = (double)lMaxDepth;
  double lPow2SumDepth   = pow2Of<double>(lSumDepth);
  unsigned int lMaxSize  = lGPDeme[0]->getTotalNodes();
  unsigned int lMinSize  = lMaxSize;
  double lSumSize        = (double)lMaxSize;
  double lPow2SumSize    = pow2Of<double>(lSumSize);  

  for(unsigned int i=1; i<lGPDeme.size(); i++) {
    const GP::FitnessKoza::Handle lIndivFitness =
      castHandleT<GP::FitnessKoza>(lGPDeme[i]->getFitness());
    lSumNrm     += (double)lIndivFitness->getNormalizedFitness();
    lPow2SumNrm += pow2Of<double>((double)lIndivFitness->getNormalizedFitness());
    lSumAdj     += (double)lIndivFitness->getAdjustedFitness();
    lPow2SumAdj += pow2Of<double>((double)lIndivFitness->getAdjustedFitness());
    lSumStd     += (double)lIndivFitness->getStandardizedFitness();
    lPow2SumStd += pow2Of<double>(lIndivFitness->getStandardizedFitness());
    lSumRaw     += (double)lIndivFitness->getRawFitness();
    lPow2SumRaw += pow2Of<double>((double)lIndivFitness->getRawFitness());
    lSumHit     += (double)lIndivFitness->getHits();
    lPow2SumHit += pow2Of<double>((double)lIndivFitness->getHits());
    if(((double)lIndivFitness->getNormalizedFitness()) > lMaxNrm) {
      lMaxNrm = lIndivFitness->getNormalizedFitness();
      lMaxAdj = lIndivFitness->getAdjustedFitness();
      lMaxStd = lIndivFitness->getStandardizedFitness();
      lMaxRaw = lIndivFitness->getRawFitness();
      lMaxHit = lIndivFitness->getHits();
    }
    if(((double)lIndivFitness->getNormalizedFitness()) < lMinNrm) {
      lMinNrm = lIndivFitness->getNormalizedFitness();
      lMinAdj = lIndivFitness->getAdjustedFitness();
      lMinStd = lIndivFitness->getStandardizedFitness();
      lMinRaw = lIndivFitness->getRawFitness();
      lMinHit = lIndivFitness->getHits();
    }

    unsigned int lTmpDepth = lGPDeme[i]->getMaxTreeDepth();
    lSumDepth     += (double)lTmpDepth;
    lPow2SumDepth += pow2Of<double>((double)lTmpDepth);
    lMaxDepth     =  maxOf(lMaxDepth, lTmpDepth);
    lMinDepth     =  minOf(lMinDepth, lTmpDepth);

    unsigned int lTmpSize = lGPDeme[i]->getTotalNodes();
    lSumSize     += (double)lTmpSize;
    lPow2SumSize += pow2Of<double>((double)lTmpSize);
    lMaxSize     =  maxOf(lMaxSize, lTmpSize);
    lMinSize     =  minOf(lMinSize, lTmpSize);
  }

  double lNrmAverage  = (lSumNrm / lGPDeme.size());
  double lNrmStdError = 
    (lPow2SumNrm - (pow2Of<double>(lSumNrm) / lGPDeme.size())) / (lGPDeme.size() - 1);
  lNrmStdError       = sqrt(lNrmStdError);

  double lAdjAverage  = (lSumAdj / lGPDeme.size());
  double lAdjStdError = 
    (lPow2SumAdj - (pow2Of<double>(lSumAdj) / lGPDeme.size())) / (lGPDeme.size() - 1);
  lAdjStdError       = sqrt(lAdjStdError);

  double lStdAverage  = (lSumStd / lGPDeme.size());
  double lStdStdError = 
    (lPow2SumStd - (pow2Of<double>(lSumStd) / lGPDeme.size())) / (lGPDeme.size() - 1);
  lStdStdError       = sqrt(lStdStdError);

  double lRawAverage  = (lSumRaw / lGPDeme.size());
  double lRawStdError = 
    (lPow2SumRaw - (pow2Of<double>(lSumRaw) / lGPDeme.size())) / (lGPDeme.size() - 1);
  lRawStdError       = sqrt(lRawStdError);

  double lHitAverage  = (lSumHit / lGPDeme.size());
  double lHitStdError =
    (lPow2SumHit - (pow2Of<double>(lSumHit) / lGPDeme.size())) / (lGPDeme.size() - 1);
  lHitStdError       = sqrt(lHitStdError);

  double lDepthAverage  = (lSumDepth / lGPDeme.size());
  double lDepthStdError =
    (lPow2SumDepth - (pow2Of<double>(lSumDepth) / lGPDeme.size())) / (lGPDeme.size() - 1);
  lDepthStdError       = sqrt(lDepthStdError);

  double lSizeAverage  = (lSumSize / lGPDeme.size());
  double lSizeStdError =
    (lPow2SumSize - (pow2Of<double>(lSumSize) / lGPDeme.size())) / (lGPDeme.size() - 1);
  lSizeStdError       = sqrt(lSizeStdError);  

  Stats& lStats = *ioDeme.getStats();
  lStats.setGenerationValues(string("deme")+uint2str(ioContext.getDemeIndex()+1),
                             ioContext.getGeneration(),
                             ioDeme.size(),
                             true);

  lStats.resize(7);
  lStats[0].mId = "normalized";
  lStats[0].mAvg = lNrmAverage;
  lStats[0].mStd = lNrmStdError;
  lStats[0].mMax = lMaxNrm;
  lStats[0].mMin = lMinNrm;

  lStats[1].mId = "adjusted";
  lStats[1].mAvg = lAdjAverage;
  lStats[1].mStd = lAdjStdError;
  lStats[1].mMax = lMaxAdj;
  lStats[1].mMin = lMinAdj;

  lStats[2].mId = "standardized";
  lStats[2].mAvg = lStdAverage;
  lStats[2].mStd = lStdStdError;
  lStats[2].mMax = lMaxStd;
  lStats[2].mMin = lMinStd;

  lStats[3].mId = "raw";
  lStats[3].mAvg = lRawAverage;
  lStats[3].mStd = lRawStdError;
  lStats[3].mMax = lMaxRaw;
  lStats[3].mMin = lMinRaw;

  lStats[4].mId = "hits";
  lStats[4].mAvg = lHitAverage;
  lStats[4].mStd = lHitStdError;
  lStats[4].mMax = (float)lMaxHit;
  lStats[4].mMin = (float)lMinHit;

  lStats[5].mId = "treedepth";
  lStats[5].mAvg = lDepthAverage;
  lStats[5].mStd = lDepthStdError;
  lStats[5].mMax = (float)lMaxDepth;
  lStats[5].mMin = (float)lMinDepth;

  lStats[6].mId = "treesize";
  lStats[6].mAvg = lSizeAverage;
  lStats[6].mStd = lSizeStdError;
  lStats[6].mMax = (float)lMaxSize;
  lStats[6].mMin = (float)lMinSize;
  Beagle_StackTraceEndM("void GP::FitnessKoza::calculateStats(Beagle::Deme& ioDeme, Beagle::Context& ioContext) const");
}


/*!
 *  \brief Read a Koza's fitness from a XML subtree.
 *  \param inIter XML iterator to use to read the fitness values.
 */
void GP::FitnessKoza::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Fitness"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Fitness> expected!");

  string lType = inIter->getAttribute("type").c_str();
  if(lType != "koza") throw Beagle_IOExceptionNodeM(*inIter, "fitness type mismatch!");

  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      if(lChild->getValue() == "Normalized") {
        PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
        if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no normalized fitness value present!");
        if(lChild2->getType() != PACC::XML::eString)
          throw Beagle_IOExceptionNodeM(*lChild2, "no normalized fitness value present!");
        mFitness = str2dbl(lChild2->getValue().c_str());
      }
      else if(lChild->getValue() == "Adjusted") {
        PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
        if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no adjusted fitness value present!");
        if(lChild2->getType() != PACC::XML::eString)
          throw Beagle_IOExceptionNodeM(*lChild2, "no adjusted fitness value present!");
        mAdjustedFitness = str2dbl(lChild2->getValue().c_str());
      }
      else if(lChild->getValue() == "Standardized") {
        PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
        if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no standardized fitness value present!");
        if(lChild2->getType() != PACC::XML::eString)
          throw Beagle_IOExceptionNodeM(*lChild2, "no standardized fitness value present!");
        mStandardizedFitness = str2dbl(lChild2->getValue().c_str());
      }
      else if(lChild->getValue() == "Raw") {
        PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
        if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no raw fitness value present!");
        if(lChild2->getType() != PACC::XML::eString)
          throw Beagle_IOExceptionNodeM(*lChild2, "no raw fitness value present!");
        mRawFitness = str2dbl(lChild2->getValue().c_str());
      }
      else if(lChild->getValue() == "Hits") {
        PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
        if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no hits value present!");
        if(lChild2->getType() != PACC::XML::eString)
          throw Beagle_IOExceptionNodeM(*lChild2, "no hits value present!");
        mHits = str2uint(lChild2->getValue().c_str());
      }
    }
  }
  Beagle_StackTraceEndM("void GP::FitnessKoza::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Set the fitness value and validity.
 *  \param inNormalizedFitness Normalized fitness value.
 *  \param inAdjustedFitness Adjusted fitness value.
 *  \param inStandardizedFitness Standardized fitness value.
 *  \param inRawFitness Raw fitness value.
 *  \param inHits Number of hits.
 */
void GP::FitnessKoza::setFitness(float inNormalizedFitness,
                                 float inAdjustedFitness,
                                 float inStandardizedFitness,
                                 float inRawFitness,
                                 unsigned int inHits)
{
  Beagle_StackTraceBeginM();
  if(isFinite(inNormalizedFitness)) mFitness = inNormalizedFitness;
  else mFitness = FLT_MIN;
  if(isFinite(inAdjustedFitness)) mAdjustedFitness = inAdjustedFitness;
  else mAdjustedFitness = 0.;
  if(isFinite(inStandardizedFitness)) mStandardizedFitness = inStandardizedFitness;
  else mStandardizedFitness = 0.;
  if(isFinite(inRawFitness)) mRawFitness = inRawFitness;
  else mRawFitness = 0.;
  mHits = inHits;
  setValid();
  Beagle_StackTraceEndM("void GP::FitnessKoza::setFitness(float inNormalizedFitness, float inAdjustedFitness, float inStandardizedFitness, float inRawFitness, unsigned int inHits)");
}


/*!
 *  \brief Write a Koza's fitness into a Beagle XML streamer.
 *  \param ioStreamer XML streamer to use to write the fitness values.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::FitnessKoza::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Fitness", inIndent);
  ioStreamer.insertAttribute("type", "koza");
  ioStreamer.openTag("Normalized", false);
  ioStreamer.insertStringContent(dbl2str(mFitness).c_str());
  ioStreamer.closeTag();
  ioStreamer.openTag("Adjusted", false);
  ioStreamer.insertStringContent(dbl2str(mAdjustedFitness).c_str());
  ioStreamer.closeTag();
  ioStreamer.openTag("Standardized", false);
  ioStreamer.insertStringContent(dbl2str(mStandardizedFitness).c_str());
  ioStreamer.closeTag();
  ioStreamer.openTag("Raw", false);
  ioStreamer.insertStringContent(dbl2str(mRawFitness).c_str());
  ioStreamer.closeTag();
  ioStreamer.openTag("Hits", false);
  ioStreamer.insertStringContent(uint2str(mHits).c_str());
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void GP::FitnessKoza::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

