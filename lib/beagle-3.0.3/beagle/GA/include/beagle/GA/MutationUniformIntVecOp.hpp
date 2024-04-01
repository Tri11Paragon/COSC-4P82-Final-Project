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
 *  \file   beagle/GA/MutationUniformIntVecOp.hpp
 *  \brief  Definition of the class GA::MutationUniformIntVecOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.3.2.1 $
 *  $Date: 2007/05/09 01:51:02 $
 */

#ifndef Beagle_GA_MutationUniformIntVecOp_hpp
#define Beagle_GA_MutationUniformIntVecOp_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"


namespace Beagle {
namespace GA {

/*!
 *  \class MutationUniformIntVecOp beagle/GA/MutationUniformIntVecOp.hpp "beagle/GA/MutationUniformIntVecOp.hpp"
 *  \brief Integer vector GA uniform mutation operator class.
 *  \ingroup GAF
 *  \ingroup GAIV
 */
class MutationUniformIntVecOp : public Beagle::MutationOp {

public:

  //! GA::MutationUniformIntVecOp allocator type.
  typedef AllocatorT<MutationUniformIntVecOp,Beagle::MutationOp::Alloc>
          Alloc;
  //! GA::MutationUniformIntVecOp handle type.
  typedef PointerT<MutationUniformIntVecOp,Beagle::MutationOp::Handle>
          Handle;
  //! GA::MutationUniformIntVecOp bag type.
  typedef ContainerT<MutationUniformIntVecOp,Beagle::MutationOp::Bag>
          Bag;

  explicit MutationUniformIntVecOp(string inMutationPbName="ga.mutunif.indpb",
                                   string inIntMutatePbName="ga.mutunif.intpb",
                                   string inName="GA-MutationUniformIntVecOp");
  virtual ~MutationUniformIntVecOp() { }

  virtual void initialize(System& ioSystem);
  virtual bool mutate(Beagle::Individual& ioIndividual, Context& ioContext);
  virtual void readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);
  virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

  IntArray::Handle mMaxValue;         //!< Integers maximum value.
  IntArray::Handle mMinValue;         //!< Integers minimum value.
  Float::Handle    mIntMutateProba;   //!< Single integer mutation probability.
  string           mIntMutatePbName;  //!< Single integer mutation probability parameter name.

};

}
}

#endif // Beagle_GA_MutationUniformIntVecOp_hpp
