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
 *  \file   beagle/src/SelectionOp.cpp
 *  \brief  Source code of class SelectionOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Beagle.hpp"

#include <vector>

using namespace Beagle;


/*!
 *  \brief Construct a new selection operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register. 
 *  \param inName Name of the operator.
 */
SelectionOp::SelectionOp(Beagle::string inReproProbaName, Beagle::string inName) :
  BreederOp(inName),
  mReproProbaName(inReproProbaName)
{ }


/*!
 *  \brief Apply the breeding operation on a breeding pool, returning a bred individual.
 *  \param inBreedingPool Breeding pool to use for the breeding operation.
 *  \param inChild Node handle associated to child node in the breeder tree.
 *  \param ioContext Evolutionary context of the breeding operation.
 *  \return Bred individual.
 */
Individual::Handle SelectionOp::breed(Individual::Bag& inBreedingPool,
                                      BreederNode::Handle inChild,
                                      Context& ioContext)
{
  Beagle_StackTraceBeginM();
  unsigned int lSelectedIndiv = selectIndividual(inBreedingPool, ioContext);
  Beagle_AssertM(lSelectedIndiv < inBreedingPool.size());
  Individual& lOriginalIndiv = *inBreedingPool[lSelectedIndiv];
  Individual::Handle lClonedIndiv =
    castHandleT<Individual>(ioContext.getDeme().getTypeAlloc()->cloneData(lOriginalIndiv));
  Beagle_NonNullPointerAssertM(lClonedIndiv);
  ioContext.setIndividualIndex(lSelectedIndiv);
  ioContext.setIndividualHandle(lClonedIndiv);
  return lClonedIndiv;
  Beagle_StackTraceEndM("Individual::Handle SelectionOp::breed(Individual::Bag& inBreedingPool, BreederNode::Handle inChild, Context& ioContext)");
}


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
float SelectionOp::getBreedingProba(BreederNode::Handle inChild)
{
  Beagle_StackTraceBeginM();
  return mReproductionProba->getWrappedValue();
  Beagle_StackTraceEndM("float SelectionOp::getBreedingProba(BreederNode::Handle inChild)");
}


/*!
 *  \brief Initialize the operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void SelectionOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  BreederOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered(mReproProbaName)) {
    mReproductionProba =
      castHandleT<Float>(ioSystem.getRegister().getEntry(mReproProbaName));
  } else {
    mReproductionProba = new Float(0.1f);
    Register::Description lDescription(
      "Reproduction probability",
      "Float",
      "0.1",
      string("Probability than an individual is reproducted as is, without modification. ")+
      string("This parameter is useful only in selection and initialization operators that ")+
      string("are composing a breeder tree.")
    );
    ioSystem.getRegister().addEntry(mReproProbaName, mReproductionProba, lDescription);
  }
  Beagle_StackTraceEndM("void SelectionOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the selection operation on all the deme.
 *  \param ioDeme Deme to which the selection operation is applied.
 *  \param ioContext Evolutionary context.
 */
void SelectionOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "selection", "Beagle::SelectionOp",
    string("Applying selection on the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );

  if(ioContext.getGeneration() == 0) {
    Beagle_LogTraceM(
      ioContext.getSystem().getLogger(),
      "selection", "Beagle::SelectionOp",
      string("Note that the reproduction probability parameter named \"") + mReproProbaName +
      string("\" is ignored when no breeder is set in the evolver, as it seems ") +
      " actually the case."
    );
  }

  std::vector< unsigned int,BEAGLE_STLALLOCATOR<unsigned int> > lIndices(ioDeme.size(), 0);
  for(unsigned int i=0; i<ioDeme.size(); i++) lIndices[selectIndividual(ioDeme,ioContext)]++;

  for(unsigned int j=0; j<ioDeme.size(); ++j) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "selection", "Beagle::SelectionOp",
      uint2ordinal(j+1)+string(" individual selected ")+
      uint2str(lIndices[j])+string(" times")
    );
  }

  unsigned int lNextEmpty  = 0;
  unsigned int lNextFilled = 0;
  while((lNextFilled < ioDeme.size()) && (lIndices[lNextFilled] <= 1)) lNextFilled++;
  while(lNextFilled < ioDeme.size()) {
    while(lIndices[lNextFilled] > 1) {
      while(lIndices[lNextEmpty] != 0) lNextEmpty++;
      ioDeme.getTypeAlloc()->copyData(*ioDeme[lNextEmpty], *ioDeme[lNextFilled]);
      lIndices[lNextFilled]--;
      lIndices[lNextEmpty]++;
    }
    while((lNextFilled < ioDeme.size()) && (lIndices[lNextFilled] <= 1)) lNextFilled++;
  }
  Beagle_StackTraceEndM("void SelectionOp::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Read a selection operator from a XML subtree.
 *  \param inIter XML iterator to use to read selection operator.
 *  \param inOpMap Operator map to use to read selection operator.
 */
void SelectionOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string mReproProbaReadName = inIter->getAttribute("repropb").c_str();
  if(mReproProbaReadName.empty() == false) mReproProbaName = mReproProbaReadName;
  Beagle_StackTraceEndM("void SelectionOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write selection operator into XML streamer.
 *  \param ioStreamer XML streamer to write selection operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void SelectionOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.insertAttribute("repropb", mReproProbaName);
  Beagle_StackTraceEndM("void SelectionOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
