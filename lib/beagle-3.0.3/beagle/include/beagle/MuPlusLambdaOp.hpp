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
 *  \file   beagle/MuPlusLambdaOp.hpp
 *  \brief  Definition of the class MuPlusLambdaOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:15 $
 */

 
#ifndef Beagle_MuPlusLambdaOp_hpp
#define Beagle_MuPlusLambdaOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/ReplacementStrategyOp.hpp"
#include "beagle/Float.hpp"

namespace Beagle {
  

/*!
 *  \class MuPlusLambdaOp beagle/MuPlusLambdaOp.hpp "beagle/MuPlusLambdaOp.hpp"
 *  \brief Mu plus lambda (Mu+Lambda) replacement strategy operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *
 *  A (Mu+Lambda) operator generates Lambda children individuals from a population of Mu parents
 *  (usually where Lambda > Mu). From the Mu parents and the Lambda individual, it keeps the Mu best 
 *  individuals to constitute the new generation.
 */
class MuPlusLambdaOp : public ReplacementStrategyOp {

public:

  //! MuPlusLambdaOp allocator type.
  typedef AllocatorT<MuPlusLambdaOp,ReplacementStrategyOp::Alloc>
          Alloc;
  //! MuPlusLambdaOp handle type.
  typedef PointerT<MuPlusLambdaOp,ReplacementStrategyOp::Handle>
          Handle;
  //! MuPlusLambdaOp bag type.
  typedef ContainerT<MuPlusLambdaOp,ReplacementStrategyOp::Bag>
          Bag;

  explicit MuPlusLambdaOp(string inLMRatioName="ec.mulambda.ratio",
                          string inName="MuPlusLambdaOp");
  virtual ~MuPlusLambdaOp() { }

  virtual void initialize(System& ioSystem);
  virtual void operate(Deme& ioDeme, Context& ioContext);
  virtual void readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

  Float::Handle mLMRatio;              //!< (Lambda / Mu) ratio to use for replacement operator.
  string   mLMRatioName;          //!< (Lambda / Mu) ratio parameter name used in register.

};

}


#endif // Beagle_MuPlusLambdaOp_hpp

