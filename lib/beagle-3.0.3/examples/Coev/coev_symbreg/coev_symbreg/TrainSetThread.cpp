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
 *  \file   TrainSetThread.cpp
 *  \brief  Implementation of the class TrainSetThread.
 *  \author Jiachuan Wang <jiacwang@ecs.umass.edu>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.11.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Coev.hpp"
#include "CoSymEvalOp.hpp"
#include "TrainSetEvalOp.hpp"
#include "TrainSetThread.hpp"
#include "SymGPEvalOp.hpp"
#include "SymGPThread.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

using namespace Beagle;

/*!
 *  \brief Construct an training set execution thread for the
 *    co-evolutionary symbolic regression.
 *  \param inConfigFilename Name of the configuration file to use.
 */
TrainSetThread::TrainSetThread(Beagle::string inConfigFilename) :
  mConfigFilename(inConfigFilename)
{ }


/*!
 *  \brief Destruct an execution thread for the TrainSet problem.
 */
TrainSetThread::~TrainSetThread()
{
  wait();
}


/*!
 *  \brief Evolver the TrainSet problem.
 */
void TrainSetThread::main()
{
  try {
    // 1. Build the system.
    System::Handle lSystem = new System;
    // 2. Build evaluation operator.
    TrainSetEvalOp::Handle lEvalOp = new TrainSetEvalOp;
    // 3. Instanciate the evolver and the vivarium for float vectors GA population.
    GA::FloatVector::Alloc::Handle lFVAlloc = new GA::FloatVector::Alloc;
    Vivarium::Handle lVivarium = new Vivarium(lFVAlloc);
    // 4. Set representation, individuals of 1 float vector, each with 20 float values.
    const unsigned int lVectorSize=20;
    // 5. Initialize the evolver and evolve the vivarium.
    GA::EvolverFloatVector::Handle lEvolver = new GA::EvolverFloatVector(lEvalOp, lVectorSize);
    lEvolver->addOperator(new Coev::TermBroadcastOp);
    lEvolver->initialize(lSystem, mConfigFilename);
    lEvolver->evolve(lVivarium);
  }
  catch(Exception& inException) {
    inException.terminate(std::cerr);
  }
  catch(std::exception& inException) {
    std::cerr << "Standard exception catched:" << std::endl << std::flush;
    std::cerr << inException.what() << std::endl << std::flush;
  }
}
