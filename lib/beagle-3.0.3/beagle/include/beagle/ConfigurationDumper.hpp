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
 *  \file   beagle/ConfigurationDumper.hpp
 *  \brief  Definition of the class ConfigurationDumper.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:09 $
 */

#ifndef Beagle_ConfigurationDumper_hpp
#define Beagle_ConfigurationDumper_hpp

#include "XML.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/String.hpp"


namespace Beagle {

// Forward declaration
class System;
class Evolver;

/*!
 *  \class ConfigurationDumper beagle/ConfigurationDumper.hpp "beagle/ConfigurationDumper.hpp"
 *  \brief Configuration dumper class parameter.
 */
class ConfigurationDumper : public String {

public:

  //! ConfigurationDumper allocator.
  typedef AllocatorT<ConfigurationDumper,String::Alloc>
          Alloc;
  //! ConfigurationDumper handle.
  typedef PointerT<ConfigurationDumper,String::Handle>
          Handle;
  //! ConfigurationDumper bag.
  typedef ContainerT<ConfigurationDumper,String::Bag>
          Bag;

  explicit ConfigurationDumper(System& inSystem, Evolver& inEvolver, string inName="");
  virtual ~ConfigurationDumper() { }

  virtual void read(PACC::XML::ConstIterator inIter);

protected:
  System&  mSystem;     //!< Reference to the associated system.
  Evolver& mEvolver;    //!< Reference to the associated evolver.

};

}

#endif // Beagle_ConfigurationDumper_hpp

