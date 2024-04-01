/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/Context.hpp
 *  \brief  Definition of the type LinGP::System.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */

/*!
 *  \defgroup LinGPSys Linear GP Evolutionary System
 *  \ingroup LinGPF
 *  \brief Linear GP evolutionary system components, part of the linear GP frameowork.
 */
 
#ifndef Beagle_LinGP_System_hpp
#define Beagle_LinGP_System_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/AssertException.hpp"
#include "beagle/Context.hpp"
#include "beagle/LinGP/InstructionSuperSet.hpp"

namespace Beagle {
namespace LinGP {

/*!
 *  \class System beagle/LinGP/System.hpp "beagle/LinGP/System.hpp"
 *  \brief The linear GP system class.
 *  \ingroup LinGPF
 *  \ingroup LinGPSys
 */
class System : public Beagle::System {

public:

  //! LinGP::System allocator type.
  typedef AllocatorT<System,Beagle::System::Alloc>
          Alloc;
  //! LinGP::System handle type.
  typedef PointerT<System,Beagle::System::Handle>
          Handle;
  //! LinGP::System bag type.
  typedef ContainerT<System,Beagle::System::Bag>
          Bag;

           System();
  explicit System(LinGP::InstructionSuperSet::Handle inSuperSet);
  explicit System(LinGP::InstructionSuperSet::Handle inSuperSet,
                  Beagle::Context::Alloc::Handle inContextAllocator);
  explicit System(LinGP::InstructionSuperSet::Handle inSuperSet,
                  Beagle::Context::Alloc::Handle inContextAllocator,
                  Randomizer::Handle inRandomizer,
                  Register::Handle   inRegister,
                  Logger::Handle     inLogger);
  explicit System(LinGP::InstructionSet::Handle inSet);
  explicit System(LinGP::InstructionSet::Handle inSet,
                  Beagle::Context::Alloc::Handle inContextAllocator);
  explicit System(LinGP::InstructionSet::Handle inSet,
                  Beagle::Context::Alloc::Handle inContextAllocator,
                  Randomizer::Handle inRandomizer,
                  Register::Handle   inRegister,
                  Logger::Handle     inLogger);
  virtual ~System() { }

  virtual void initialize(int& ioArgc, char** ioArgv);
  virtual void initialize(Beagle::string inRegisterFileName);

  /*!
   *  \brief Get a constant reference to the linear GP context allocator of the system.
   *  \return Constant reference to the linear GP context allocator.
   */
  inline const Beagle::Context::Alloc& getContextAllocator() const
  {
    return castObjectT<const Beagle::Context::Alloc&>(*mContextAllocator);
  }

  /*!
   *  \brief Get reference to the linear GP context allocator of the system.
   *  \return Reference to the linear GP context allocator.
   */
  inline Beagle::Context::Alloc& getContextAllocator()
  {
    return castObjectT<Beagle::Context::Alloc&>(*mContextAllocator);
  }

  /*!
   *  \brief Get a handle to the linear GP context allocator of the system.
   *  \return Handle to the linear GP context allocator.
   */
  inline Beagle::Context::Alloc::Handle getContextAllocatorHandle()
  {
    return castHandleT<Beagle::Context::Alloc>(mContextAllocator);
  }

  /*!
   *  \brief Get a constant reference to the linear GP instruction super set of the system.
   *  \return Constant reference to the linear GP instruction super set allocator.
   */
  inline const LinGP::InstructionSuperSet& getInstructionSuperSet() const
  {
    return *mInstructionSuperSet;
  }

  /*!
   *  \brief Get a reference to the linear GP instruction super set of the system.
   *  \return Reference to the linear GP instruction super set allocator.
   */
  inline LinGP::InstructionSuperSet& getInstructionSuperSet()
  {
    return *mInstructionSuperSet;
  }

  /*!
   *  \brief Get a handle to the linear GP instruction super set of the system.
   *  \return Handle to the linear GP instruction super set allocator.
   */
  inline LinGP::InstructionSuperSet::Handle getInstructionSuperSetHandle()
  {
    return mInstructionSuperSet;
  }

protected:
  LinGP::InstructionSuperSet::Handle mInstructionSuperSet;   //!< The super set of instructions

};

}
}

#endif // Beagle_LinGP_System_hpp



