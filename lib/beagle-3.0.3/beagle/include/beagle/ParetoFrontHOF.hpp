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
 *  \file   beagle/ParetoFrontHOF.hpp
 *  \brief  Definition of the class ParetoFrontHOF.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:16 $
 */

#ifndef Beagle_ParetoFrontHOF_hpp
#define Beagle_ParetoFrontHOF_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Individual.hpp"
#include "beagle/Logger.hpp"
#include "beagle/HallOfFame.hpp"
#include "beagle/Deme.hpp"

namespace Beagle {


/*!
 *  \class ParetoFrontHOF beagle/ParetoFrontHOF.hpp "beagle/ParetoFrontHOF.hpp"
 *  \brief A Pareto front hall-of-fame for multiobjective optimization.
 *  \warning It may be costfull to use this hall-of-fame during an evolution. 
 *    It may be preferable to evaluate the Pareto front at the end of an evolution,
 *    with operator ParetoFrontCalculateOp.
 *  \ingroup ECF
 *  \ingroup Pop
 *  \ingroup MultiObj
 */
class ParetoFrontHOF : public HallOfFame {

public:

  //! ParetoFrontHOF allocator type.
  typedef AllocatorT<ParetoFrontHOF,HallOfFame::Alloc>
          Alloc;
  //! ParetoFrontHOF handle type.
  typedef PointerT<ParetoFrontHOF,HallOfFame::Handle>
          Handle;
  //! ParetoFrontHOF bag type.
  typedef ContainerT<ParetoFrontHOF,HallOfFame::Bag>
          Bag;

  explicit ParetoFrontHOF(Individual::Alloc::Handle inIndivAlloc=NULL);
  virtual ~ParetoFrontHOF() { }

  virtual bool updateWithDeme(unsigned int inSizeHOF, const Deme& inDeme, Context& ioContext);
  virtual bool updateWithIndividual(unsigned int inSizeHOF, const Individual& inIndividual,
                                    Context& ioContext);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}


#endif // Beagle_ParetoFrontHOF_hpp

