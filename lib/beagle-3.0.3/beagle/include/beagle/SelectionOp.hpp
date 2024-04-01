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
 *  \file   beagle/SelectionOp.hpp
 *  \brief  Definition of the class SelectionOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:17 $
 */

#ifndef Beagle_SelectionOp_hpp
#define Beagle_SelectionOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/BreederOp.hpp"
#include "beagle/Float.hpp"


namespace Beagle {

/*!
 *  \class SelectionOp beagle/SelectionOp.hpp "beagle/SelectionOp.hpp"
 *  \brief Abstract selection operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class SelectionOp : public BreederOp {

public:

  //! SelectionOp allocator type.
  typedef AbstractAllocT<SelectionOp,BreederOp::Alloc>
          Alloc;
  //! SelectionOp handle type.
  typedef PointerT<SelectionOp,BreederOp::Handle>
          Handle;
  //! SelectionOp bag type.
  typedef ContainerT<SelectionOp,BreederOp::Bag>
          Bag;

  explicit SelectionOp(string inReproProbaName="ec.repro.prob",
                       string inName="SelectionOp");
  virtual ~SelectionOp() { }

  /*!
   *  \brief Select an individual in a pool.
   *  \param ioPool Pool in which the individual is choosen.
   *  \param ioContext Evolutionary context.
   *  \return Index of the choosen individual in the pool.
   */
  virtual unsigned int selectIndividual(Individual::Bag& ioPool, Context& ioContext) = 0;

  virtual Individual::Handle breed(Individual::Bag& inBreedingPool,
                                   BreederNode::Handle inChild,
                                   Context& ioContext);
  virtual float              getBreedingProba(BreederNode::Handle inChild);
  virtual void               initialize(System& ioSystem);
  virtual void               operate(Deme& ioDeme, Context& ioContext);
  virtual void               readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);
  virtual void               writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

  Float::Handle mReproductionProba;    //!< Reproduction probability.
  string   mReproProbaName;       //!< Reproduction probability name.

};

}

#endif // Beagle_SelectionOp_hpp
