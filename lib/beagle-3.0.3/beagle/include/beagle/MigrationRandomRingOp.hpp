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
 *  \file   beagle/MigrationRandomRingOp.hpp
 *  \brief  Definition of the class MigrationRandomRingOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.1 $
 *  $Date: 2007/05/09 01:51:15 $
 */

#ifndef Beagle_MigrationRandomRingOp_hpp
#define Beagle_MigrationRandomRingOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/MigrationOp.hpp"
#include "beagle/Individual.hpp"
#include "beagle/IndividualAlloc.hpp"

namespace Beagle {

/*!
 *  \class MigrationRandomRingOp beagle/MigrationRandomRingOp.hpp "beagle/MigrationRandomRingOp.hpp"
 *  \brief Migrate individuals choosen randomly, following a ring topology, operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class MigrationRandomRingOp : public MigrationOp {

public:

  //! MigrationRandomRingOp allocator type
  typedef AllocatorT<MigrationRandomRingOp,MigrationOp::Alloc>
          Alloc;
  //! MigrationRandomRingOp handle type.
  typedef PointerT<MigrationRandomRingOp,MigrationOp::Handle>
          Handle;
  //! MigrationRandomRingOp bag type.
  typedef ContainerT<MigrationRandomRingOp,MigrationOp::Bag>
          Bag;

  explicit MigrationRandomRingOp(string inName="MigrationRandomRingOp");
  virtual ~MigrationRandomRingOp() { }

  virtual void migrate(Deme& ioDeme, Context& ioContext, unsigned int inNumberMigrants);

};

}

#endif // Beagle_MigrationRandomRingOp_hpp
