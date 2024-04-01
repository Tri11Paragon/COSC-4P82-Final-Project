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
 *  \file   CoevSymbregMain.cpp
 *  \brief  Implementation of the main routine of co-evolutionary symbolic regression.
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

using namespace std;
using namespace Beagle;

/*!
 *  \brief Main routine for the co-evolutionary symbolic regression example.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 *  \ingroup CoSym
 */
int main(int argc, char** argv) {
  try {
    // Deduce configuration filename from argv[0].
    Beagle::string lConfigFilename1 = "symgp-thread.conf";
    Beagle::string lConfigFilename2 = "trainset-thread.conf";
    Beagle::string lProgramName = argv[0];
    Beagle::string::size_type lLibsPos  = lProgramName.find(Beagle::string(".libs"));
    Beagle::string::size_type lSlashPos = lProgramName.find_last_of(Beagle::string("/\\"), lLibsPos);
    if(lSlashPos != Beagle::string::npos) {
      lConfigFilename1.insert(0, lProgramName, 0, lSlashPos+1);
      lConfigFilename2.insert(0, lProgramName, 0, lSlashPos+1);
    }
    // Creates and starts first thread/population for co-evolution.
    SymGPThread lThread1(lConfigFilename1);
    lThread1.run();
    // Creates and starts second thread/population for co-evolution.
    TrainSetThread lThread2(lConfigFilename2);
    lThread2.run();
    // Wait for the threads to finish.
    lThread1.wait();
    lThread2.wait();
  }
  catch(std::exception& inException) {
    std::cerr << "Standard exception catched:" << std::endl << std::flush;
    std::cerr << inException.what() << std::endl << std::flush;
    return 1;
  }
}
