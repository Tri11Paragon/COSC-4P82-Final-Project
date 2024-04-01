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
 *  \file   beagle/src/NSGA2Op.cpp
 *  \brief  Source code of class NSGA2Op.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <cfloat>
#include <float.h>     // To get FLT_MAX on Borland C++ Builder
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Build NSGA2 replacement strategy operator.
 *  \param inLMRatioName Lamda over Mu parameter name used in the register.
 *  \param inName Name of the NSGA2 operator.
 */
NSGA2Op::NSGA2Op(Beagle::string inLMRatioName, Beagle::string inName) :
  ReplacementStrategyOp(inName),
  mLMRatioName(inLMRatioName)
{ }


/*!
 *  \brief Apply NSGA2 multiobjective selection operator as a replacement strategy.
 *  \param ioDeme Deme on which selection operator is applied.
 *  \param ioContext Evolutionary context.
 */
void NSGA2Op::applyAsReplacementStrategy(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::NSGA2Op",
    string("Processing using NSGA2 replacement strategy the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  Beagle_LogObjectM(
    ioContext.getSystem().getLogger(),
    Logger::eTrace,
    "replacement-strategy", "Beagle::NSGA2Op",
    (*this)
  );

  // Generate a new generation of individuals, merged with the actual one.
  const unsigned int lLambda =
    (unsigned int)std::ceil(mLMRatio->getWrappedValue()*float(ioDeme.size()));
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::NSGA2Op",
    string("Generating ")+uint2str(lLambda)+
    string(" offsprings using breeder tree")
  );
  RouletteT<unsigned int> lRoulette;
  buildRoulette(lRoulette, ioContext);
  Individual::Bag lOffsprings(ioDeme);
  for(unsigned int i=0; i<lLambda; ++i) {
    unsigned int lIndexBreeder = lRoulette.select(ioContext.getSystem().getRandomizer());
    BreederNode::Handle lSelectedBreeder=getRootNode();
    for(unsigned int j=0; j<lIndexBreeder; ++j)
      lSelectedBreeder=lSelectedBreeder->getNextSibling();
    Beagle_NonNullPointerAssertM(lSelectedBreeder);
    Beagle_NonNullPointerAssertM(lSelectedBreeder->getBreederOp());
    Individual::Handle lBredIndiv =
      lSelectedBreeder->getBreederOp()->breed(ioDeme, lSelectedBreeder->getFirstChild(), ioContext);
    Beagle_NonNullPointerAssertM(lBredIndiv);
    lOffsprings.push_back(lBredIndiv);
  }

  // Fast non-dominated sorting, followed by insertion of the first Pareto fronts.
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::NSGA2Op",
    "Applying fast non-dominated sorting on the union of parents and offsprings individual"
  );
  NSGA2Op::Fronts lParetoFronts;
  sortFastND(lParetoFronts, ioDeme.size(), lOffsprings, ioContext);
  unsigned int lIndexDeme=0;
  for(unsigned int j=0; j<(lParetoFronts.size()-1); ++j) {
    for(unsigned int k=0; k<lParetoFronts[j].size(); ++k) {
      ioDeme[lIndexDeme++] = lOffsprings[lParetoFronts[j][k]];
    }
  }

  // Insertion of the last Pareto front, using crowding distance
  Individual::Bag lLastFrontIndiv;
  for(unsigned int l=0; l<lParetoFronts.back().size(); ++l) {
    lLastFrontIndiv.push_back(lOffsprings[lParetoFronts.back()[l]]);
  }
  NSGA2Op::Distances lDistances;
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::NSGA2Op",
    string("Computing crowding distance on the ")+uint2ordinal(lParetoFronts.size())+
    string(" Pareto front, which is made of ")+uint2str(lParetoFronts.back().size())+
    " individuals"
  );
  evalCrowdingDistance(lDistances, lLastFrontIndiv);
  for(unsigned int m=0; lIndexDeme<ioDeme.size(); ++m) {
    ioDeme[lIndexDeme++] = lLastFrontIndiv[lDistances[m].second];
  }
  Beagle_StackTraceEndM("void NSGA2Op::applyAsReplacementStrategy(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Apply NSGA2 multiobjective selection operator as a standard operator.
 *  \param ioDeme Deme on which selection operator is applied.
 *  \param ioContext Evolutionary context.
 */
void NSGA2Op::applyAsStandardOperator(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  // Fast non-dominated sorting, followed by insertion of the first Pareto fronts.
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::NSGA2Op",
    "Applying fast non-dominated sorting on the whole population"
  );
  NSGA2Op::Fronts lParetoFronts;
  const unsigned int lDesiredPopSize = (*mPopSize)[ioContext.getDemeIndex()];
  Individual::Bag lSortedPop(ioDeme);
  sortFastND(lParetoFronts, lDesiredPopSize, lSortedPop, ioContext);
  unsigned int lIndexDeme=0;
  for(unsigned int j=0; j<(lParetoFronts.size()-1); ++j) {
    for(unsigned int k=0; k<lParetoFronts[j].size(); ++k) {
      ioDeme[lIndexDeme++] = lSortedPop[lParetoFronts[j][k]];
    }
  }

  // Insertion of the last Pareto front, using crowding distance
  Individual::Bag lLastFrontIndiv;
  for(unsigned int l=0; l<lParetoFronts.back().size(); ++l) {
    lLastFrontIndiv.push_back(lSortedPop[lParetoFronts.back()[l]]);
  }
  NSGA2Op::Distances lDistances;
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::NSGA2Op",
    string("Computing crowding distance on the ")+uint2ordinal(lParetoFronts.size())+
    string(" Pareto front, which is made of ")+uint2str(lParetoFronts.back().size())+
    " individuals"
  );
  evalCrowdingDistance(lDistances, lLastFrontIndiv);
  for(unsigned int m=0; lIndexDeme<lDesiredPopSize; ++m) {
    ioDeme[lIndexDeme++] = lLastFrontIndiv[lDistances[m].second];
  }
  ioDeme.resize(lDesiredPopSize);
  Beagle_StackTraceEndM("void NSGA2Op::applyAsStandardOperator(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Evaluate crowding distance of a pool of individuals.
 *  \param outDistances Evaluated crowding distance and associated indexes.
 *  \param inIndividualPool Pool of individuals to evaluate distance on.
 */
void NSGA2Op::evalCrowdingDistance(NSGA2Op::Distances& outDistances,
                                   const Individual::Bag& inIndividualPool) const
{
  Beagle_StackTraceBeginM();
  outDistances.clear();
  if(inIndividualPool.size() == 0) return;
  outDistances.resize(inIndividualPool.size());
  std::vector< CrowdingPair,BEAGLE_STLALLOCATOR<CrowdingPair> > lCrowdingSet(inIndividualPool.size());
  for(unsigned int i=0; i<inIndividualPool.size(); ++i) {
    outDistances[i].first = 0.0;
    outDistances[i].second = i;
    lCrowdingSet[i].first = i;
    lCrowdingSet[i].second = castHandleT<FitnessMultiObj>(inIndividualPool[i]->getFitness());
  }
  unsigned int lNumberObjectives = lCrowdingSet.back().second->size();
  std::vector< float,BEAGLE_STLALLOCATOR<float> > lScalingFactors = 
    lCrowdingSet.back().second->getScalingFactors();
  for(unsigned int j=0; j<lNumberObjectives; ++j) {
    std::sort(lCrowdingSet.begin(), lCrowdingSet.end(), IsLessCrowdingPairPredicate(j));
    outDistances[lCrowdingSet.front().first].first = DBL_MAX;
    outDistances[lCrowdingSet.back().first].first  = DBL_MAX;
    for(unsigned int k=1; k<(lCrowdingSet.size()-1); ++k) {
      if(outDistances[lCrowdingSet[k].first].first < DBL_MAX) {
        double lNeighborDist = (*lCrowdingSet[k+1].second)[j] - (*lCrowdingSet[k-1].second)[j];
        outDistances[lCrowdingSet[k].first].first += (lNeighborDist * lScalingFactors[j]);
      }
    }
  }
  std::sort(outDistances.begin(), outDistances.end(),
            std::greater< std::pair<double,unsigned int> >());
  Beagle_StackTraceEndM("void NSGA2Op::evalCrowdingDistance(NSGA2Op::Distances& outDistances, const Individual::Bag& inIndividualPool) const");
}

/*!
 *  \brief Initialize the NSGA2 operator.
 *  \param ioSystem Reference to the system to use for the initialization.
 */
void NSGA2Op::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  ReplacementStrategyOp::initialize(ioSystem);

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

  if(ioSystem.getRegister().isRegistered(mLMRatioName)) {
    mLMRatio = castHandleT<Float>(ioSystem.getRegister().getEntry(mLMRatioName));
  } else {
    mLMRatio = new Float((float)1.0);
    Register::Description lDescription(
      "(Lambda / Mu) ratio",
      "Float",
      "1.0",
      string("(Lambda / Mu) ratio, which mean how much bigger is the child population from ") +
      string("the parent population during a (Mu+Lambda), (Mu,Lambda) or NSGA2 ")+
      string("replacement process.")
    );
    ioSystem.getRegister().addEntry(mLMRatioName, mLMRatio, lDescription);
  }
  Beagle_StackTraceEndM("void NSGA2Op::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the NSGA2 multiobjective selection operation on a deme in the given context.
 *  \param ioDeme Reference to the deme on which the operation takes place.
 *  \param ioContext Evolutionary context of the operation.
 */
void NSGA2Op::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(getRootNode()==NULL) applyAsStandardOperator(ioDeme,ioContext);
  else applyAsReplacementStrategy(ioDeme,ioContext);
  Beagle_StackTraceEndM("void NSGA2Op::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Post-initialize hook for the NSGA operator.
 *  \param ioSystem Evolutionary system.
 */
void NSGA2Op::postInit(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ec.hof.vivasize")) {
    UInt::Handle lVivaHOFSize =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.hof.vivasize"));
    if(lVivaHOFSize->getWrappedValue() > 0) {
      Beagle_LogInfoM(
        ioSystem.getLogger(),
        "replacement-strategy", "Beagle::NSGA2Op",
        string("Warning: the vivarium hall-of-fame size (parameter \"ec.hof.vivasize\") ")+
        string("is non-zero; the hall-of-fame is not meaningful in a multiobjective ")+
        string("EA context")
      );
    }
  }
  if(ioSystem.getRegister().isRegistered("ec.hof.demesize")) {
    UInt::Handle lVivaHOFSize =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.hof.demesize"));
    if(lVivaHOFSize->getWrappedValue() > 0) {
      Beagle_LogInfoM(
        ioSystem.getLogger(),
        "replacement-strategy", "Beagle::NSGA2Op",
        string("Warning: the demes hall-of-fame size (parameter \"ec.hof.demesize\") ")+
        string("is non-zero; the hall-of-fame is not meaningful in a multiobjective ")+
        string("EA context")
      );
    }
  }
  Beagle_StackTraceEndM("void NSGA2Op::postInit(System& ioSystem)");
}


/*!
 *  \brief Read a NSGA2Op operator from a XML subtree.
 *  \param inIter XML iterator to use to read NSGA2Op operator.
 *  \param inOpMap Operator map to use to read NSGA2Op operator.
 */
void NSGA2Op::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string lLMRatioReadName = inIter->getAttribute("ratio_name").c_str();
  if(lLMRatioReadName.empty() == false) mLMRatioName = lLMRatioReadName;

  ReplacementStrategyOp::readWithMap(inIter, inOpMap);
  Beagle_StackTraceEndM("void NSGA2Op::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Do fast non-dominated sort and evaluate Pareto fronts of data up to given size.
 *  \param outParetoFronts Pareto fronts from the fast ND sort.
 *  \param inSortStop Number of individuals on the Pareto fronts needed to stop the sort.
 *  \param inIndividualPool Pool of individuals to get next Pareto front of.
 *  \param ioContext Evolutionary context.
 */
void NSGA2Op::sortFastND(NSGA2Op::Fronts& outParetoFronts,
                         unsigned int inSortStop,
                         const Individual::Bag& inIndividualPool,
                         Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  outParetoFronts.clear();
  if(inSortStop == 0) return;
  outParetoFronts.resize(1);
  unsigned int lParetoSorted = 0;
  // N: Number of dominating individuals
  std::vector< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> > lN(inIndividualPool.size(), 0);
  // S: Set of dominated individuals
  Fronts lS(inIndividualPool.size());

  // First pass to get first Pareto front and domination sets
  for(unsigned int i=0; i<inIndividualPool.size(); ++i) {
    FitnessMultiObj::Handle lFitI = castHandleT<FitnessMultiObj>(inIndividualPool[i]->getFitness());
    for(unsigned int j=(i+1); j<inIndividualPool.size(); ++j) {
      FitnessMultiObj::Handle lFitJ =
        castHandleT<FitnessMultiObj>(inIndividualPool[j]->getFitness());
      if(lFitJ->isDominated(*lFitI)) {
        lS[i].push_back(j);  // Add index j to dominated set of i
        ++lN[j];             // Increment domination counter of j
      }
      else if(lFitI->isDominated(*lFitJ)) {
        lS[j].push_back(i);  // Add index i to dominated set of j
        ++lN[i];             // Increment domination counter of i
      }
    }
    if(lN[i] == 0) {         // If i is non-dominated
      outParetoFronts.back().push_back(i);
      ++lParetoSorted;
    }
  }

  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "replacement-strategy", "Beagle::NSGA2Op",
    uint2ordinal(1)+string(" Pareto front consists of")+
    uint2str(outParetoFronts.back().size())+" individuals"
  );

  // Continue ranking until individual pool is empty or stop criterion reach
  while((lParetoSorted < inIndividualPool.size()) && (lParetoSorted < inSortStop)) {
    unsigned int lIndexLastFront = outParetoFronts.size() - 1;
    outParetoFronts.resize(outParetoFronts.size() + 1);
    for(unsigned int k=0; k<outParetoFronts[lIndexLastFront].size(); ++k) {
      unsigned int lIndexPk = outParetoFronts[lIndexLastFront][k];
      for(unsigned int l=0; l<lS[lIndexPk].size(); ++l) {
        unsigned int lIndexSl = lS[lIndexPk][l];
        if(--lN[lIndexSl] == 0) {
          outParetoFronts.back().push_back(lIndexSl);
          ++lParetoSorted;
        }
      }
    }
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "replacement-strategy", "Beagle::NSGA2Op",
      uint2ordinal(outParetoFronts.size())+string(" Pareto front consists of")+
      uint2str(outParetoFronts.back().size())+" individuals"
    );
  }
  Beagle_StackTraceEndM("void NSGA2Op::sortFastND(Fronts& outParetoFronts, unsigned int inSortStop, const Individual::Bag& inIndividualPool, Context& ioContext) const");
}


/*!
 *  \brief Write MuCommaLamdaOp operator into XML streamer.
 *  \param ioStreamer XML streamer to write NSGA2Op operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void NSGA2Op::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag(getName().c_str(), inIndent);
  ioStreamer.insertAttribute("ratio_name", mLMRatioName);
  if(getRootNode() != NULL) getRootNode()->write(ioStreamer, inIndent);
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void NSGA2Op::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
