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
 *  \file   beagle/GP/src/ModuleCompressOp.cpp
 *  \brief  Implementation of ModuleCompressOp.
 *  \author Matthew Walker <m.g.walker@massey.ac.nz>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:07 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct an compress operator for evolutionary module acquisition.
 *  \param inName Name of the operator.
 */
GP::ModuleCompressOp::ModuleCompressOp(Beagle::string inName) :
  Operator(inName)
{ }


/*!
 *  \brief Initialize GP compress operator for evolutionary module acquisition.
 *  \param ioSystem Evolutionary system.
 */
void GP::ModuleCompressOp::initialize(Beagle::System& ioSystem) 
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

  if(ioSystem.getRegister().isRegistered("gp.ema.maxmodules")) {
    mMaxModulesVectorSize = castHandleT<UInt>(ioSystem.getRegister()["gp.ema.maxmodules"]);
  } else {
    mMaxModulesVectorSize = new UInt(25);
    Register::Description lDescription(
      "Maximum allowed modules",
      "UInt",
      "25",
      "Maximum number of allowed modules for evolutionary module acquisition"
    );
    ioSystem.getRegister().addEntry("gp.ema.maxmodules", mMaxModulesVectorSize, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("gp.ema.maxargs")) {
    mMaxModulesArgs = castHandleT<UInt>(ioSystem.getRegister()["gp.ema.maxargs"]);
  } else {
    mMaxModulesArgs = new UInt(4);
    Register::Description lDescription(
      "Max. number of module arguments",
      "UInt",
      "4",
      "Maximum number of allowed arguments to each module for evolutionary module acquisition"
    );
    ioSystem.getRegister().addEntry("gp.ema.maxargs", mMaxModulesArgs, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("gp.ema.compresspb")) {
    mCompressProba = castHandleT<Float>(ioSystem.getRegister()["gp.ema.compresspb"]);
  } else {
    mCompressProba = new Float(0.2f);
    Register::Description lDescription(
      "Probability of an individual being compressed",
      "Float",
      "0.2",
      string("Probability of an individual being compress. Expansion randomly selects a ")+
      string("subtree, create a module from it and change the subtree for a reference to ")+
      string("the module.")
    );
    ioSystem.getRegister().addEntry("gp.ema.compresspb", mCompressProba, lDescription);
  }
  Beagle_StackTraceEndM("void GP::ModuleCompressOp::initialize(Beagle::System& ioSystem) ");
}


/*!
 *  \brief Clean-up the module vector component from unused modules.
 *  \param ioVivarium Vivarium which is tested for module usage.
 *  \param ioContext Evolutionary context.
 */
void GP::ModuleCompressOp::cleanup(GP::Vivarium& ioVivarium, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  ModuleVectorComponent::Handle lModuleVectorComponent =
    castHandleT<ModuleVectorComponent>(ioContext.getSystem().getComponent("ModuleVector"));
  if(lModuleVectorComponent==NULL) {
    throw Beagle_RunTimeExceptionM(string("GP system is not configured with a module vector. ")+
      string("Consider adding a GP::ModuleVectorComponent object to the system."));
  }

  // Loop over the population and hall-of-fames for unused modules.
  const string lModuleName = mModulePrimitName->getWrappedValue();
  std::vector< bool,BEAGLE_STLALLOCATOR<bool> > lModuleUtilization(lModuleVectorComponent->size(), false);
  for(unsigned int i=0; i<ioVivarium.size(); ++i) {
    if(ioVivarium[i]==NULL) continue;
    GP::Deme& lDeme = *ioVivarium[i];
    for(unsigned int j=0; j<lDeme.size(); ++j) {
      if(lDeme[j]==NULL) continue;
      GP::Individual& lIndividual = *lDeme[j];
      for(unsigned int k=0; k<lIndividual.size(); ++k) {
        if(lIndividual[k]==NULL) continue;
        GP::Tree& lTree = *lIndividual[k];
        for(unsigned int l=0; l<lTree.size(); ++l) {
          if(lTree[l].mPrimitive->getName() == lModuleName) {
            GP::Module::Handle lModule = castHandleT<GP::Module>(lTree[l].mPrimitive);
            lModuleUtilization[lModule->getIndex()] = true;
          }
        }
      }
    }
    HallOfFame& lHoFDeme = lDeme.getHallOfFame();
    for(unsigned int j=0; j<lHoFDeme.size(); ++j) {
      if(lHoFDeme[j].mIndividual==NULL) continue;
      GP::Individual& lIndividual = castObjectT<GP::Individual&>(*lHoFDeme[j].mIndividual);
      for(unsigned int k=0; k<lIndividual.size(); ++k) {
        if(lIndividual[k]==NULL) continue;
        GP::Tree& lTree = *lIndividual[k];
        for(unsigned int l=0; l<lTree.size(); ++l) {
          if(lTree[l].mPrimitive->getName() == lModuleName) {
            GP::Module::Handle lModule = castHandleT<GP::Module>(lTree[l].mPrimitive);
            lModuleUtilization[lModule->getIndex()] = true;
          }
        }
      }
    }
  }
  HallOfFame& lHoFViva = ioVivarium.getHallOfFame();
  for(unsigned int j=0; j<lHoFViva.size(); ++j) {
    if(lHoFViva[j].mIndividual==NULL) continue;
    GP::Individual& lIndividual = castObjectT<GP::Individual&>(*lHoFViva[j].mIndividual);
    for(unsigned int k=0; k<lIndividual.size(); ++k) {
      if(lIndividual[k]==NULL) continue;
      GP::Tree& lTree = *lIndividual[k];
      for(unsigned int l=0; l<lTree.size(); ++l) {
        if(lTree[l].mPrimitive->getName() == lModuleName) {
          GP::Module::Handle lModule = castHandleT<GP::Module>(lTree[l].mPrimitive);
          lModuleUtilization[lModule->getIndex()] = true;
        }
      }
    }
  }

  // Remove unused modules from module vector.
  for(unsigned int i=0; i<lModuleUtilization.size(); ++i) {
    if(lModuleUtilization[i] == false) (*lModuleVectorComponent)[i] = NULL;
  }
  Beagle_StackTraceEndM("void GP::ModuleCompressOp::cleanup(GP::Vivarium& ioVivarium, GP::Context& ioContext)");
}


/*!
 *  \brief Compress tree into the given module.
 *  \param inModuleIndex Index of the module to create from compression.
 *  \param ioTree Tree to compress.
 *  \param ioContext Evolutionary context.
 *  \return Whether or not the tree has been compressed by modularization.
 */
bool GP::ModuleCompressOp::compress(unsigned int inModuleIndex,
                                    GP::Tree& ioTree,
                                    GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  // Get candidates for compression.
  std::vector< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> > lCandidates;
  listCompressionCandidates(lCandidates, 0, ioTree);
  if(lCandidates.size() == 0) return false;

  // Log tree before compression.
  Beagle_LogDebugM(  
    ioContext.getSystem().getLogger(),
    "EMA", "ModuleCompressOp",
    string("Tree before compression: ")+
    ioTree.serialize()
  );

  // Select the node to be the module and its argument.
  const unsigned int lNodeToCompress =
    lCandidates[ioContext.getSystem().getRandomizer().rollInteger(0, lCandidates.size()-1)];
  std::set< unsigned int,std::less<unsigned int>,BEAGLE_STLALLOCATOR<unsigned int> > lSelectedArgs;
  const unsigned int lNodeToCompressSubTreeSize = ioTree[lNodeToCompress].mSubTreeSize;
  std::list< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> > lPossibleArgs;
  for(unsigned int i=1; i<lNodeToCompressSubTreeSize; ++i) {
    lPossibleArgs.push_back(lNodeToCompress+i);
  }
  unsigned int lNbPossibleArgs = (lNodeToCompressSubTreeSize-1);
  const unsigned int lNbArgs =
    ioContext.getSystem().getRandomizer().rollInteger(1, mMaxModulesArgs->getWrappedValue());
  for(unsigned int i=0; (i<lNbArgs) && (lNbPossibleArgs>0); ++i) {
    const unsigned int lRNDValue =
      ioContext.getSystem().getRandomizer().rollInteger(0, (lNbPossibleArgs-1));
    unsigned int lArgIndex = 0;
    unsigned int lCounter=0;
    for(std::list< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> >::iterator lIter=lPossibleArgs.begin(); 
        lIter!=lPossibleArgs.end(); ++lIter) {
      if(lCounter++ == lRNDValue) {
        lArgIndex = *lIter;
        break;
      }
    }
    lSelectedArgs.insert(lArgIndex);
    const unsigned int lArgSubTreeSize = ioTree[lArgIndex].mSubTreeSize;
    for(std::list< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> >::iterator lIter=lPossibleArgs.begin();
        lIter!=lPossibleArgs.end();) {
      // Nodes that are parents to the selected argument, remove only direct parents.
      if((*lIter) < lArgIndex) {
        if(((*lIter)+ioTree[*lIter].mSubTreeSize) > lArgIndex) {
          std::list< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> >::iterator lEraseIter = lIter;
          ++lIter;
          lPossibleArgs.erase(lEraseIter);
          --lNbPossibleArgs;
        }
        else ++lIter;
      }
      // Remove selected argument and nodes in its subtree.
      else if((*lIter) < (lArgIndex+lArgSubTreeSize)) {
        std::list< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> >::iterator lEraseIter = lIter;
        ++lIter;
        lPossibleArgs.erase(lEraseIter);
        --lNbPossibleArgs;
      }
      // We are over last node of the selected argument subtree, we can stop iterating.
      else break;
    }
  }
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleCompressOp",
    string("Compressing the subtree of the ")+uint2ordinal(lNodeToCompress+1)+
    string(" node of tree as the  ")+uint2ordinal(inModuleIndex+1)+
    string(" module")
  );

  // Create the module.
  ModuleVectorComponent::Handle lModuleVectorComponent =
    castHandleT<ModuleVectorComponent>(ioContext.getSystem().getComponent("ModuleVector"));
  if(lModuleVectorComponent==NULL) {
    throw Beagle_RunTimeExceptionM(string("GP system is not configured with a module vector. ")+
      string("Consider adding a GP::ModuleVectorComponent object to the system."));
  }
  (*lModuleVectorComponent)[inModuleIndex] =
    castHandleT<GP::Tree>(ioContext.getIndividual().getTypeAlloc()->allocate());
  GP::Tree& lModule = *(*lModuleVectorComponent)[inModuleIndex];
  lModule.setPrimitiveSetIndex(ioTree.getPrimitiveSetIndex());
  const unsigned int lNbArgsSelected = lSelectedArgs.size();
  lModule.setNumberArguments(lNbArgsSelected);

  // Get a reference to the module and argument primitives.
  GP::PrimitiveSet& lPrimitSet = ioTree.getPrimitiveSet(ioContext);
  GP::Module::Handle lModPrimit =
    castHandleT<GP::Module>(lPrimitSet.getPrimitiveByName(mModulePrimitName->getWrappedValue()));
  if(lModPrimit == NULL) {
    std::ostringstream lOSS;
    lOSS << "Used primitive set of actual tree (index: " << ioTree.getPrimitiveSetIndex();
    lOSS << ") doesn't have a module primitive of name \"" << mModulePrimitName->getWrappedValue();
    lOSS << "\", as specified in parameter \"gp.ema.modulename\".";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  GP::Argument::Handle lArgPrimit =
    castHandleT<GP::Argument>(lPrimitSet.getPrimitiveByName(lModPrimit->getArgsName()));
  if(lArgPrimit == NULL) {
    std::ostringstream lOSS;
    lOSS << "Used primitive set of actual tree (index: " << ioTree.getPrimitiveSetIndex();
    lOSS << ") doesn't have a argument primitive of name \"" << lModPrimit->getArgsName();
    lOSS << "\", as specified by the module primitive \"" << lModPrimit->getName();
    lOSS << "\".";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }

  // Create new compressed tree and module tree.
  GP::Tree::Handle lNewTree =
    castHandleT<GP::Tree>(ioContext.getIndividual().getTypeAlloc()->allocate());
  Primitive::Handle lNewModPrimit = lModPrimit->generateInvoker(inModuleIndex,
                                                                lModPrimit->getName(),
                                                                lModPrimit->getArgsName(),
                                                                ioContext);
  lNewTree->push_back(GP::Node(lNewModPrimit));
  unsigned int lArgIndex=0;
  for(unsigned int i=0; i<lNodeToCompressSubTreeSize; ++i) {
    const unsigned int lChildIndex = i+lNodeToCompress;
    if(lSelectedArgs.find(lChildIndex)==lSelectedArgs.end()) lModule.push_back(ioTree[lChildIndex]);
    else {
      lModule.push_back(GP::Node(lArgPrimit->generateArgument(lArgIndex++)));
      const unsigned int lLastSubTreeIndex = i+ioTree[lChildIndex].mSubTreeSize;
      while(i<lLastSubTreeIndex) {
        lNewTree->push_back(ioTree[lNodeToCompress+i]);
        if((i+1) == lLastSubTreeIndex) break;
        else ++i;
      }
      Beagle_AssertM(i<lNodeToCompressSubTreeSize);
    }
  }
  Beagle_AssertM(lArgIndex == lNbArgsSelected);
  lModule.fixSubTreeSize();
  ioTree.erase(ioTree.begin()+lNodeToCompress,
               ioTree.begin()+lNodeToCompress+lNodeToCompressSubTreeSize);
  ioTree.insert(ioTree.begin()+lNodeToCompress, lNewTree->begin(), lNewTree->end());
  ioTree.fixSubTreeSize();

  // Log the module and compressed tree generated.
  Beagle_LogDebugM(  
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleCompressOp",
    string("New module generated is: ")+lModule.serialize()
  );
  Beagle_LogDebugM(  
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleCompressOp",
    string("Tree after compression: ")+
    ioTree.serialize()
  );

  return true;
  Beagle_StackTraceEndM("bool GP::ModuleCompressOp::compress(unsigned int inModuleIndex, GP::Tree& ioTree, GP::Context& ioContext)");
}


/*!
 *  \brief Recursive function used to list possible candidates in actual tree for module creation.
 *  \param outCandidates List of candidates in actual tree.
 *  \param inNodeIndex Node index in the GP tree for which the method is called.
 *  \param inTree Tree to get the candidates for compression.
 *  \return True is there was existing modules in subtree, false if not.
 */
bool GP::ModuleCompressOp::listCompressionCandidates(
  std::vector< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> >& outCandidates,
  unsigned int inNodeIndex,
  const GP::Tree& inTree) const
{
  Beagle_StackTraceBeginM();
  const unsigned int lNbArgs = inTree[inNodeIndex].mPrimitive->getNumberArguments();
  bool lModulesInSubTrees =
    (inTree[inNodeIndex].mPrimitive->getName() == mModulePrimitName->getWrappedValue());
  unsigned int lChildIndex = inNodeIndex+1;
  for(unsigned int i=0; i<lNbArgs; ++i) {
    if(listCompressionCandidates(outCandidates,lChildIndex,inTree)) lModulesInSubTrees = true;
    lChildIndex += inTree[lChildIndex].mSubTreeSize;
  }
  if((lNbArgs>0) && (lModulesInSubTrees==false)) outCandidates.push_back(inNodeIndex);
  return lModulesInSubTrees;
  Beagle_StackTraceEndM("bool GP::ModuleCompressOp::listCompressionCandidates(std::vector< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> >& outCandidates, unsigned int inNodeIndex, const GP::Tree& inTree) const");
}


/*!
 *  \brief Apply compression operator to the deme.
 *  \param ioDeme Deme to apply compression on.
 *  \param ioContext Evolutionary context.
 */
void GP::ModuleCompressOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  GP::Deme&    lGPDeme    = castObjectT<GP::Deme&>(ioDeme);
  GP::Context& lGPContext = castObjectT<GP::Context&>(ioContext);

  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleCompressOp",
    string("Creating modules of the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  if(mCompressProba->getWrappedValue()==0.0) return;

  // Cleaning up module vector once at each generation.
  if(ioContext.getDemeIndex()==0) {
    Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
      "EMA", "GP::ModuleCompressOp",
      string("Cleaning up the module vector component of unused modules before ")+
      string("apply compression")
    );
    cleanup(lGPContext.getVivarium(), lGPContext);
  }

  const float lCompressPb = mCompressProba->getWrappedValue();
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "EMA", "GP::ModuleCompressOp",
    string("Creating modules with probability ")+
    dbl2str(lCompressPb)
  );

  // Generate list of available modules.
  ModuleVectorComponent::Handle lModuleVectorComponent =
    castHandleT<ModuleVectorComponent>(ioContext.getSystem().getComponent("ModuleVector"));
  if(lModuleVectorComponent==NULL) {
    throw Beagle_RunTimeExceptionM(string("GP system is not configured with a module vector. ")+
      string("Consider adding a GP::ModuleVectorComponent object to the system."));
  }
  std::vector< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> > lEmptyModules;
  for(unsigned int i=lModuleVectorComponent->size(); i>0; --i) {
    if((*lModuleVectorComponent)[i-1] == NULL) lEmptyModules.push_back(i-1);
  }
  const unsigned int lMaxModuleVecSize = mMaxModulesVectorSize->getWrappedValue();
  if(lEmptyModules.empty() && (lModuleVectorComponent->size()>=lMaxModuleVecSize)) return;

  GP::Individual::Handle lOldIndividualHandle = lGPContext.getIndividualHandle();
  unsigned int lOldIndividualIndex = lGPContext.getIndividualIndex();
  GP::Tree::Handle lOldGenotypeHandle = lGPContext.getGenotypeHandle();
  unsigned int lOldGenotypeIndex = lGPContext.getGenotypeIndex();

  // Apply compression operation
  std::vector< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> > lIndivIndex(lGPDeme.size());
  for(unsigned int i=0; i<lIndivIndex.size(); ++i) lIndivIndex[i] = i;
  std::random_shuffle(lIndivIndex.begin(), lIndivIndex.end(), ioContext.getSystem().getRandomizer());
  for(unsigned int i=0; i<lIndivIndex.size(); ++i) {
    if(lEmptyModules.empty() && (lModuleVectorComponent->size()>=lMaxModuleVecSize)) break;
    if(ioContext.getSystem().getRandomizer().rollUniform() <= lCompressPb) {
      unsigned int lNewModIndex = 0;
      if(lEmptyModules.empty()) {
        if(lModuleVectorComponent->size()>=lMaxModuleVecSize) break;
        lNewModIndex = lModuleVectorComponent->size();
        lModuleVectorComponent->add(NULL);
      }
      else {
        lNewModIndex = lEmptyModules.back();
        lEmptyModules.pop_back();
      }
      unsigned int lNbNodesIndiv = 0;
      GP::Individual& lCompressedIndiv = *lGPDeme[lIndivIndex[i]];
      for(unsigned int j=0; j<lCompressedIndiv.size(); ++j) {
        lNbNodesIndiv += lCompressedIndiv[j]->size();
      }
      if(lNbNodesIndiv == 0) continue;
      unsigned int lNodeIndex = 
        lGPContext.getSystem().getRandomizer().rollInteger(0, (lNbNodesIndiv-1));
      unsigned int lTreeIndex = 0;
      for(; lNodeIndex>=lCompressedIndiv[lTreeIndex]->size(); ++lTreeIndex) {
        lNodeIndex -= lCompressedIndiv[lTreeIndex]->size();
      }
      Beagle_AssertM(lTreeIndex < lCompressedIndiv.size());
      if(lCompressedIndiv[lTreeIndex]->size() < 2) continue;
      lGPContext.setGenotypeHandle(lCompressedIndiv[lTreeIndex]);
      lGPContext.setGenotypeIndex(lTreeIndex);
      lGPContext.setIndividualHandle(lGPDeme[lIndivIndex[i]]);
      lGPContext.setIndividualIndex(lIndivIndex[i]);
      bool lCompressCompleted = compress(lNewModIndex, *lCompressedIndiv[lTreeIndex], lGPContext);
      if(lCompressCompleted==false) lEmptyModules.push_back(lNewModIndex);
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
  Beagle_StackTraceEndM("void GP::ModuleCompressOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)");
}

