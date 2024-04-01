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
 *  \file   beagle/LinGP/InstructionSuperSet.hpp
 *  \brief  Definition of the type LinGP::InstructionSuperSet.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */


#ifndef Beagle_LinGP_InstructionSuperSet_hpp
#define Beagle_LinGP_InstructionSuperSet_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/LinGP/InstructionSet.hpp"

namespace Beagle {
namespace LinGP {


// Forward declaration
class System;


/*!
 *  \class InstructionSuperSet beagle/LinGP/InstructionSuperSet.hpp
 *    "beagle/LinGP/InstructionSuperSet.hpp"
 *  \brief Linear GP instruction super set class.
 *  \ingroup LinGPF
 *  \ingroup Instructions
 *  \ingroup LinGPSys
 */
class InstructionSuperSet : public InstructionSet::Bag {

public:

  //! LinGP::InstructionSuperSet allocator type.
  typedef AllocatorT<InstructionSuperSet,InstructionSet::Bag::Alloc>
          Alloc;
  //! LinGP::InstructionSuperSet handle type.
  typedef PointerT<InstructionSuperSet,InstructionSet::Bag::Handle>
          Handle;
  //! LinGP::InstructionSuperSet bag type.
  typedef ContainerT<InstructionSuperSet,InstructionSet::Bag::Bag>
          Bag;

           InstructionSuperSet() { }
  virtual ~InstructionSuperSet() { }

  virtual void initialize(LinGP::System& ioSystem);
  virtual void insert(InstructionSet::Handle inInstructionSet);
  virtual void read(PACC::XML::ConstIterator inIter);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}
}

#endif // Beagle_LinGP_InstructionSuperSet_hpp
