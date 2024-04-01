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
 *  \file   beagle/GA/EvolverFloatVector.hpp
 *  \brief  Definition of the class GA::EvolverFloatVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.3 $
 *  $Date: 2007/09/10 18:24:10 $
 */
 
#ifndef Beagle_GA_EvolverFloatVector_hpp
#define Beagle_GA_EvolverFloatVector_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Evolver.hpp"
#include "beagle/EvaluationOp.hpp"
#include "beagle/TerminationOp.hpp"
#include "beagle/UInt.hpp"

namespace Beagle {
namespace GA {

/*!
 *  \class EvolverFloatVector beagle/GA/EvolverFloatVector.hpp "beagle/GA/EvolverFloatVector.hpp"
 *  \brief Real-valued GA evolver class.
 *  \ingroup GAF
 *  \ingroup GAFV
 */
class EvolverFloatVector : public Beagle::Evolver {

public:

  //! GA::EvolverFloatVector allocator type.
  typedef AllocatorT<EvolverFloatVector,Beagle::Evolver::Alloc>
          Alloc;
  //! GA::EvolverFloatVector handle type.
  typedef PointerT<EvolverFloatVector,Beagle::Evolver::Handle>
          Handle;
  //! GA::EvolverFloatVector bag type.
  typedef ContainerT<EvolverFloatVector,Beagle::Evolver::Bag>
          Bag;

  explicit EvolverFloatVector(unsigned int inInitSize=1);
  explicit EvolverFloatVector(EvaluationOp::Handle inEvalOp, unsigned int inInitSize=1);
  explicit EvolverFloatVector(UIntArray inInitSize);
  explicit EvolverFloatVector(EvaluationOp::Handle inEvalOp, UIntArray inInitSize);
  virtual ~EvolverFloatVector() { }

  virtual void initialize(System::Handle ioSystem, int ioArgc, char** ioArgv);
  virtual void initialize(System::Handle ioSystem, std::string inConfigFilename);

};

}
}

#endif // Beagle_GA_EvolverFloatVector_hpp
