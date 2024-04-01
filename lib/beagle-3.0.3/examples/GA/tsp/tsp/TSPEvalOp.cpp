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
 *  \file   TSPEvalOp.cpp
 *  \brief  Implementation of the class TSPEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.5.2.1 $
 *  $Date: 2007/05/09 01:51:23 $
 */

#include "beagle/GA.hpp"
#include "TSPEvalOp.hpp"

#include <cmath>

#define BEAGLE_TSP_PROBLEMSIZE 25


using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator for the TSP problem.
 */
TSPEvalOp::TSPEvalOp() :
  EvaluationOp("TSPEvalOp")
{ }


/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the individual.
 */
Fitness::Handle TSPEvalOp::evaluate(Individual& inIndividual, Context& ioContext)
{
  Beagle_AssertM(inIndividual.size() == 1);
  GA::IntegerVector::Handle lPath = castHandleT<GA::IntegerVector>(inIndividual[0]);
  const unsigned int lRootIndex = mDistancesMatrix->getRows()-1;
  double lTripDistance = (*mDistancesMatrix)(lRootIndex,(*lPath)[0]);
  for(unsigned int i=1; i<lPath->size(); ++i) {
    lTripDistance += (*mDistancesMatrix)((*lPath)[i-1],(*lPath)[i]);
  }
  lTripDistance += (*mDistancesMatrix)((*lPath)[lPath->size()-1],lRootIndex);
  return new FitnessSimpleMin(float(lTripDistance));
}


/*!
 *  \brief Initialize the TSP evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void TSPEvalOp::initialize(Beagle::System& ioSystem)
{
  Beagle::EvaluationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("ts.graph.distances")) {
    mDistancesMatrix = castHandleT<Matrix>(ioSystem.getRegister().getEntry("ts.graph.distances"));
  } else {
    mDistancesMatrix = new Matrix(0,0);
    Beagle::string lLongDescrip("Distances between graph's nodes (the towns by analogy). ");
    lLongDescrip += "If the matrix value is not specified, it will be randomly generated ";
    lLongDescrip += "at the initialization time.";
    Register::Description lDescription(
      "TSP distances matrix",
      "Matrix",
      "",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ts.graph.distances", mDistancesMatrix, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ga.init.vectorsize")) {
    mIntVectorSize = castHandleT<UInt>(ioSystem.getRegister()["ga.init.vectorsize"]);
  } else {
    mIntVectorSize = new UInt(0);
    Register::Description lDescription(
      "Initial integer vectors sizes",
      "UInt",
      "0",
      "Integer vector size of initialized individuals."
    );
    ioSystem.getRegister().addEntry("ga.init.vectorsize", mIntVectorSize, lDescription);
  }
}


/*!
 *  \brief Post-initialize the TSP evaluation operator.
 *  \param ioSystem Evolutionary system.
 */
void TSPEvalOp::postInit(Beagle::System& ioSystem)
{
  Beagle::EvaluationOp::postInit(ioSystem);

  if((mDistancesMatrix->getRows()==0) || (mDistancesMatrix->getCols()==0)) {
    // Generating randomly the TSP graph to solve.
    std::vector< std::pair<double,double> > lNodePosition(BEAGLE_TSP_PROBLEMSIZE);
    for(unsigned int i=0; i<BEAGLE_TSP_PROBLEMSIZE; ++i) {
      lNodePosition[i].first  = ioSystem.getRandomizer().rollUniform(0.0, 10.0);
      lNodePosition[i].second = ioSystem.getRandomizer().rollUniform(0.0, 10.0);
    }
    // Computing distance (simply Euclidean distances) between the nodes.
    mDistancesMatrix->resize(BEAGLE_TSP_PROBLEMSIZE,BEAGLE_TSP_PROBLEMSIZE);
    for(unsigned int i=0; i<BEAGLE_TSP_PROBLEMSIZE; ++i) {
      for(unsigned int j=(i+1); j<BEAGLE_TSP_PROBLEMSIZE; ++j) {
        double lDistance = ((lNodePosition[j].first-lNodePosition[i].first) *
                            (lNodePosition[j].first-lNodePosition[i].first)) +
                           ((lNodePosition[j].second-lNodePosition[i].second) *
                            (lNodePosition[j].second-lNodePosition[i].second));
        lDistance = std::sqrt(lDistance);
        lDistance += ioSystem.getRandomizer().rollGaussian(0.0, 0.5);       // Add some noise
        (*mDistancesMatrix)(i,j) = (*mDistancesMatrix)(j,i) = lDistance;
      }
    }
    for(unsigned int i=0; i<BEAGLE_TSP_PROBLEMSIZE; ++i) (*mDistancesMatrix)(i,i) = 0.0;
  }
  else if(mDistancesMatrix->getRows() != mDistancesMatrix->getCols()) {
    std::ostringstream lOSS;
    lOSS << "Distances matrix of TSP evaluation operator (registered as ";
    lOSS << "'ts.graph.distances' parameter) must be square!";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }

  mIntVectorSize->getWrappedValue() = (mDistancesMatrix->getRows()-1);

  Beagle_LogInfoM(
    ioSystem.getLogger(),
    "evaluation", "TSPEvalOp",
    Beagle::string("Distances matrix used in TSP problem: ")+mDistancesMatrix->serialize()
  );
}


