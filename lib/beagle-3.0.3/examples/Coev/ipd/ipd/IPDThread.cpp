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
 *  \file   IPDThread.cpp
 *  \brief  Implementation of the class IPDThread.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Coev.hpp"
#include "IPDThread.hpp"
#include "IPDEvalOp.hpp"

using namespace Beagle;


/*!
 *  \brief Construct an execution thread for the IPD problem.
 *  \param inConfigFilename Name of the configuration file to use.
 */
IPDThread::IPDThread(Beagle::string inConfigFilename) :
  mConfigFilename(inConfigFilename)
{ }


/*!
 *  \brief Destruct an execution thread for the IPD problem.
 */
IPDThread::~IPDThread()
{
  wait();
}


/*!
 *  \brief Evolver the IPD problem.
 */
void IPDThread::main()
{
  try {
    // 1. Set encoding, that is a bitstring of 3 bits
    const unsigned int lNumberOfBits=3;
    // 2. Build the system.
    System::Handle lSystem = new System;
    // 3. Build evaluation operator.
    IPDEvalOp::Handle lEvalOp = new IPDEvalOp;
    // 4. Instanciate the vivarium with a bitstring allocator.
    GA::BitString::Alloc::Handle lBSAlloc = new GA::BitString::Alloc;
    Vivarium::Handle lVivarium = new Vivarium(lBSAlloc);
    // 5. Initialize the evolver by reading configuration file.
    GA::EvolverBitString::Handle lEvolver = new GA::EvolverBitString(lEvalOp, lNumberOfBits);
    lEvolver->addOperator(new Coev::TermBroadcastOp);
    lEvolver->initialize(lSystem, mConfigFilename);
    // 6. Evolve vivarium.
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
