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
 *  \file   beagle/GP/System.hpp
 *  \brief  Definition of the type GP::System.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:05 $
 */

/*!
 *  \defgroup GPSys GP Evolutionary System
 *  \ingroup GPF
 *  \brief Genetic programming evolutionary system components, part of the GP frameowork.
 */
 
#ifndef Beagle_GP_System_hpp
#define Beagle_GP_System_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/AssertException.hpp"
#include "beagle/Context.hpp"
#include "beagle/GP/PrimitiveSuperSet.hpp"
#include "beagle/GP/ModuleVectorComponent.hpp"


namespace Beagle {
namespace GP {

/*!
 *  \class System beagle/GP/System.hpp "beagle/GP/System.hpp"
 *  \brief The GP system class.
 *  \ingroup GPF
 *  \ingroup GPSys
 */
class System : public Beagle::System {

public:

  //! GP::System allocator type.
  typedef AllocatorT<System,Beagle::System::Alloc>
          Alloc;
  //! GP::System handle type.
  typedef PointerT<System,Beagle::System::Handle>
          Handle;
  //! GP::System bag type.
  typedef ContainerT<System,Beagle::System::Bag>
          Bag;

           System();
  explicit System(GP::PrimitiveSuperSet::Handle inSuperSet);
  explicit System(GP::PrimitiveSuperSet::Handle inSuperSet,
                  Beagle::Context::Alloc::Handle inGPContextAllocator);
  explicit System(GP::PrimitiveSuperSet::Handle inSuperSet,
                  Beagle::Context::Alloc::Handle inGPContextAllocator,
                  Randomizer::Handle inRandomizer,
                  Register::Handle inRegister,
                  Logger::Handle inLogger,
                  GP::ModuleVectorComponent::Handle inModuleVector);
  explicit System(GP::PrimitiveSet::Handle inSet);
  explicit System(GP::PrimitiveSet::Handle inSet,
                  Beagle::Context::Alloc::Handle inGPContextAllocator);
  explicit System(GP::PrimitiveSet::Handle inSet,
                  Beagle::Context::Alloc::Handle inGPContextAllocator,
                  Randomizer::Handle inRandomizer,
                  Register::Handle inRegister,
                  Logger::Handle inLogger,
                  GP::ModuleVectorComponent::Handle inModuleVector);
  virtual ~System() { }

  virtual void read(PACC::XML::ConstIterator inIter);

  /*!
   *  \brief Get a constant reference to the GP context allocator of the system.
   *  \return Constant reference to the GP context allocator.
   */
  inline const Beagle::Context::Alloc& getContextAllocator() const
  {
    Beagle_StackTraceBeginM();
    return castObjectT<const Beagle::Context::Alloc&>(*mContextAllocator);
    Beagle_StackTraceEndM("const Context::Alloc& GP::System::getContextAllocator() const");
  }

  /*!
   *  \brief Get reference to the GP context allocator of the system.
   *  \return Reference to the GP context allocator.
   */
  inline Beagle::Context::Alloc& getContextAllocator()
  {
    Beagle_StackTraceBeginM();
    return castObjectT<Beagle::Context::Alloc&>(*mContextAllocator);
    Beagle_StackTraceEndM("Context::Alloc& GP::System::getContextAllocator()");
  }

  /*!
   *  \brief Get a handle to the GP context allocator of the system.
   *  \return Handle to the GP context allocator.
   */
  inline Beagle::Context::Alloc::Handle getContextAllocatorHandle()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<Beagle::Context::Alloc>(mContextAllocator);
    Beagle_StackTraceEndM("Context::Alloc::Handle GP::System::getContextAllocatorHandle()");
  }

  /*!
   *  \brief Get a constant reference to the GP primitive super set of the system.
   *  \return Constant reference to the GP primitive super set allocator.
   */
  inline const GP::PrimitiveSuperSet& getPrimitiveSuperSet() const
  {
    Beagle_StackTraceBeginM();
    return *mPrimitiveSuperSet;
    Beagle_StackTraceEndM("const GP::PrimitiveSuperSet& GP::System::getPrimitiveSuperSet() const");
  }

  /*!
   *  \brief Get a reference to the GP primitive super set of the system.
   *  \return Reference to the GP primitive super set allocator.
   */
  inline GP::PrimitiveSuperSet& getPrimitiveSuperSet()
  {
    Beagle_StackTraceBeginM();
    return *mPrimitiveSuperSet;
    Beagle_StackTraceEndM("GP::PrimitiveSuperSet& GP::System::getPrimitiveSuperSet()");
  }

  /*!
   *  \brief Get a handle to the GP primitive super set of the system.
   *  \return Handle to the GP primitive super set allocator.
   */
  inline GP::PrimitiveSuperSet::Handle getPrimitiveSuperSetHandle()
  {
    Beagle_StackTraceBeginM();
    return mPrimitiveSuperSet;
    Beagle_StackTraceEndM("GP::PrimitiveSuperSet::Handle GP::System::getPrimitiveSuperSetHandle()");
  }

protected:

  GP::PrimitiveSuperSet::Handle mPrimitiveSuperSet;   //!< The super set of primitives

};

}
}

#endif // Beagle_GP_System_hpp



