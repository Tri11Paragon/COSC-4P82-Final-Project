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
 *  \file   SymGPThread.cpp
 *  \brief  Implementation of the class SymGPThread.
 *  \author Jiachuan Wang <jiacwang@ecs.umass.edu>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.10.2.1 $
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
 *  \brief Construct the GP execution thread for the co-evolutionary symbolic regression problem.
 *  \param inConfigFilename Name of the configuration file to use.
 */
SymGPThread::SymGPThread(Beagle::string inConfigFilename) :
  mConfigFilename(inConfigFilename)
{ }


/*!
 *  \brief Destruct an execution thread for the SymGp problem.
 */
SymGPThread::~SymGPThread()
{
  wait();
}


/*!
 *  \brief Evolver the SymGp problem.
 */
void SymGPThread::main()
{
  try {
    // 1: Build primitives.
    GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet;
    lSet->insert(new GP::Add);
    lSet->insert(new GP::Subtract);
    lSet->insert(new GP::Multiply);
    lSet->insert(new GP::Divide);
    //lSet->insert(new GP::Sin);
    //lSet->insert(new GP::Cos);
    //lSet->insert(new GP::Exp);
    //lSet->insert(new GP::Log);
    lSet->insert(new GP::TokenT<Double>("X"));
    //lSet->insert(new GP::TokenT<Double>("Pi", Double(M_PI)));
    lSet->insert(new GP::EphemeralDouble);
    // 2: Build a system.
    GP::System::Handle lSystem = new GP::System(lSet);
    // 3: Build evaluation operator.
    SymGPEvalOp::Handle lEvalOp = new SymGPEvalOp;
    // 4: Build an evolver and a vivarium.
    GP::Evolver::Handle lEvolver = new GP::Evolver(lEvalOp);
    lEvolver->addOperator(new Coev::TermBroadcastOp);
    GP::Vivarium::Handle lVivarium =
      new GP::Vivarium(new GP::Tree::Alloc, new FitnessSimpleMin::Alloc);
    // 5: Initialize and evolve the vivarium.
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
