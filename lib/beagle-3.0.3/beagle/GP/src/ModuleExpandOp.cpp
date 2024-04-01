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
 *  \file   beagle/GP/src/ModuleExpandOp.cpp
 *  \brief  Implementation of ModuleExpandOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.4.2.1 $
 *  $Date: 2007/05/09 01:51:07 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct an expand operator for evolutionary module acquisition.
 *  \param inName Name of the operator.
 */
GP::ModuleExpandOp::ModuleExpandOp(Beagle::string inName) :
  Operator(inName)
{ }


/*!
 *  \brief Initialize GP expand operator for evolutionary module acquisition.
 *  \param ioSystem Evolutionary system.
 */
void GP::ModuleExpandOp::initialize(Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle::Operator::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("gp.ema.modulename")) {
    mModulePrimitName = castHandleT<String>(ioSystem.getRegister()["gp.ema.modulename"]);
  } else {
    mModulePrimitName = new String("MODULE");
    Register::Description lDescription(
      "Module primitive name for EMA",
      "String",
      "MODULE",
      "Name of the GP primitive to use as module reference in the GP trees"
    );
    ioSystem.getRegister().addEntry("gp.ema.modulename", mModulePrimitName, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("gp.ema.expandpb")) {
    mExpandProba = castHandleT<Float>(ioSystem.getRegister()["gp.ema.expandpb"]);
  } else {
    mExpandProba = new Float(0.2f);
    Register::Description lDescription(
      "Probability of an individual being expanded",
      "Float",
      "0.2",
      string("Probability of an individual being expanded. Expansion randomly selects a ")+
      string("module primitive and inserts it back into the tree.")
    );
    ioSystem.getRegister().addEntry("gp.ema.expandpb", mExpandProba, lDescription);
  }
  Beagle_StackTraceEndM("void GP::ModuleExpandOp::initialize(Beagle::System& ioSystem)");
}


/*!
 *  \brief Expand given module of a GP tree.
 *  \param inNodeToExpand Index of node to expand in GP tree.
 *  \param ioTree Tree from which module will be expanded.
 *  \param ioContext Evolutionary context.
 */
void GP::ModuleExpandOp::expand(unsigned int inNodeToExpand,
                                GP::Tree& ioTree,
                                GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  // Log tree before expansion.
  Beagle_LogDebugM(  
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleExpandOp",
    string("Tree before expansion: ")+
    ioTree.serialize()
  );

  // Get the module index and reference.
  Module::Handle lModuleInstance = castHandleT<Module>(ioTree[inNodeToExpand].mPrimitive);
  unsigned int lModuleIndex = lModuleInstance->getIndex();
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleExpandOp",
    string("Expanding ")+uint2ordinal(lModuleIndex+1)+
    string(" module (called from ")+uint2ordinal(inNodeToExpand+1)+
    string(" node of the tree)")
  );
  ModuleVectorComponent::Handle lModuleVectorComponent =
    castHandleT<ModuleVectorComponent>(ioContext.getSystem().getComponent("ModuleVector"));
  if(lModuleVectorComponent==NULL) {
    throw Beagle_RunTimeExceptionM(string("GP system is not configured with a module vector. ")+
      string("Consider adding a GP::ModuleVectorComponent object to the system."));
  }
  Beagle::GP::Tree::Handle lModule = (*lModuleVectorComponent)[lModuleIndex];
  Beagle_LogDebugM(  
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleExpandOp",
    string("Module is: ")+lModule->serialize()
  );

  // Generate new tree.
  GP::Tree::Handle lNewTree =
    castHandleT<GP::Tree>(ioContext.getIndividual().getTypeAlloc()->allocate());
  string lArgName = lModuleInstance->getArgsName();
  ioTree.setContextToNode(inNodeToExpand, ioContext);
  for(unsigned int i=0; i<lModule->size(); ++i) {
    if((*lModule)[i].mPrimitive->getName() != lArgName) {
      lNewTree->push_back(GP::Node((*lModule)[i].mPrimitive));
    }
    else {
      GP::Argument::Handle lArg = castHandleT<GP::Argument>((*lModule)[i].mPrimitive);
      const unsigned int lChildIndex =
        ioTree[inNodeToExpand].mPrimitive->getChildrenNodeIndex(lArg->getIndex(), ioContext);
      lNewTree->insert(lNewTree->end(), ioTree.begin()+lChildIndex,
                       ioTree.begin()+lChildIndex+ioTree[lChildIndex].mSubTreeSize);
    }
  }
  ioTree.erase(ioTree.begin()+inNodeToExpand,
               ioTree.begin()+inNodeToExpand+ioTree[inNodeToExpand].mSubTreeSize);
  ioTree.insert(ioTree.begin()+inNodeToExpand, lNewTree->begin(), lNewTree->end());
  ioTree.fixSubTreeSize();

  // Log results.
  Beagle_LogDebugM(  
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleExpandOp",
    string("Tree after expansion: ")+
    ioTree.serialize()
  );
  Beagle_StackTraceEndM("void GP::ModuleExpandOp::expand(unsigned int inNodeToExpand, GP::Tree& ioTree, GP::Context& ioContext)");
}


/*!
 *  \brief Expand modules in trees of actual population.
 *  \param ioDeme Deme to apply operation.
 *  \param ioContext Evolutionary context.
 */
void GP::ModuleExpandOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleExpandOp",
    string("Expanding modules of the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  if(mExpandProba->getWrappedValue()==0.0) return;

  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleExpandOp",
    string("Expanding modules with probability ")+
    dbl2str(mExpandProba->getWrappedValue())
  );

  GP::Deme&    lGPDeme    = castObjectT<GP::Deme&>(ioDeme);
  GP::Context& lGPContext = castObjectT<GP::Context&>(ioContext);
  GP::Individual::Handle lOldIndividualHandle = lGPContext.getIndividualHandle();
  unsigned int lOldIndividualIndex = lGPContext.getIndividualIndex();
  GP::Tree::Handle lOldGenotypeHandle = lGPContext.getGenotypeHandle();
  unsigned int lOldGenotypeIndex = lGPContext.getGenotypeIndex();
  const string lModuleName = mModulePrimitName->getWrappedValue();
  for(unsigned int i=0; i<lGPDeme.size(); i++) {
    for(unsigned int j=0; j<lGPDeme[i]->size(); ++j) {
      GP::Tree& lTree = *(*lGPDeme[i])[j];
      for(unsigned int k=0; k<lTree.size(); ++k) {
        if(lTree[k].mPrimitive->getName() == lModuleName) {
          if(ioContext.getSystem().getRandomizer().rollUniform() <= mExpandProba->getWrappedValue()) {
            lGPContext.setIndividualHandle(lGPDeme[i]);
            lGPContext.setIndividualIndex(i);
            lGPContext.setGenotypeHandle((*lGPDeme[i])[j]);
            lGPContext.setGenotypeIndex(j);
            Beagle_LogVerboseM(
              ioContext.getSystem().getLogger(),
              "expand", "ExpandOp",
              string("Expanding the ") +uint2ordinal(i+1)+" individual"
            );
            expand(k, lTree, lGPContext);
          }
        }
      }
    }
  }
  lGPContext.setGenotypeHandle(lOldGenotypeHandle);
  lGPContext.setGenotypeIndex(lOldGenotypeIndex);
  lGPContext.setIndividualHandle(lOldIndividualHandle);
  lGPContext.setIndividualIndex(lOldIndividualIndex);

  Beagle_LogObjectDebugM(
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleExpandOp",
    *(ioContext.getSystem().getComponent("ModuleVector"))
  );
  Beagle_StackTraceEndM("void GP::ModuleExpandOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)");
}
