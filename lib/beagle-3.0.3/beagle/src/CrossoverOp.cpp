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
 *  \file   beagle/src/CrossoverOp.cpp
 *  \brief  Source code of class CrossoverOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

#include <vector>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a plain new crossover operator.
 *  \param inMatingPbName Mating probability parameter name used in register.
 *  \param inName Name of the crossover operator.
 */
CrossoverOp::CrossoverOp(Beagle::string inMatingPbName, Beagle::string inName) :
  BreederOp(inName),
  mMatingProbaName(inMatingPbName)
{ }


/*!
 *  \brief Apply the crossover operation on a breeding pool, returning a mated individual.
 *  \param inBreedingPool Breeding pool to use for the crossover operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the crossover operation.
 *  \return Mated individual.
 */
Individual::Handle CrossoverOp::breed(Individual::Bag& inBreedingPool,
                                      BreederNode::Handle inChild,
                                      Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Context::Handle lContext2 =
    castObjectT<Context*>(ioContext.getSystem().getContextAllocator().clone(ioContext));

  Beagle_NonNullPointerAssertM(inChild);
  Beagle_NonNullPointerAssertM(inChild->getBreederOp());
  Individual::Handle lIndiv1 = inChild->getBreederOp()->breed(inBreedingPool,
                                                              inChild->getFirstChild(), 
                                                              ioContext);

  Beagle_NonNullPointerAssertM(inChild->getNextSibling());
  Beagle_NonNullPointerAssertM(inChild->getNextSibling()->getBreederOp());
  Individual::Handle lIndiv2 =
    inChild->getNextSibling()->getBreederOp()->breed(inBreedingPool,
                                                     inChild->getNextSibling()->getFirstChild(),
                                                     *lContext2);
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "crossover", "Beagle::CrossoverOp",
    string("Mating the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
    string(" individual with the ")+uint2ordinal(lContext2->getIndividualIndex()+1)+
    " individual"
  );

  if((lIndiv1 != NULL) && (lIndiv2 != NULL)) {
    bool lMated = mate(*lIndiv1, ioContext, *lIndiv2, *lContext2);
    if(lMated && (lIndiv1->getFitness() != NULL)) lIndiv1->getFitness()->setInvalid();
  }

  return lIndiv1;
  Beagle_StackTraceEndM("Individual::Handle CrossoverOp::breed(Individual::Bag& inBreedingPool, BreederNode::Handle inChild, Context& ioContext)");
}


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
float CrossoverOp::getBreedingProba(BreederNode::Handle inChild)
{
  Beagle_StackTraceBeginM();
  return mMatingProba->getWrappedValue();
  Beagle_StackTraceEndM("float CrossoverOp::getBreedingProba(BreederNode::Handle inChild)");
}


/*!
 *  \brief Initialize the crossover operator.
 *  \param ioSystem Reference to the system to use for the initialization.
 */
void CrossoverOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  BreederOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered(mMatingProbaName)) {
    mMatingProba = castHandleT<Float>(ioSystem.getRegister().getEntry(mMatingProbaName));
  } else {
    mMatingProba = new Float(0.5);
    Register::Description lDescription(
      "Individual crossover probability",
      "Float",
      "0.5",
      "Single individual crossover probability for a generation."
    );
    ioSystem.getRegister().addEntry(mMatingProbaName, mMatingProba, lDescription);
  }
  Beagle_StackTraceEndM("void CrossoverOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the crossover operation on the deme.
 *  \param ioDeme Current deme of individuals to mate.
 *  \param ioContext Context of the evolution.
 */
void CrossoverOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_ValidateParameterM(mMatingProba->getWrappedValue()>=0.0, mMatingProbaName, "<0");

  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "crossover", "Beagle::CrossoverOp",
    string("Mating individuals of the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "crossover", "Beagle::CrossoverOp",
    string("Mating individuals with probability ")+
    dbl2str(mMatingProba->getWrappedValue())
  );

  Context::Handle lContext2 =
    castObjectT<Context*>(ioContext.getSystem().getContextAllocator().clone(ioContext));
  Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
  unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();

  std::vector< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> > lMateVector;
  for(unsigned int i=0; i<ioDeme.size(); i++) {
    if(ioContext.getSystem().getRandomizer().rollUniform() <= mMatingProba->getWrappedValue()) {
      lMateVector.push_back(i);
    }
  }

  std::random_shuffle(lMateVector.begin(), lMateVector.end(),
                      ioContext.getSystem().getRandomizer());
  if((lMateVector.size() % 2) != 0) lMateVector.pop_back();

  for(unsigned int j=0; j<lMateVector.size(); j+=2) {
    unsigned int lFirstMate  = lMateVector[j];
    unsigned int lSecondMate = lMateVector[j+1];
    ioContext.setIndividualIndex(lFirstMate);
    ioContext.setIndividualHandle(ioDeme[lFirstMate]);
    lContext2->setIndividualIndex(lSecondMate);
    lContext2->setIndividualHandle(ioDeme[lSecondMate]);

    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "crossover", "Beagle::CrossoverOp",
      string("Mating the ")+uint2ordinal(lFirstMate+1)+
      string(" individual with the ")+uint2ordinal(lSecondMate+1)+" individual"
    );

    bool lMated = mate(*ioDeme[lFirstMate], ioContext, *ioDeme[lSecondMate], *lContext2);
    if(lMated && (ioDeme[lFirstMate]->getFitness() != NULL))
      ioDeme[lFirstMate]->getFitness()->setInvalid();
    if(lMated && (ioDeme[lSecondMate]->getFitness() != NULL))
      ioDeme[lSecondMate]->getFitness()->setInvalid();
  }

  ioContext.setIndividualIndex(lOldIndividualIndex);
  ioContext.setIndividualHandle(lOldIndividualHandle);
  Beagle_StackTraceEndM("void CrossoverOp::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Read a crossover operator from XML tree.
 *  \param inIter XML iterator to use to read crossover operator.
 *  \param inOpMap Operator map to use to read crossover operator.
 */
void CrossoverOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string mMatingProbaReadName = inIter->getAttribute("matingpb").c_str();
  if(mMatingProbaReadName.empty()==false) mMatingProbaName = mMatingProbaReadName;
  Beagle_StackTraceEndM("void CrossoverOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write crossover operator content into XML streamer.
 *  \param ioStreamer XML streamer to write crossover operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void CrossoverOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.insertAttribute("matingpb", mMatingProbaName);
  Beagle_StackTraceEndM("void CrossoverOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

