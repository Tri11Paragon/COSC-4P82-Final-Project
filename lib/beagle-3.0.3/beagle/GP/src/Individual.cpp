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
 *  \file   beagle/GP/src/Individual.cpp
 *  \brief  Source code of class GP::Individual.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20.2.1 $
 *  $Date: 2007/05/09 01:51:06 $
 */

#include "beagle/GP.hpp"
#include "beagle/RouletteT.hpp"

using namespace Beagle;


/*!
 *  \brief Contruct a GP individual.
 *  \par Note:
 *    Use GP::Tree allocator to allocate the genotypes.
 */
GP::Individual::Individual() :
  Beagle::Individual(new GP::Tree::Alloc)
{ }


/*!
 *  \brief Contruct a GP individual from a GP tree allocator.
 *  \param inGenotypeAlloc Allocator of GP trees.
 */
GP::Individual::Individual(GP::Tree::Alloc::Handle inGenotypeAlloc) :
  Beagle::Individual(inGenotypeAlloc)
{ }


/*!
 *  \brief Contruct a GP individual from a genotype allocator and a fitness allocator.
 *  \param inGenotypeAlloc Allocator of GP trees.
 *  \param inFitnessAlloc Fitness allocator.
 *  \param inN Number of genotypes of the GP individual.
 */
GP::Individual::Individual(GP::Tree::Alloc::Handle inGenotypeAlloc,
                           Fitness::Alloc::Handle inFitnessAlloc,
                           size_type inN) :
  Beagle::Individual(inGenotypeAlloc, inFitnessAlloc, inN)
{ }


/*!
 *  \brief Randomly select a tree from the individual.  The
 *    selection is biased towards trees with a greater number of nodes
 *    (i.e. Each node has an equal probability of being selected.
 *  \return Randomly selected tree
 */
unsigned int GP::Individual::chooseRandomTree(Beagle::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  unsigned int lChosenNode = ioContext.getSystem().getRandomizer().rollInteger(0, getTotalNodes()-1);
  unsigned int lChosenTree = 0;
  for(; (lChosenTree+1)<size(); lChosenTree++) {
    if(lChosenNode < (*this)[lChosenTree]->size()) break;
    else lChosenNode -= (*this)[lChosenTree]->size();
  }
  return lChosenTree;
  Beagle_StackTraceEndM("unsigned int GP::Individual::chooseRandomTree(Beagle::Context& ioContext) const");
}


/*!
 *  \brief Randomly select a node from a specific tree in the
 *    individual.  Each node has an equal probability of being selected.
 *  \return Randomly selected tree
 */
unsigned int GP::Individual::chooseRandomNode(unsigned int inTree, Beagle::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Beagle_AssertM(operator[](inTree)->size()!=0);
  return ioContext.getSystem().getRandomizer().rollInteger(0, operator[](inTree)->size()-1);
  Beagle_StackTraceEndM("unsigned int GP::Individual::chooseRandomNode(unsigned int inTree, Beagle::Context& ioContext) const");
}


/*!
 *  \brief Randomly select a node that takes arguments from a specific
 *    tree in the individual.
 *  \return Randomly selected tree
 */
unsigned int GP::Individual::chooseRandomNodeWithArgs(unsigned int inTree, 
                                                      Beagle::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  const GP::Tree& lTree = *(operator[](inTree));
  unsigned int lSize = lTree.size();
  if (lSize < 2) {
    throw Beagle_RunTimeExceptionM(string("In GP::Individual::chooseRandomNodeWithArgs(): ")+
      string("Could not choose a node with arguments because the specified tree has fewer ")+
      string("than two (")+uint2str(lSize)+string(") nodes, hence there are no such nodes")+
      string(" in the tree. This occurred while calling chooseRandomNodeWithArgs() with an ")+
      string("inTree value of ")+uint2str(inTree));
  }

  // Loop through the tree adding appropriate nodes into the roulette
  RouletteT<unsigned int> lRoulette;
  for (unsigned int i=0; i<lSize; i++) {
    if(lTree[i].mSubTreeSize > 1) lRoulette.insert(i);
  }

  // Select node with roulette
  Beagle_AssertM(!lRoulette.empty());
  return lRoulette.select(ioContext.getSystem().getRandomizer());
  Beagle_StackTraceEndM("unsigned int GP::Individual::chooseRandomNodeWithArgs(unsigned int inTree, Beagle::Context& ioContext) const");
}


/*!
 *  \brief Randomly select a node that takes no argument from a specific
 *    tree in the individual.
 *  \return Randomly selected tree
 */
unsigned int GP::Individual::chooseRandomNodeWithoutArgs(unsigned int inTree, 
                                                         Beagle::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  // Loop through the tree adding appropriate nodes into the roulette
  unsigned int lSize = operator[](inTree)->size();
  RouletteT<unsigned int> lRoulette;
  for (unsigned int i=0; i<lSize; i++) {
    if (operator[](inTree)->operator[](i).mSubTreeSize == 1) {
      lRoulette.insert(i);
    }
  }

  // Select node with roulette
  Beagle_AssertM(!lRoulette.empty());
  return lRoulette.select(ioContext.getSystem().getRandomizer());
  Beagle_StackTraceEndM("unsigned int GP::Individual::chooseRandomNodeWithoutArgs(unsigned int inTree, Beagle::Context& ioContext) const");
}


/*!
 *  \brief Interpret the GP individual.
 *  \param outResult Datum containing the result of the interpretation.
 *  \param ioContext GP evolutionary context.
 *  \throw Beagle::ObjectException When individual or tree are empty.
 *  \throw Beagle::AssertException When context is not correctly set.
 *  \throw Beagle::GP::MaxNodesExecutionException If number of nodes execution is more than allowed.
 *  \throw Beagle::GP::MaxTimeExecutionException If elapsed execution time is more than allowed.
 */
void GP::Individual::run(GP::Datum& outResult, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(&ioContext.getIndividual() != this) {
    throw Beagle_RunTimeExceptionM(string("In GP::Individual::run():  For the given context, ")+
      string("getIndividual() did not return the same individual as was passed to this (run) ")+
      string("method. Consider setting the context's individual to be the same by using the ")+
      string("method Context::setIndividualHandle()."));
  }
  if(empty()) throw Beagle_ObjectExceptionM("Could not interpret, individual has no trees!");
  if((*this)[0]->empty()) throw Beagle_ObjectExceptionM("Could not interpret, 1st tree is empty!");
  Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
  unsigned int lOldTreeIndex  = ioContext.getGenotypeIndex();
  ioContext.setGenotypeIndex(0);
  ioContext.setGenotypeHandle((*this)[0]);
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "individual", "Beagle::GP::Individual",
    string("Running the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
    string(" individual")
  );
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "individual", "Beagle::GP::Individual",
    string("The individual is: ")+serialize()
  );
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "individual", "Beagle::GP::Individual",
    string("Executing the first tree root node \"")+
    (*(*this)[0])[0].mPrimitive->getName()+"\""
  );
  ioContext.setNodesExecutionCount(0);
  ioContext.incrementNodesExecuted();
  ioContext.getExecutionTimer().reset(); 
  ioContext.pushCallStack(0);  
  (*(*this)[0])[0].mPrimitive->execute(outResult, ioContext);
  ioContext.popCallStack();
  ioContext.checkExecutionTime();
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "individual", "Beagle::GP::Individual",
    string("Result of executing the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
    string(" individual: ")+outResult.serialize()
  );
  ioContext.setGenotypeIndex(lOldTreeIndex);
  ioContext.setGenotypeHandle(lOldTreeHandle);
  Beagle_StackTraceEndM("void GP::Individual::run(GP::Datum& outResult, GP::Context& ioContext)");
}


/*!
 *  \brief Validate the GP individual.
 *  \param ioContext Evolutionary context.
 *  \return True if the GP individual is valid, false if not. 
 */
bool GP::Individual::validate(GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  bool lResult = true;

  Beagle_LogDetailedM(
    ioContext.getSystem().getLogger(),
    "individual", "Beagle::GP::Individual",
    string("Validating ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
    string(" individual")
  );

  // Store original values.
  GP::Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
  unsigned int lOldTreeIndex = ioContext.getGenotypeIndex();

  // Loop through each of the trees in the individual
  for (unsigned int i=0; i<size(); i++) {
    GP::Tree::Handle lTree = (*this)[i];
    if (lTree == NULL) {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "individual", "Beagle::GP::Individual",
        string("Skipping ")+uint2ordinal(i+1)+string(" tree because it's NULL-valued")
      );
      continue;
    }

    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "individual", "Beagle::GP::Individual",
      string("Validating ")+uint2ordinal(i+1)+string(" tree")
    );

    // Store the new values
    ioContext.setGenotypeHandle(lTree);
    ioContext.setGenotypeIndex(i);

    lTree->setContextToNode(0, ioContext);
    if(!lTree->validateSubTree(0, ioContext)) {
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "individual", "Beagle::GP::Individual",
        string("Validation of ")+uint2ordinal(i+1)+string(" tree failed.")
      );
      lResult = false;
      break;
    }
  }

  if(lResult) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "individual", "Beagle::GP::Individual",
      string("Individual passed validation testing.")
    );
  }

  // Restore the original values.
  ioContext.setGenotypeHandle(lOldTreeHandle);
  ioContext.setGenotypeIndex(lOldTreeIndex);

  return lResult;
  Beagle_StackTraceEndM("bool GP::Individual::validate(GP::Context& ioContext)");
}
