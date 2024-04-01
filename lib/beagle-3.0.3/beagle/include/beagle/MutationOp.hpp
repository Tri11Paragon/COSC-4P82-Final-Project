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
 *  \file   beagle/MutationOp.hpp
 *  \brief  Definition of the class MutationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:15 $
 */

#ifndef Beagle_MutationOp_hpp
#define Beagle_MutationOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Float.hpp"
#include "beagle/Individual.hpp"
#include "beagle/BreederOp.hpp"

namespace Beagle {

/*!
 *  \class MutationOp beagle/MutationOp.hpp "beagle/MutationOp.hpp"
 *  \brief Abstract mutation operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class MutationOp : public BreederOp {

public:

  //! MutationOp allocator type.
  typedef AbstractAllocT<MutationOp,BreederOp::Alloc>
          Alloc;
  //! MutationOp handle type.
  typedef PointerT<MutationOp,BreederOp::Handle>
          Handle;
  //! MutationOp bag type.
  typedef ContainerT<MutationOp,BreederOp::Bag>
          Bag;

  explicit MutationOp(string inMutationPbName="ec.mut.prob",
                      string inName="MutationOp");
  virtual ~MutationOp() { }

  /*!
   *  \brief Mutate an individual.
   *  \param ioIndividual Individual to mutate.
   *  \param ioContext Context of the evolution.
   *  \return True if the individual is effectively mutated, false if not.
   */
  virtual bool mutate(Individual& ioIndividual, Context& ioContext) = 0;

  virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
                                   BreederNode::Handle inChild,
                                   Context& ioContext);
  virtual float              getBreedingProba(BreederNode::Handle inChild);
  virtual void               initialize(System& ioSystem);
  virtual void               operate(Deme& ioDeme, Context& ioContext);
  virtual void               readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);
  virtual void               writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:
  Float::Handle mMutationProba;   //!< Individual mutation probability.
  string   mMutationPbName;  //!< Mutation probability parameter name.

};

}

#endif // Beagle_MutationOp_hpp
