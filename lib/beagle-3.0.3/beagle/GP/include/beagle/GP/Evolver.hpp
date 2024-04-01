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
 *  \file   beagle/GP/Evolver.hpp
 *  \brief  Definition of the class GP::Evolver.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:04 $
 */

/*!
 *  \defgroup GPOp GP Operators and Evolvers
 *  \ingroup GPF
 *  \brief Genetic programming operators and evolvers, part of the GP framework.
 */
 
#ifndef Beagle_GP_Evolver_hpp
#define Beagle_GP_Evolver_hpp

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
#include "beagle/GP/EvaluationOp.hpp"
#include "beagle/TerminationOp.hpp"

namespace Beagle {
namespace GP {

/*!
 *  \class Evolver beagle/GP/Evolver.hpp "beagle/GP/Evolver.hpp"
 *  \brief Standard GP Generational evolver class.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
class Evolver : public Beagle::Evolver {

public:

  //! GP::Evolver allocator type.
  typedef AllocatorT<Evolver,Beagle::Evolver::Alloc>
          Alloc;
  //! GP::Evolver handle type.
  typedef PointerT<Evolver,Beagle::Evolver::Handle>
          Handle;
  //! GP::Evolver bag type.
  typedef ContainerT<Evolver,Beagle::Evolver::Bag>
          Bag;

           Evolver();
  explicit Evolver(Beagle::EvaluationOp::Handle inEvalOp);
  virtual ~Evolver() { }

protected:

  virtual void addBasicOperators();

};

}
}

#endif // Beagle_GP_Evolver_hpp
