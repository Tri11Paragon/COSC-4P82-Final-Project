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
 *  \file   beagle/MilestoneReadOp.hpp
 *  \brief  Definition of the class MilestoneReadOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:15 $
 */

#ifndef Beagle_MilestoneReadOp_hpp
#define Beagle_MilestoneReadOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/String.hpp"
#include "beagle/Vivarium.hpp"

namespace Beagle {

/*!
 *  \class MilestoneReadOp beagle/MilestoneReadOp.hpp "beagle/MilestoneReadOp.hpp"
 *  \brief Read a milestone operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class MilestoneReadOp : public Operator {

public:

  //! MilestoneReadOp allocator type
  typedef AllocatorT<MilestoneReadOp,Operator::Alloc>
          Alloc;
  //! MilestoneReadOp handle type.
  typedef PointerT<MilestoneReadOp,Operator::Handle>
          Handle;
  //! MilestoneReadOp bag type.
  typedef ContainerT<MilestoneReadOp,Operator::Bag>
          Bag;

  enum ParseCommands { 
    eParseMilestone = 1,
    eParseSystem    = 2,
    eParseEvolver   = 4,
    eParseRegister  = 8,
    eParseVivarium  = 16,
    eParseAll = 255 
  };

  explicit MilestoneReadOp(string inName="MilestoneReadOp");
  virtual ~MilestoneReadOp() { }

  virtual void initialize(System& ioSystem);
  virtual void operate(Deme& ioDeme, Context& ioContext);
  virtual void readMilestone(string inFilename, Context& ioContext, int inCommands=eParseAll);

private:
  String::Handle mMilestoneName;  //!< Name of the milestone to read.

};

}

#endif // Beagle_MilestoneReadOp_hpp
