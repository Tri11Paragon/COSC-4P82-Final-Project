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
 *  \file   beagle/GP/src/ModuleVectorComponent.cpp
 *  \brief  Implementation of class ModuleVectorComponent.
 *  \author Matthew Walker
 *  \author Christian Gagne
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:07 $
 */
 

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct module vector GP system component for evolutionary module acquisition.
 *  \param inTreeAlloc Tree allocator used to generate used individuals.
 */
GP::ModuleVectorComponent::ModuleVectorComponent(GP::Tree::Alloc::Handle inTreeAlloc) : 
  Component("ModuleVector"),
  mModules(inTreeAlloc)
{ }


/*!
 *  \brief Read module vector from XML subtree with a context.
 *  \param inIter XML iterator to read modules from.
 *  \param ioSystem Evolutionary system.
 */
void GP::ModuleVectorComponent::readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="ModuleVector"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <ModuleVector> expected!");

  GP::Context::Handle lGPContext =
    castHandleT<GP::Context>(ioSystem.getContextAllocator().allocate());
  GP::System& lGPSystem = castObjectT<GP::System&>(ioSystem);
  lGPContext->setSystemHandle(&lGPSystem);
  mModules.resize(0);
  GP::Tree::Alloc::Handle lTreeAlloc = castHandleT<GP::Tree::Alloc>(mModules.getTypeAlloc());
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Genotype")) {
      if(lChild->getFirstChild()==NULL) mModules.push_back(NULL);
      else {
        mModules.push_back(lTreeAlloc->allocate());
        lGPContext->setGenotypeHandle(mModules.back());
        lGPContext->setGenotypeIndex(mModules.size()-1);
        mModules.back()->readWithContext(lChild, *lGPContext);
      }
    }
  }
  Beagle_StackTraceEndM("void GP::ModuleVectorComponent::readWithSystem(PACC::XML::ConstIterator inIter, Beagle::System& ioSystem)");
}


/*!
 *  \brief Write modules into XML streamer.
 *  \param ioStreamer XML streamer to write modules into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::ModuleVectorComponent::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("ModuleVector");
  for(unsigned int i=0; i<mModules.size(); ++i) {
    if(mModules[i]==NULL) {
      ioStreamer.openTag("Genotype", inIndent);
      ioStreamer.closeTag();
    }
    else mModules[i]->write(ioStreamer, inIndent);
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void GP::ModuleVectorComponent::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
