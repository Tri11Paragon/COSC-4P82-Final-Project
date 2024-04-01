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
 *  \file   beagle/Map.hpp
 *  \brief  Definition of class Map.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.1 $
 *  $Date: 2007/05/09 01:51:15 $
 */

#ifndef Beagle_Map_hpp
#define Beagle_Map_hpp

#include <string>
#include <map>

#include "XML.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"

namespace Beagle {


/*!
 *  \class Map beagle/Map.hpp "beagle/Map.hpp"
 *  \brief Associative container using a string as key accessors to the
 *    Object::Handle instances.
 *  \ingroup OOF
 *  \ingroup DataStruct
 */
class Map : public Object,
            public std::map< string,Object::Handle,std::less<string>,
                             BEAGLE_STLALLOCATOR< std::pair<const string,Object::Handle> > > {

public:

  //! Map allocator type.
  typedef AllocatorT<Map,Object::Alloc>
          Alloc;
  //! Map handle type.
  typedef PointerT<Map,Object::Handle>
          Handle;
  //! Map bag type.
  typedef ContainerT<Map,Object::Bag>
          Bag;

           Map() { }
  virtual ~Map() { }

  virtual bool isEqual(const Object& inRightObj) const;
  virtual bool isLess(const Object& inRightObj) const;
  virtual void read(PACC::XML::ConstIterator inIter);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

#endif // Beagle_Map_hpp

