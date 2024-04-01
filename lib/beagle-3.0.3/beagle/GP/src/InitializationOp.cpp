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

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a plain new initialization operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register.
 *  \param inName Name of the initialization operator.
 */
GP::InitializationOp::InitializationOp(std::string inReproProbaName, std::string inName) :
  Beagle::InitializationOp(inReproProbaName, inName)
{ }


/*!
 *  \brief Register the parameters of the generic GP intialization operator.
 *  \param ioSystem System of the evolution.
 */
void GP::InitializationOp::initialize(Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle::InitializationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("gp.init.maxdepth")) {
    mMaxTreeDepth = castHandleT<UInt>(ioSystem.getRegister()["gp.init.maxdepth"]);
  } else {
    mMaxTreeDepth = new UInt(5);
    Register::Description lDescription(
      "Maximum initial tree depth",
      "UInt",
      "5",
      "Maximum depth for newly initialized trees."
    );
    ioSystem.getRegister().addEntry("gp.init.maxdepth", mMaxTreeDepth, lDescription);
  }
  if(ioSystem.getRegister().isRegistered("gp.init.mindepth")) {
    mMinTreeDepth = castHandleT<UInt>(ioSystem.getRegister()["gp.init.mindepth"]);
  } else {
    mMinTreeDepth = new UInt(2);
    Register::Description lDescription(
      "Minimum initial tree depth",
      "UInt",
      "2",
      "Minimum depth for newly initialized trees."
    );
    ioSystem.getRegister().addEntry("gp.init.mindepth", mMinTreeDepth, lDescription);
  }
  if(ioSystem.getRegister().isRegistered("gp.init.maxtree")) {
    mMaxNumberTrees = castHandleT<UInt>(ioSystem.getRegister()["gp.init.maxtree"]);
  } else {
    mMaxNumberTrees = new UInt(1);
    std::ostringstream lOSS;
    lOSS << "Maximum number of GP tree in newly initialized individuals. More than one tree ";
    lOSS << "is usually useful with ADFs (and other ADx).";
    Register::Description lDescription(
      "Maximum number of trees",
      "UInt",
      "1",
      lOSS.str()
    );
    ioSystem.getRegister().addEntry("gp.init.maxtree", mMaxNumberTrees, lDescription);
  }
  if(ioSystem.getRegister().isRegistered("gp.init.mintree")) {
    mMinNumberTrees = castHandleT<UInt>(ioSystem.getRegister()["gp.init.mintree"]);
  } else {
    mMinNumberTrees = new UInt(1);
    std::ostringstream lOSS;
    lOSS << "Minimum number of GP tree in newly initialized individuals. More than one tree ";
    lOSS << "is usually useful with ADFs (and other ADx).";
    Register::Description lDescription(
      "Minimum number of trees",
      "UInt",
      "1",
      lOSS.str()
    );
    ioSystem.getRegister().addEntry("gp.init.mintree", mMinNumberTrees, lDescription);
  }
  if(ioSystem.getRegister().isRegistered("gp.init.maxargs")) {
    mMaxTreeArgs = castHandleT<UIntArray>(ioSystem.getRegister()["gp.init.maxargs"]);
  } else {
    mMaxTreeArgs = new UIntArray;
    mMaxTreeArgs->push_back(0);
    mMaxTreeArgs->push_back(2);
    std::ostringstream lOSS;
    lOSS << "Maximum number of arguments in GP tree. Tree arguments are ";
    lOSS << "is usually useful with ADFs (and similar stuff).";
    Register::Description lDescription(
      "Max. number of tree arguments",
      "UIntArray",
      "0/2",
      lOSS.str()
    );
    ioSystem.getRegister().addEntry("gp.init.maxargs", mMaxTreeArgs, lDescription);
  }
  if(ioSystem.getRegister().isRegistered("gp.init.minargs")) {
    mMinTreeArgs = castHandleT<UIntArray>(ioSystem.getRegister()["gp.init.minargs"]);
  } else {
    mMinTreeArgs = new UIntArray;
    mMinTreeArgs->push_back(0);
    mMinTreeArgs->push_back(2);
    std::ostringstream lOSS;
    lOSS << "Minimum number of arguments in GP tree. Tree arguments are ";
    lOSS << "is usually useful with ADFs a(nd similar stuff).";
    Register::Description lDescription(
      "Min. number of tree arguments",
      "UIntArray",
      "0/2",
      lOSS.str()
    );
    ioSystem.getRegister().addEntry("gp.init.minargs", mMinTreeArgs, lDescription);
  }
  Beagle_StackTraceEndM("void GP::InitializationOp::registerParams(Beagle::System&)");
}


/*!
 *  \brief Initialize the trees of an individual.
 *  \param outIndividual Individual to initialize.
 *  \param ioContext Evolution context.
 *  \throw Beagle::RunTimeException If the min/max depths are incorrectly set.
 */
void GP::InitializationOp::initIndividual(Beagle::Individual& outIndividual,
                                          Beagle::Context& ioContext)
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  if (mMinTreeDepth==NULL || mMaxTreeDepth==NULL)
    throw Beagle_RunTimeExceptionM(std::string("GP::InitializationOp has not been initialized.").
      append(" Consider the GP::InitializationOp::registerParams() method."));
  if(*mMinTreeDepth > *mMaxTreeDepth) {
    std::string lMessage = "GP::InitializationOp::initIndividual: Minimum tree depth is superior ";
    lMessage += "to the maximum tree depth. Could not initialize the individuals!";
    throw Beagle::ValidationException(lMessage);
  }
#endif // BEAGLE_NDEBUG
  Beagle_ValidateParameterM(mMinTreeDepth->getWrappedValue()>0,"gp.init.mindepth",">0");

  GP::Individual& lIndividual = castObjectT<GP::Individual&>(outIndividual);
  GP::Context& lContext = castObjectT<GP::Context&>(ioContext);

  const unsigned int lPrimitiveSuperSetSize = lContext.getSystem().getPrimitiveSuperSet().size();
#ifndef BEAGLE_NDEBUG
  if(lPrimitiveSuperSetSize == 0)
    throw Beagle_RunTimeExceptionM(std::string("GP::InitializationOp::initIndividual(): There ").
      append(" are no PrimitiveSets in the PrimitiveSuperSet.  There needs to be at least one").
      append(" PrimitiveSet. See the examples that are included with Beagle to learn how").
      append(" to create a PrimitiveSet, add Primitives to it, and then construct a System based").
      append(" on the PrimitiveSet."));
#endif // BEAGLE_NDEBUG

  // Choose randomly the number of individuals in tree
  const unsigned int lMaxDepth   = mMaxTreeDepth->getWrappedValue();
  const unsigned int lMinDepth   = mMinTreeDepth->getWrappedValue();
  const unsigned int lMaxNbTrees = mMaxNumberTrees->getWrappedValue();
  const unsigned int lMinNbTrees = mMinNumberTrees->getWrappedValue();
  Beagle_AssertM(lMaxNbTrees >= lMinNbTrees);
  const unsigned int lNbTrees =
    ioContext.getSystem().getRandomizer().rollInteger(lMinNbTrees,lMaxNbTrees);
  lIndividual.resize(lNbTrees);
  GP::Tree::Handle lOldTreeHandle = lContext.getGenotypeHandle();
  unsigned int lOldTreeIndex = lContext.getGenotypeIndex();
  for(unsigned int i=0; i<lIndividual.size(); ++i) {
    if(i<lPrimitiveSuperSetSize) lIndividual[i]->setPrimitiveSetIndex(i);
    else lIndividual[i]->setPrimitiveSetIndex(lPrimitiveSuperSetSize-1);
    const unsigned int lMaxArgs =
      (i<mMaxTreeArgs->size()) ? (*mMaxTreeArgs)[i] : mMaxTreeArgs->back();
    const unsigned int lMinArgs =
      (i<mMinTreeArgs->size()) ? (*mMinTreeArgs)[i] : mMinTreeArgs->back();
    Beagle_AssertM(lMaxArgs >= lMinArgs);
    const unsigned int lNbArgs =
      ioContext.getSystem().getRandomizer().rollInteger(lMinArgs,lMaxArgs);
    lIndividual[i]->setNumberArguments(lNbArgs);
  }
  for(unsigned int i=0; i<lIndividual.size(); ++i) {
    lContext.setGenotypeHandle(lIndividual[i]);
    lContext.setGenotypeIndex(i);
    const unsigned int lTreeDepth =
      lContext.getSystem().getRandomizer().rollInteger(lMinDepth,lMaxDepth);
    lContext.emptyCallStack();
    lIndividual[i]->resize(0);
    initTree(*lIndividual[i], lMinDepth, lTreeDepth, lContext);
  }

  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "initialization", "Beagle::GP::InitializationOp",
    std::string("Initialized individual: ")+lIndividual.serialize()
  );

  lContext.setGenotypeIndex(lOldTreeIndex);
  lContext.setGenotypeHandle(lOldTreeHandle);
  Beagle_StackTraceEndM("void GP::InitializationOp::initIndividual(Beagle::Individual& outIndividual, Beagle::Context& ioContext)");
}

