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
 *  \file   MaxFctFloatMain.cpp
 *  \brief  Implementation of the main routine for the maximisation problem.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:23 $
 */

#include "beagle/GA.hpp"
#include "MaxFctFloatEvalOp.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

using namespace std;
using namespace Beagle;

/*!
 *  \brief Main routine for the function maximisation problem.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 *  \ingroup MaxFctFloat
 */
int main(int argc, char** argv) {
  try {
    // 1. Build the system.
    System::Handle lSystem = new System;
    // 2. Build evaluation operator.
    MaxFctFloatEvalOp::Handle lEvalOp = new MaxFctFloatEvalOp;
    // 3. Instanciate the evolver and the vivarium for float vectors GA population.
    GA::FloatVector::Alloc::Handle lFVAlloc = new GA::FloatVector::Alloc;
    Vivarium::Handle lVivarium = new Vivarium(lFVAlloc);
    // 4. Set representation, float vectors of 5 values.
    const unsigned int lVectorSize=5;  
    // 5. Initialize the evolver and evolve the vivarium.
    GA::EvolverFloatVector::Handle lEvolver = new GA::EvolverFloatVector(lEvalOp, lVectorSize);
    lEvolver->initialize(lSystem, argc, argv);
    lEvolver->evolve(lVivarium);
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
