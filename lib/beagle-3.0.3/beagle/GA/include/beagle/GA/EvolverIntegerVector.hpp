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
 *  \file   beagle/GA/EvolverIntegerVector.hpp
 *  \brief  Definition of the class GA::EvolverIntegerVector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.1.2.1 $
 *  $Date: 2007/05/09 01:51:02 $
 */
 
#ifndef Beagle_GA_EvolverIntegerVector_hpp
#define Beagle_GA_EvolverIntegerVector_hpp

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
 *  \class EvolverIntegerVector beagle/GA/EvolverIntegerVector.hpp "beagle/GA/EvolverIntegerVector.hpp"
 *  \brief Integer-valued GA evolver class.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class EvolverIntegerVector : public Beagle::Evolver {

public:

  //! GA::EvolverIntegerVector allocator type.
  typedef AllocatorT<EvolverIntegerVector,Beagle::Evolver::Alloc>
          Alloc;
  //! GA::EvolverIntegerVector handle type.
  typedef PointerT<EvolverIntegerVector,Beagle::Evolver::Handle>
          Handle;
  //! GA::EvolverIntegerVector bag type.
  typedef ContainerT<EvolverIntegerVector,Beagle::Evolver::Bag>
          Bag;

  explicit EvolverIntegerVector(unsigned int inInitSize=1);
  explicit EvolverIntegerVector(EvaluationOp::Handle inEvalOp, unsigned int inInitSize=1);
  virtual ~EvolverIntegerVector() { }

};

}
}

#endif // Beagle_GA_EvolverIntegerVector_hpp
