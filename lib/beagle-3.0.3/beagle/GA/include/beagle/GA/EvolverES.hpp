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
 *  \file   beagle/GA/EvolverES.hpp
 *  \brief  Definition of the class GA::EvolverES.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.1 $
 *  $Date: 2007/05/09 01:51:02 $
 */

#ifndef Beagle_GA_EvolverES_hpp
#define Beagle_GA_EvolverES_hpp

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
 *  \class EvolverES beagle/GA/EvolverES.hpp "beagle/GA/EvolverES.hpp"
 *  \brief Evolution strategy evolver class.
 *  \ingroup GAF
 *  \ingroup GAES
 */
class EvolverES : public Beagle::Evolver {

public:

  //! GA::EvolverES allocator type.
  typedef AllocatorT<EvolverES,Beagle::Evolver::Alloc>
          Alloc;
  //! GA::EvolverES handle type.
  typedef PointerT<EvolverES,Beagle::Evolver::Handle>
          Handle;
  //! GA::EvolverES bag type.
  typedef ContainerT<EvolverES,Beagle::Evolver::Bag>
          Bag;

  explicit EvolverES(unsigned int inInitSize=1);
  explicit EvolverES(EvaluationOp::Handle inEvalOp, unsigned int inInitSize=1);
  explicit EvolverES(UIntArray inInitSize);
  explicit EvolverES(EvaluationOp::Handle inEvalOp, UIntArray inInitSize);
  virtual ~EvolverES() { }

};

}
}

#endif // Beagle_GA_EvolverES_hpp
