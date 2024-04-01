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
 *  \file   beagle/src/MutationOp.cpp
 *  \brief  Source code of class MutationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a plain new mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inName Name of the mutation operator.
 */
MutationOp::MutationOp(Beagle::string inMutationPbName,
                       Beagle::string inName) :
  BreederOp(inName),
  mMutationPbName(inMutationPbName)
{ }


/*!
 *  \brief Apply the mutation operation on a breeding pool, returning a mutated individual.
 *  \param inBreedingPool Breeding pool to use for the mutation operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the mutation operation.
 *  \return Mutated individual.
 */
Individual::Handle MutationOp::breed(Individual::Bag& inBreedingPool,
                                     BreederNode::Handle inChild,
                                     Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(inChild);
  Beagle_NonNullPointerAssertM(inChild->getBreederOp());
  Individual::Handle lIndiv =
    inChild->getBreederOp()->breed(inBreedingPool, inChild->getFirstChild(), ioContext);

  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::MutationOp",
    string("Mutating the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+" individual"
  );
  bool lMutated = mutate(*lIndiv, ioContext);
  if(lMutated && (lIndiv->getFitness() != NULL)) lIndiv->getFitness()->setInvalid();
  return lIndiv;
  Beagle_StackTraceEndM("Individual::Handle MutationOp::breed(Individual::Bag& inBreedingPool, BreederNode::Handle inChild, Context& ioContext)");
}


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
float MutationOp::getBreedingProba(BreederNode::Handle inChild)
{
  Beagle_StackTraceBeginM();
  return mMutationProba->getWrappedValue();
  Beagle_StackTraceEndM("float MutationOp::getBreedingProba(BreederNode::Handle inChild)");
}


/*!
 *  \brief Initialize the mutation operator.
 *  \param ioSystem Reference to the system to use for the initialization.
 */
void MutationOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  BreederOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    mMutationProba =
      castHandleT<Float>(ioSystem.getRegister().getEntry(mMutationPbName));
  } else {
    mMutationProba = new Float((float)0.1);
    Register::Description lDescription(
      "Individual mutation probability",
      "Float",
      "0.1",
      "Mutation probability for a single individual."
    );
    ioSystem.getRegister().addEntry(mMutationPbName, mMutationProba, lDescription);
  }
  Beagle_StackTraceEndM("void MutationOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the mutation operation on the deme.
 *  \param ioDeme Current deme of individuals to mutate.
 *  \param ioContext Context of the evolution.
 */
void MutationOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
  unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::MutationOp",
    string("Mutating individuals of the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::MutationOp",
    string("Mutating individuals with probability ")+
    dbl2str(mMutationProba->getWrappedValue())
  );

  for(unsigned int i=0; i<ioDeme.size(); i++) {
    if(ioContext.getSystem().getRandomizer().rollUniform() <= mMutationProba->getWrappedValue()) {
      ioContext.setIndividualIndex(i);
      ioContext.setIndividualHandle(ioDeme[i]);
      Beagle_LogVerboseM(
        ioContext.getSystem().getLogger(),
        "mutation", "Beagle::MutationOp",
        string("Mutating the ")+uint2ordinal(i+1)+" individual"
      );
      bool lMutated = mutate(*ioDeme[i], ioContext);
      if(lMutated && (ioDeme[i]->getFitness() != NULL))
        ioDeme[i]->getFitness()->setInvalid();
    }
  }
  ioContext.setIndividualIndex(lOldIndividualIndex);
  ioContext.setIndividualHandle(lOldIndividualHandle);
  Beagle_StackTraceEndM("void MutationOp::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Read a mutation operator from XML tree.
 *  \param inIter XML iterator to use to read crossover operator.
 *  \param inOpMap Operator map to use to read crossover operator.
 */
void MutationOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string mMutationPbReadName = inIter->getAttribute("mutationpb").c_str();
  if(mMutationPbReadName.empty() == false) mMutationPbName = mMutationPbReadName;
  Beagle_StackTraceEndM("void MutationOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write mutation operator content into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void MutationOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.insertAttribute("mutationpb", mMutationPbName);
  Beagle_StackTraceEndM("void MutationOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

