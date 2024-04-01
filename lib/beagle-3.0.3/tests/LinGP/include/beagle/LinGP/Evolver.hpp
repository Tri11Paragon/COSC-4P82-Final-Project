/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/Evolver.hpp
 *  \brief  Definition of the class LinGP::Evolver.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */

/*!
 *  \defgroup LinGPOp Linear GP Operators and Evolvers
 *  \ingroup LinGPF
 *  \brief Genetic programming operators and evolvers, part of the linear GP framework.
 */
 
#ifndef Beagle_LinGP_Evolver_hpp
#define Beagle_LinGP_Evolver_hpp

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
#include "beagle/LinGP/EvaluationOp.hpp"

namespace Beagle {
namespace LinGP {

/*!
 *  \class Evolver beagle/LinGP/Evolver.hpp "beagle/LinGP/Evolver.hpp"
 *  \brief Standard linear GP generational evolver class.
 *  \ingroup LinGPF
 *  \ingroup LinGPOp
 */
class Evolver : public Beagle::Evolver {

public:

  //! LinGP::Evolver allocator type.
  typedef AllocatorT<Evolver,Beagle::Evolver::Alloc>
          Alloc;
  //! LinGP::Evolver handle type.
  typedef PointerT<Evolver,Beagle::Evolver::Handle>
          Handle;
  //! LinGP::Evolver bag type.
  typedef ContainerT<Evolver,Beagle::Evolver::Bag>
          Bag;

           Evolver();
  explicit Evolver(Beagle::EvaluationOp::Handle inEvalOp);
  virtual ~Evolver() { }

};

}
}

#endif // Beagle_LinGP_Evolver_hpp
