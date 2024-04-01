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
 *  \file   beagle/GP/src/PrimitiveSuperSet.cpp
 *  \brief  Implementation of the type GP::PrimitiveSuperSet.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.1 $
 *  $Date: 2007/05/09 01:51:07 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct primitive super set.
 */
GP::PrimitiveSuperSet::PrimitiveSuperSet(GP::PrimitiveSet::Alloc::Handle inPrimitSetAlloc) :
  Component("PrimitiveSuperSet"),
  mPrimitSets(inPrimitSetAlloc)
{
  Beagle_StackTraceBeginM();
  addBasicPrimitives();
  Beagle_StackTraceEndM("GP::PrimitiveSuperSet::PrimitiveSuperSet(GP::PrimitiveSet::Alloc::Handle inPrimitSetAlloc)");
}


/*!
 *  \brief Add common primitives to the primitive super set. 
 */
void GP::PrimitiveSuperSet::addBasicPrimitives()
{
  Beagle_StackTraceBeginM();
  addPrimitive(new GP::ADF);
  addPrimitive(new GP::ADF(GP::Invoker::eGenerator, GP::Primitive::eAny, "ADFI", "ARGI"));
  addPrimitive(new GP::ADF(GP::Invoker::eGenerator, GP::Primitive::eAny, "ADFB", "ARGB"));
  addPrimitive(new GP::Module);
  addPrimitive(new GP::Module(GP::Invoker::eGenerator, GP::Primitive::eAny, "MODULEI", "ARGI"));
  addPrimitive(new GP::Module(GP::Invoker::eGenerator, GP::Primitive::eAny, "MODULEB", "ARGB"));
  addPrimitive(new GP::ArgumentT<Double>);
  addPrimitive(new GP::ArgumentT<Int>(GP::Argument::eCaching, GP::Argument::eGenerator, "ARGI"));
  addPrimitive(new GP::ArgumentT<Bool>(GP::Argument::eCaching, GP::Argument::eGenerator, "ARGB"));
  addPrimitive(new GP::IfThenElseT<Double>("IFTE"));
  addPrimitive(new GP::IfThenElseT<Int>("IFTEI"));
  addPrimitive(new GP::IfThenElseT<Bool>("IFTEB"));
  addPrimitive(new GP::AbsT<Double>("ABS"));
  addPrimitive(new GP::AbsT<Int>("ABSI"));
  addPrimitive(new GP::Add);
  addPrimitive(new GP::AddT<Int>("ADDI"));
  addPrimitive(new GP::Subtract);
  addPrimitive(new GP::SubtractT<Int>("SUBI"));
  addPrimitive(new GP::Multiply);
  addPrimitive(new GP::MultiplyT<Int>("MULI"));
  addPrimitive(new GP::Divide);
  addPrimitive(new GP::Sin);
  addPrimitive(new GP::Cos);
  addPrimitive(new GP::Exp);
  addPrimitive(new GP::Log);
  addPrimitive(new GP::EphemeralDouble);
  addPrimitive(new GP::And);
  addPrimitive(new GP::Or);
  addPrimitive(new GP::Not);
  addPrimitive(new GP::Nand);
  addPrimitive(new GP::Nor);
  addPrimitive(new GP::Xor);
  Beagle_StackTraceEndM("void GP::PrimitiveSuperSet::addBasicPrimitives()");
}


/*!
 *  \brief Initialize the super set of primitive.
 */
void GP::PrimitiveSuperSet::initialize(GP::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Component::initialize(ioSystem);

  Beagle_LogDetailedM(
    ioSystem.getLogger(),
    "initialization","Beagle::GP::PrimitiveSet",
    "Initializing primitive super set"
  );
  for(unsigned int i=0; i<mPrimitSets.size(); i++) {
    Beagle_LogVerboseM(
      ioSystem.getLogger(),
      "initialization","Beagle::GP::PrimitiveSet",
      string("Initializing ")+uint2ordinal(i+1)+string(" primitive set")
    );
    mPrimitSets[i]->initialize(ioSystem);
  }
  Beagle_StackTraceEndM("void GP::PrimitiveSuperSet::initialize(GP::System& ioSystem)");
}


/*!
 *  \brief Insert a nes set of primitives in the super set.
 *  \param inPrimitiveSet Inserted primitive set.
 */
void GP::PrimitiveSuperSet::insert(GP::PrimitiveSet::Handle inPrimitiveSet)
{
  Beagle_StackTraceBeginM();
  mPrimitSets.push_back(inPrimitiveSet);
  for(unsigned int i=0; i<inPrimitiveSet->size(); ++i) addPrimitive((*inPrimitiveSet)[i]);
  Beagle_StackTraceEndM("void GP::PrimitiveSuperSet::insert(GP::PrimitiveSet::Handle inPrimitiveSet)");
}


/*!
 *  \brief Read a primitive super set from a XML subtree.
 *  \param inIter XML iterator used to read the super set from.
 *  \param ioSystem Evolutionary system.
 *  \throw Beagle::IOException If size atribute not present.
 */
void GP::PrimitiveSuperSet::readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="PrimitiveSuperSet"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <PrimitiveSuperSet> expected!");
  GP::Context::Handle lGPContext =
    castHandleT<GP::Context>(ioSystem.getContextAllocator().allocate());
  GP::System& lGPSystem = castObjectT<GP::System&>(ioSystem);
  lGPContext->setSystemHandle(&lGPSystem);
  unsigned int lNbPrimitSet=0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="PrimitiveSet")) ++lNbPrimitSet;
  }
  mPrimitSets.resize(lNbPrimitSet);
  unsigned int lPSIndex=0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="PrimitiveSet")) {
      mPrimitSets[lPSIndex++]->readWithContext(lChild, *lGPContext);
    }
  }
  Beagle_StackTraceEndM("void GP::PrimitiveSuperSet::readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem)");
}


/*!
 *  \brief Write a primitive super set into a Beagle XML stream.
 *  \param ioStreamer XMl streamer to write the super set into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::PrimitiveSuperSet::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("PrimitiveSuperSet", inIndent);
  for(unsigned int i=0; i<mPrimitSets.size(); i++) mPrimitSets[i]->write(ioStreamer, inIndent);
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void GP::PrimitiveSuperSet::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

