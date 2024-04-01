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
 *  \file   beagle/GP/src/System.cpp
 *  \brief  Implementation of the type GP::System.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.1 $
 *  $Date: 2007/05/09 01:51:07 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a new GP system.
 */
GP::System::System() :
  Beagle::System(new GP::Context::Alloc),
  mPrimitiveSuperSet(new GP::PrimitiveSuperSet)
{
  Beagle_StackTraceBeginM();
  addComponent(mPrimitiveSuperSet);
  addComponent(new ModuleVectorComponent);
  Beagle_StackTraceEndM("GP::System::System()");
}


/*!
 *  \brief Construct a new GP system.
 *  \param inSuperSet Handle to the primitive super set to use.
 */
GP::System::System(GP::PrimitiveSuperSet::Handle inSuperSet) :
  Beagle::System(new GP::Context::Alloc),
  mPrimitiveSuperSet(inSuperSet)
{
  Beagle_StackTraceBeginM();
  addComponent(mPrimitiveSuperSet);
  addComponent(new ModuleVectorComponent);
  Beagle_StackTraceEndM("GP::System::System(GP::PrimitiveSuperSet::Handle inSuperSet)");
}


/*!
 *  \brief Construct a new GP system.
 *  \param inSuperSet Handle to the primitive super set to use.
 *  \param inGPContextAllocator Context allocator to use.
 */
GP::System::System(GP::PrimitiveSuperSet::Handle inSuperSet,
                   Beagle::Context::Alloc::Handle inGPContextAllocator) :
  Beagle::System(inGPContextAllocator),
  mPrimitiveSuperSet(inSuperSet)
{
  Beagle_StackTraceBeginM();
  addComponent(mPrimitiveSuperSet);
  addComponent(new ModuleVectorComponent);
  Beagle_StackTraceEndM("GP::System::System(GP::PrimitiveSuperSet::Handle inSuperSet, Beagle::Context::Alloc::Handle inGPContextAllocator)");
}


/*!
 *  \brief Construct a new GP system.
 *  \param inSuperSet Handle to the primitive super set to use.
 *  \param inGPContextAllocator Context allocator to use.
 *  \param inRandomizer Handle to the randomizer used.
 *  \param inRegister Handle to the register used.
 *  \param inLogger Handle to the logger used.
 *  \param inModuleVector Module vector component to use.
 */
GP::System::System(GP::PrimitiveSuperSet::Handle inSuperSet,
                   Beagle::Context::Alloc::Handle inGPContextAllocator,
                   Randomizer::Handle inRandomizer,
                   Register::Handle inRegister,
                   Logger::Handle inLogger,
                   GP::ModuleVectorComponent::Handle inModuleVector) :
  Beagle::System(inGPContextAllocator, inRandomizer, inRegister, inLogger),
  mPrimitiveSuperSet(inSuperSet)
{
  Beagle_StackTraceBeginM();
  addComponent(mPrimitiveSuperSet);
  addComponent(inModuleVector);
  Beagle_StackTraceEndM("GP::System::System(GP::PrimitiveSuperSet::Handle inSuperSet, Beagle::Context::Alloc::Handle inGPContextAllocator, Randomizer::Handle inRandomizer, Register::Handle inRegister, Logger::Handle inLogger, GP::ModuleVectorComponent::Handle inModuleVector)");
}


/*!
 *  \brief Construct a new GP system.
 *  \param inSet Handle to the primitive set to use.
 */
GP::System::System(GP::PrimitiveSet::Handle inSet) :
  Beagle::System(new GP::Context::Alloc),
  mPrimitiveSuperSet(new PrimitiveSuperSet)
{
  Beagle_StackTraceBeginM();
  addComponent(mPrimitiveSuperSet);
  mPrimitiveSuperSet->insert(inSet);
  addComponent(new ModuleVectorComponent);
  Beagle_StackTraceEndM("GP::System::System(GP::PrimitiveSet::Handle inSet)");
}


/*!
 *  \brief Construct a new GP system.
 *  \param inSet Handle to the primitive set to use.
 *  \param inGPContextAllocator Context allocator to use.
 */
GP::System::System(GP::PrimitiveSet::Handle inSet,
                   Beagle::Context::Alloc::Handle inGPContextAllocator) :
  Beagle::System(inGPContextAllocator),
  mPrimitiveSuperSet(new PrimitiveSuperSet)
{
  Beagle_StackTraceBeginM();
  addComponent(mPrimitiveSuperSet);
  mPrimitiveSuperSet->insert(inSet);
  addComponent(new ModuleVectorComponent);
  Beagle_StackTraceEndM("GP::System::System(GP::PrimitiveSet::Handle inSet, Beagle::Context::Alloc::Handle inGPContextAllocator)");
}


/*!
 *  \brief Construct a new GP system.
 *  \param inSet Handle to the primitive set to use.
 *  \param inGPContextAllocator Context allocator to use.
 *  \param inRandomizer Handle to the randomizer used.
 *  \param inRegister Handle to the register used.
 *  \param inLogger Handle to the logger used.
 *  \param inModuleVector Module vector component to use.
 */
GP::System::System(GP::PrimitiveSet::Handle inSet,
                   Beagle::Context::Alloc::Handle inGPContextAllocator,
                   Randomizer::Handle inRandomizer,
                   Register::Handle inRegister,
                   Logger::Handle inLogger,
                   GP::ModuleVectorComponent::Handle inModuleVector) :
  Beagle::System(inGPContextAllocator, inRandomizer, inRegister, inLogger),
  mPrimitiveSuperSet(new PrimitiveSuperSet)
{
  Beagle_StackTraceBeginM();
  addComponent(mPrimitiveSuperSet);
  mPrimitiveSuperSet->insert(inSet);
  addComponent(inModuleVector);
  Beagle_StackTraceEndM("GP::System::System(GP::PrimitiveSet::Handle inSet, Beagle::Context::Alloc::Handle inGPContextAllocator, Randomizer::Handle inRandomizer, Register::Handle inRegister, Logger::Handle inLogger, GP::ModuleVectorComponent::Handle inModuleVector)");
}


/*!
 *  \brief Read evolutionary system from XML subtree.
 *  \param inIter XML iterator used to read system.
 */
void GP::System::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="System"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <System> expected!");

  // First reading pass, read randomizer, register, logger and primitive super set.
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      if(lChild->getValue() == "Randomizer")    mRandomizer->readWithSystem(lChild, *this);
      else if(lChild->getValue() == "Register") mRegister->readWithSystem(lChild, *this);
      else if(lChild->getValue() == "Logger")   mLogger->readWithSystem(lChild, *this);
      else if(lChild->getValue() == "PrimitiveSuperSet")
        mPrimitiveSuperSet->readWithSystem(lChild, *this);
    }
  }

  // Second reading pass, read other components.
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      if((lChild->getValue() != "Randomizer") && 
         (lChild->getValue() != "Register") &&
         (lChild->getValue() != "Logger") &&
         (lChild->getValue() != "PrimitiveSuperSet")) {
        if(find(lChild->getValue().c_str())==end())
          throw Beagle_ObjectExceptionM(string("The component named \"")+
                                        string(lChild->getValue().c_str())+
                                        string("\" is not present in the system."));
        (*this)[lChild->getValue().c_str()]->readWithSystem(lChild, *this);
      }
    }
  }
  Beagle_StackTraceEndM("void GP::System::read(PACC::XML::ConstIterator inIter)");
}

