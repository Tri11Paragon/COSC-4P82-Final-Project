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
 *  \file   ParityMain.cpp
 *  \brief  Implementation of the main routine for the even-6 parity problem.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.2 $
 *  $Date: 2007/05/11 20:04:33 $
 */

#include "beagle/GP.hpp"
#include "ParityEvalOp.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>

using namespace std;
using namespace Beagle;

/*!
 *  \brief Main routine for the even-6 parity problem.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 *  \ingroup Parity
 */
int main(int argc, char *argv[]) {
  try {
    // 1: Build primitives.
    GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet;
    lSet->insert(new GP::And);
    lSet->insert(new GP::Or);
    lSet->insert(new GP::Nand);
    lSet->insert(new GP::Nor);
    for (unsigned int i=0; i<ParityFanInM; i++) {
      lSet->insert(new GP::TokenT<Bool>(std::string("IN")+uint2str(i)));
    }
    // 2: Build a system.
    GP::System::Handle lSystem = new GP::System(lSet);
    // 3: Build operators.
    EvaluationOp::Handle lEvalOp = new ParityEvalOp;
    // 4: Build an evolver and a vivarium.
    Evolver::Handle lEvolver = new GP::Evolver(lEvalOp);
    GP::Vivarium::Handle lVivarium = new GP::Vivarium;
    // 5: Initialize and evolve the vivarium.
    lEvolver->initialize(lSystem, argc, argv);
    lEvolver->evolve(lVivarium);
  }
  catch(Exception& inException) {
    inException.terminate();
  }
  catch(exception& inException) {
    cerr << "Standard exception catched:" << endl;
    cerr << inException.what() << endl << flush;
    return 1;
  }
  catch(...) {
    cerr << "Unknown exception catched!" << endl << flush;
    return 1;
  }
  return 0;
}

