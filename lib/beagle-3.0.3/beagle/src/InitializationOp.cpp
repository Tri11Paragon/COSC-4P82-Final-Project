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
 *  \file   beagle/src/InitializationOp.cpp
 *  \brief  Source code of class InitializationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.18.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;


/*!
 *  \brief Construct a plain new initialization operator.
 *  \param inReproProbaName Reproduction probability parameter name used in register. 
 *  \param inName Name of the initialization operator.
 */
InitializationOp::InitializationOp(Beagle::string inReproProbaName, Beagle::string inName) :
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
Individual::Handle InitializationOp::breed(Individual::Bag& inBreedingPool,
                                           BreederNode::Handle inChild,
                                           Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Individual::Handle lNewIndiv =
    castHandleT<Individual>(ioContext.getDeme().getTypeAlloc()->allocate());
  initIndividual(*lNewIndiv, ioContext);
  if(lNewIndiv->getFitness() != NULL) lNewIndiv->getFitness()->setInvalid();
  ioContext.setIndividualHandle(lNewIndiv);
  return lNewIndiv;
  Beagle_StackTraceEndM("Individual::Handle InitializationOp::breed(Individual::Bag& inBreedingPool, BreederNode::Handle inChild, Context& ioContext)");
}


/*!
 *  \return Return selection probability of breeder operator.
 *  \param inChild Child node in the breeder tree.
 */
float InitializationOp::getBreedingProba(BreederNode::Handle inChild)
{
  Beagle_StackTraceBeginM();
  return mReproductionProba->getWrappedValue();
  Beagle_StackTraceEndM("float InitializationOp::getBreedingProba(BreederNode::Handle inChild)");
}


/*!
 *  \brief Initialize the initialization operator.
 *  \param ioSystem Reference to the system to use for the initialization.
 */
void InitializationOp::initialize(System& ioSystem)
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
      string("Probability that an individual is reproducted as is, without modification. ")+
      string("This parameter is useful only in selection and initialization operators that ")+
      string("are composing a breeder tree.")
    );
    ioSystem.getRegister().addEntry(mReproProbaName, mReproductionProba, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ec.pop.size")) {
    mPopSize = castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
  } else {
    mPopSize = new UIntArray(1,100);
    string lLongDescrip("Number of demes and size of each deme of the population. ");
    lLongDescrip += "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
    lLongDescrip += "The size of the UIntArray is the number of demes present in the ";
    lLongDescrip += "vivarium, while each value of the vector is the size of the corresponding ";
    lLongDescrip += "deme.";
    Register::Description lDescription(
      "Vivarium and demes sizes",
      "UIntArray",
      "100",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("ec.init.seedsfile")) {
    mSeedsFile =
      castHandleT<String>(ioSystem.getRegister().getEntry("ec.init.seedsfile"));
  } else {
    mSeedsFile = new String("");
    string lMessage = "Name of file to use for seeding the evolution ";
    lMessage += "with crafted individual. An empty string means no seeding.";
    Register::Description lDescription(
      "Seeds individual filename",
      "String",
      "\"\"",
      lMessage
    );
    ioSystem.getRegister().addEntry("ec.init.seedsfile", mSeedsFile, lDescription);
  }
  Beagle_StackTraceEndM("void InitializationOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the initialization operation on the deme.
 *  \param ioDeme Current deme of individuals to initialize.
 *  \param ioContext Context of the evolution.
 */
void InitializationOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  if(mPopSize == NULL) {
    string lMessage;
    lMessage = string("It appears that the operator \"")+getName()+
    string("\" has not been initialized properly because the population's size has not been ")+
    string("initialized in InitializationOp (a base class of operator \"")+getName()+
    string("\"). Consider calling the initialize() method of your initialization operator. ")+
    string("If that is already being called then you might also like to consider whether a ")+
    string("call to Beagle::InitializationOp::initialize() should be added to the operator's ")+
    string("initialize() method.");
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  if(ioContext.getVivariumHandle()!=NULL) {
    Beagle_AssertM(mPopSize->size() == ioContext.getVivarium().size());
  }
#endif // BEAGLE_NDEBUG

  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "initialization", "Beagle::InitializationOp",
    string("Initializing the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "initialization", "Beagle::InitializationOp",
    string("Resizing the deme to ")+
    uint2str((*mPopSize)[ioContext.getDemeIndex()])+" individuals"
  );
  ioDeme.resize((*mPopSize)[ioContext.getDemeIndex()]);

  unsigned int lSeededIndividuals = 0;
  if(mSeedsFile->getWrappedValue().empty() == false) {
    Beagle_LogInfoM(
      ioContext.getSystem().getLogger(),
      "initialization", "Beagle::InitializationOp",
      string("Reading seeds file \"")+mSeedsFile->getWrappedValue()+
      string("\" to initialize the ")+uint2ordinal(ioContext.getDemeIndex()+1)+
      string(" deme")
    ); 
    lSeededIndividuals = readSeeds(mSeedsFile->getWrappedValue(), ioDeme, ioContext);
    Beagle_LogDetailedM(
      ioContext.getSystem().getLogger(),
      "initialization", "Beagle::InitializationOp",
      uint2str(lSeededIndividuals)+string(" individuals read to seed the deme")
    ); 
  }

  Individual::Handle lOldIndividualHandle = ioContext.getIndividualHandle();
  unsigned int lOldIndividualIndex = ioContext.getIndividualIndex();
  for(unsigned int i=lSeededIndividuals; i<ioDeme.size(); ++i) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "initialization", "Beagle::InitializationOp",
      string("Initializing the ")+uint2ordinal(i+1)+" individual"
    );
    ioContext.setIndividualHandle(ioDeme[i]);
    ioContext.setIndividualIndex(i);
    initIndividual(*ioDeme[i], ioContext);
    if(ioDeme[i]->getFitness() != NULL) ioDeme[i]->getFitness()->setInvalid();
  }
  ioContext.setIndividualIndex(lOldIndividualIndex);
  ioContext.setIndividualHandle(lOldIndividualHandle);
  Beagle_StackTraceEndM("void InitializationOp::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Read individuals from a given file to seed the evolution.
 *  \param inFileName File name to read seeds individual from.
 *  \param ioDeme Deme to initialize with seeds.
 *  \param ioContext Evolutionary context.
 *  \return Number of seeds read.
 *
 *  Seeds file format is quite simple: XML file with "Beagle" root tag, and then a
 *  "Seeds" tag with in it the individuals representation to read. Here is a
 *  seeds file example with one individual in it.
 *  \verbatim
    <?xml version="1.0" encoding="ISO-8859-1"?>
    <Beagle>
      <Seeds>
        <Individual>
          <Genotype type="bitstring">11111</Genotype>
        </Individual>
      </Seeds>
    </Beagle>
    \endverbatim
 *  If there is less individuals in the seed file than the population size, the
 *  remaining individuals are initialized as usual. If there is more individuals
 *  than needed, the last individuals of the seeds file are ignored. If more than one
 *  deme is used in the evolution, each deme will be seeded with the same seeds file.
 *
 */
unsigned int InitializationOp::readSeeds(Beagle::string inFileName, 
										 Deme& ioDeme, 
										 Context& ioContext)
{
  Beagle_StackTraceBeginM();
#ifdef BEAGLE_HAVE_LIBZ
  igzstream lIFStream(inFileName.c_str());
#else // BEAGLE_HAVE_LIBZ
  std::ifstream lIFStream(inFileName.c_str());
#endif // BEAGLE_HAVE_LIBZ
  PACC::XML::Document lParser;
  try {
    lParser.parse(lIFStream, inFileName.c_str());
  } catch(IOException& inExcept) {
    std::ostringstream lOSS;
    lOSS << "The seeds file is invalid: " << inExcept.what();
    throw Beagle_IOExceptionMessageM(lOSS.str().c_str());
  }

  unsigned int lReadIndividuals=0;
  unsigned int lOldIndivIndex = ioContext.getIndividualIndex();
  Individual::Handle lOldIndivHandle = ioContext.getIndividualHandle();
  for(PACC::XML::ConstIterator lNode=lParser.getFirstRoot(); lNode; ++lNode) {
    if((lNode->getType()==PACC::XML::eData) && (lNode->getValue()=="Beagle")) {
      for(PACC::XML::ConstIterator lChild=lNode->getFirstChild(); lChild; ++lChild) {
        if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Seeds")) {
          for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
            if((lChild2->getType()==PACC::XML::eData) && (lChild2->getValue()=="Individual")) {
              if(lReadIndividuals >= ioDeme.size()) break;
              Beagle_NonNullPointerAssertM(ioDeme[lReadIndividuals]);
              ioContext.setIndividualIndex(lReadIndividuals);
              ioContext.setIndividualHandle(ioDeme[lReadIndividuals]);
              Beagle_LogVerboseM(
                ioContext.getSystem().getLogger(),
                "initialization", "Beagle::InitializationOp",
                string("Reading the ")+uint2ordinal(lReadIndividuals+1)+
                string(" individual from seeds file")
              );
              ioDeme[lReadIndividuals]->readWithContext(lChild2, ioContext);
              Beagle_LogDebugM(
                ioContext.getSystem().getLogger(),
                "initialization", "Beagle::InitializationOp",
                string("Read seeded individual: ")+ioDeme[lReadIndividuals]->serialize()
              );
              ++lReadIndividuals;
            }
          }
        }
      }
    }
  }
  ioContext.setIndividualHandle(lOldIndivHandle);
  ioContext.setIndividualIndex(lOldIndivIndex);
  return lReadIndividuals;
  Beagle_StackTraceEndM("unsigned int InitializationOp::readSeeds(string inFileName, Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Read a initialization operator from a XML subtree.
 *  \param inIter XML iterator to use to read initialization operator.
 *  \param inOpMap Operator map to use to read initialization operator.
 */
void InitializationOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string mReproProbaReadName = inIter->getAttribute("repropb").c_str();
  if(mReproProbaReadName.empty() == false) mReproProbaName = mReproProbaReadName;
  Beagle_StackTraceEndM("void InitializationOp::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write initialization operator into XML streamer.
 *  \param ioStreamer XML streamer to write initialization operator into.
 *  \param inIndent Whether XML output should be indented.
 */
void InitializationOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.insertAttribute("repropb", mReproProbaName);
  Beagle_StackTraceEndM("void InitializationOp::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
