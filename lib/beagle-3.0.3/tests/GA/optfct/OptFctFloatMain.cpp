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
 *  \file   OptFctFloatMain.cpp
 *  \brief  Main routine for the real-valued function optimization problems.
 *  \author Christian Gagne
 *  $Revision: 1.3.2.1 $
 *  $Date: 2007/05/11 19:13:09 $
 */

#include "Functions.hpp"
#include "OptFctFloatEvalOpT.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Main routine for the real-valued function optimization problems.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 */
int main(int argc, char** argv) {
  try {
    // 1. Build the system.
    System::Handle lSystem = new System;
    // 2. Build the different evaluation operators.
    OptFctFloatEvalOpT<SphereFunct>::Handle lSphereEvalOp =
      new OptFctFloatEvalOpT<SphereFunct>("SphereEvalOp");
    OptFctFloatEvalOpT<SchwefelFunct>::Handle lSchwefelEvalOp =
      new OptFctFloatEvalOpT<SchwefelFunct>("SchwefelEvalOp");
    OptFctFloatEvalOpT<F3Funct>::Handle lF3EvalOp =
      new OptFctFloatEvalOpT<F3Funct>("F3EvalOp");
    OptFctFloatEvalOpT<RosenbrockFunct>::Handle lRosenbrockEvalOp =
      new OptFctFloatEvalOpT<RosenbrockFunct>("RosenbrockEvalOp");
    OptFctFloatEvalOpT<RastriginFunct>::Handle lRastriginEvalOp =
      new OptFctFloatEvalOpT<RastriginFunct>("RastriginEvalOp");
    // 3. Instanciate the evolver and the vivarium for float vectors GA population.
    GA::FloatVector::Alloc::Handle lFVAlloc = new GA::FloatVector::Alloc;
    FitnessSimpleMin::Alloc::Handle lFitAlloc = new FitnessSimpleMin::Alloc;
    Vivarium::Handle lVivarium = new Vivarium(lFVAlloc, lFitAlloc);
    // 4. Initialize the evolver and evolve the vivarium.
    GA::EvolverFloatVector::Handle lEvolver = new GA::EvolverFloatVector(lSphereEvalOp, 5);
    lEvolver->addOperator(lSchwefelEvalOp);
    lEvolver->addOperator(lF3EvalOp);
    lEvolver->addOperator(lRosenbrockEvalOp);
    lEvolver->addOperator(lRastriginEvalOp);
    lEvolver->initialize(lSystem, argc, argv);
    lEvolver->evolve(lVivarium);
    // 5. Log best-of-run solution.
    Beagle_LogObjectM(
      lSystem->getLogger(),
      Beagle::Logger::eBasic,
      "main",
      "main",
      lVivarium->getHallOfFame()
    );
  }
  catch(Exception& inException) {
    inException.terminate(cerr);
  }
  catch(std::exception& inException) {
    cerr << "Standard exception catched:" << endl << flush;
    cerr << inException.what() << endl << flush;
    return 1;
  }
  return 0;
}
