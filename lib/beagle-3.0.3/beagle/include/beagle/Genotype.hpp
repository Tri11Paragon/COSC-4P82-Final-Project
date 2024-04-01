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
 *  \file   beagle/Genotype.hpp
 *  \brief  Definition of the type Genotype.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.1 $
 *  $Date: 2007/05/09 01:51:11 $
 */

#ifndef Beagle_Genotype_hpp
#define Beagle_Genotype_hpp

#include "XML.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"


namespace Beagle {

// Forward declaration
class Context;


/*!
 *  \class Genotype beagle/Genotype.hpp "beagle/Genotype.hpp"
 *  \brief A plain empty genotype.
 *  \ingroup ECF
 *  \ingroup Pop
 */
class Genotype : public Object {

public:

  //! Genotype allocator type.
  typedef AllocatorT<Genotype,Object::Alloc>
          Alloc;
  //! Genotype handle type.
  typedef PointerT<Genotype,Object::Handle>
          Handle;
  //! Genotype bag type.
  typedef ContainerT<Genotype,Object::Bag>
          Bag;

           Genotype() { }
  virtual ~Genotype() { }

  virtual void         readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
  virtual void         write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  virtual unsigned int getSize() const;                         // Undefined for Genotype!
  virtual void         read(PACC::XML::ConstIterator inIter);   // Undefined for Genotype!

};

}

#endif // Beagle_Genotype_hpp
