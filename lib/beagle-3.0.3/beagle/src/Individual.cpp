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
 *  \file   beagle/src/Individual.cpp
 *  \brief  Source code of class Individual.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.20.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct an individual from a genotype allocator.
 *  \param inGenotypeAlloc Allocator of genotypes.
 */
Individual::Individual(Genotype::Alloc::Handle inGenotypeAlloc) :
  Genotype::Bag(inGenotypeAlloc),
  mFitnessAlloc(new FitnessSimple::Alloc),
  mFitness(new FitnessSimple)
{ }


/*!
 *  \brief Construct an individual.
 *  \param inGenotypeAlloc Allocator of genotypes.
 *  \param inFitnessAlloc Fitness object allocator.
 *  \param inN Number of genotypes of the individual.
 */
Individual::Individual(Genotype::Alloc::Handle inGenotypeAlloc,
                       Fitness::Alloc::Handle inFitnessAlloc,
                       unsigned int inN) :
  Genotype::Bag(inGenotypeAlloc, inN),
  mFitnessAlloc(inFitnessAlloc),
  mFitness(castObjectT<Fitness*>(inFitnessAlloc->allocate()))
{ }


/*!
 *  \brief Copy construct an individual.
 *  \param inRightIndividual Original individual to use.
 */
Individual::Individual(const Individual& inRightIndividual) :
  Genotype::Bag(inRightIndividual),
  mFitnessAlloc(inRightIndividual.mFitnessAlloc),
  mFitness(castObjectT<Fitness*>(
    inRightIndividual.mFitnessAlloc->clone(*inRightIndividual.mFitness)))
{ }


/*!
 *  \brief Copy an individual.
 *  \param inRightIndividual Original individual to copy.
 *  \return Actual individual.
 */
Individual& Individual::operator=(const Individual& inRightIndividual)
{
  Beagle_StackTraceBeginM();
  if(&inRightIndividual == this) return *this;
  Genotype::Bag::operator=(inRightIndividual);
  mFitnessAlloc = inRightIndividual.mFitnessAlloc;
  try {
    mFitness = castObjectT<Fitness*>(mFitnessAlloc->clone(*inRightIndividual.mFitness));
  } catch(BadCastException& inException) {
    std::ostringstream lMessage;
    lMessage << "Bad cast exception catched when copying an individual over another: \"";
    lMessage << inException.what() << "\". This may be caused by the fact that fitness ";
    lMessage << "allocator of the actual individual doesn't match the fitness type ";
    lMessage << "of the individual to copy. Be sure that you configured the good ";
    lMessage << "fitness allocator in the population allocators, as described in sections ";
    lMessage << "3.4.6 and 4.2 of the user manual.";
    throw Beagle_RunTimeExceptionM(lMessage.str().c_str());
  }
  return *this;
  Beagle_StackTraceEndM("Individual& Individual::operator=(const Individual& inRightIndividual)");
}


/*!
 *  \brief Copy an individual and its data (genotype and fitness)
 *  \param inOrigContainer Original individual to copy.
 *  \throw BasCastException If the types of the original individual are not as specified.
 */
void Individual::copyData(const Container& inOrigContainer)
{
  Beagle_StackTraceBeginM();
  const Individual& lOrigIndividual = castObjectT<const Individual&>(inOrigContainer);
  if(lOrigIndividual.getTypeAlloc() == NULL) {
    string lMessage = "The copyData() method must be call only with as argument a individual";
    lMessage += " that have a type allocator!";
    throw Beagle_InternalExceptionM(lMessage);
  }
  if(this == &lOrigIndividual) return;
  (*this) = lOrigIndividual;
  clear();
  for(unsigned int i=0; i<lOrigIndividual.size(); ++i) {
    Beagle_NonNullPointerAssertM(lOrigIndividual[i]);
    push_back(getTypeAlloc()->clone(*lOrigIndividual[i]));
  }
  Beagle_StackTraceEndM("void Individual::copyData(const Container& inOrigContainer)");
}


/*!
 *  \brief Computing two individuals distances in genotype space is actually not implemented.
 */
double Individual::getDistanceGenotype(const Individual&) const
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("getDistanceGenotype","Individual",getName());
  return 0.0;
  Beagle_StackTraceEndM("double Individual::getDistanceGenotype(const Individual&) const");
}


/*!
 *  \brief Computing two individuals distances in phenotype space is actually not implemented.
 */
double Individual::getDistancePhenotype(const Individual&) const
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("getDistancePhenotype","Individual",getName());
  return 0.0;
  Beagle_StackTraceEndM("double Individual::getDistancePhenotype(const Individual&) const");
}


/*!
 *  \brief Get size (length) of the individual, summing size of genotypes.
 *  \return Total size of individual.
 */
unsigned int Individual::getSize() const
{
  Beagle_StackTraceBeginM();
  unsigned int lSize=0;
  for(unsigned int i=0; i<size(); ++i) lSize += (*this)[i]->getSize();
  return lSize;
  Beagle_StackTraceEndM("unsigned int Individual::getSize() const");
}


/*!
 *  \brief  Compare the fitness of two individual.
 *  \param  inRightObj Individual to compare to this.
 *  \return True if individuals have the same fitness, false if not.
 *  \throw  AssertException When a fitness handle of the individuals is NULL.
 *  \throw  BadCastException If the types of the parameters are not as specified.
 */
bool Individual::isEqual(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const Individual& lRightIndividual = castObjectT<const Individual&>(inRightObj);
  if((mFitness == NULL) && (lRightIndividual.mFitness == NULL)) return true;
  if((mFitness == NULL) || (lRightIndividual.mFitness == NULL)) return false;
  return mFitness->isEqual(*lRightIndividual.mFitness);
  Beagle_StackTraceEndM("bool Individual::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief  Compare the equality of the genotypes of two individuals.
 *  \param  inRightIndividual Individual to compare to this.
 *  \return True if individuals are identicals, false if not.
 */
bool Individual::isIdentical(const Individual& inRightIndividual) const
{
  Beagle_StackTraceBeginM();
  if(isEqual(inRightIndividual) == false) return false;
  if(size() != inRightIndividual.size()) return false;
  for(unsigned int i=0; i<size(); ++i) {
    if((*this)[i]->isEqual(*inRightIndividual[i]) == false) return false;
  }
  return true;
  Beagle_StackTraceEndM("bool Individual::isIdentical(const Individual& inRightIndividual) const");
}


/*!
 *  \brief  Compare the fitness order of two individuals.
 *
 *  isLess() is called when two individuals are compared with each
 *  other.  For example, if we have two Indiviudals, 'A' and 'B', then
 *  A < B calls operator<(Object, Object), which in
 *  turn calls Beagle::Individual::isLess().
 *
 *  For more information on operator<(Object, Object) and similar
 *  functions, see the "List of all members" for Beagle::Object.
 *
 *  \param  inRightObj Individual to compare to this.
 *  \return True if left individual fitness is less that the right one, false if not.
 *  \throw  AssertException When a fitness handle of the individuals is NULL.
 *  \throw  BadCastException If the types of the parameters are not as specified.
 */
bool Individual::isLess(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const Individual& lRightIndividual = castObjectT<const Individual&>(inRightObj);
  if((mFitness == NULL) || (lRightIndividual.mFitness == NULL)) return false;
  return mFitness->isLess(*lRightIndividual.mFitness);
  Beagle_StackTraceEndM("bool Individual::isLess(const Object& inRightObj) const");
}


/*!
 *  \brief Reading an individual with method read is undefined. Use readWithContext instead.
 */
void Individual::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("read","Individual",getName());
  Beagle_StackTraceEndM("void Individual::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Read individual from a XML file. If several individuals are in file, read
 *    first tagged occurence of individual.
 *  \param inFileName Filename to read individual from.
 *  \param ioSystem Evolutionary system to read individual.
 *  \return True if an individual was found and read from file, false if nothing was found.
 */
bool Individual::readFromFile(Beagle::string inFileName, System& ioSystem)
{
  Beagle_StackTraceBeginM();
  std::ifstream lIFS(inFileName.c_str());
  PACC::XML::Document lParser(lIFS, inFileName.c_str());
  lIFS.close();
  PACC::XML::ConstFinder lIndivFinder = (PACC::XML::ConstIterator)lParser.getFirstDataTag();
  PACC::XML::ConstIterator lIndivTag = lIndivFinder.find("//Individual");
  if(!lIndivTag) return false;

  Context::Handle lContext = castHandleT<Context>(ioSystem.getContextAllocator().allocate());
  lContext->setSystemHandle(&ioSystem);
  lContext->setIndividualHandle(this);
  lContext->setIndividualIndex(0);
  readWithContext(lIndivTag, *lContext);

  Beagle_LogInfoM(
    ioSystem.getLogger(),
    "individual", "Beagle::Individual",
    string("Read from file \"")+inFileName+string("\" the following individual: ")+
    serialize()
  );

  return true;
  Beagle_StackTraceEndM("bool Individual::readFromFile(string inFileName, System& ioSystem)");
}


/*!
 *  \brief Read an individual from an XML node.
 *  \param inIter XML iterator to read the individual from.
 *  \param ioContext Evolutionary context to use to read the individual.
 *  \throw Beagle::IOException If the format is not respected.
 */
void Individual::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Individual"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Individual> expected!");

  size_type lSize=0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Genotype")) ++lSize;
  }
  if((getTypeAlloc()==NULL) && (lSize>size())) {
    std::ostringstream lMessageOSS;
    lMessageOSS << "Individual size (" << lSize << ") is bigger than the actual size (";
    lMessageOSS << size() << "), and there is no type allocator for resizing!";
    throw Beagle_IOExceptionNodeM(*inIter, lMessageOSS.str().c_str());
  }
  resize(lSize);

  unsigned int lOldGenoIndex = ioContext.getGenotypeIndex();
  Genotype::Handle lOldGenoHandle = ioContext.getGenotypeHandle();
  size_type lIdx = 0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      if(lChild->getValue() == "Fitness") {
        string lValidity = lChild->getAttribute("valid").c_str();
        if(lValidity != "no") {
          Beagle_NonNullPointerAssertM(mFitness);
          mFitness->read(lChild);
        }
        else if(mFitness != NULL) mFitness->setInvalid();
      }
      else if(lChild->getValue() == "Genotype") {
        Beagle_NonNullPointerAssertM((*this)[lIdx]);
        ioContext.setGenotypeHandle((*this)[lIdx]);
        ioContext.setGenotypeIndex(lIdx);
        (*this)[lIdx]->readWithContext(lChild, ioContext);
        ++lIdx;
      }
    }
  }
  ioContext.setGenotypeHandle(lOldGenoHandle);
  ioContext.setGenotypeIndex(lOldGenoIndex);
  Beagle_StackTraceEndM("void Individual::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)");
}


/*!
 *  \brief Write an individual into a XML streamer.
 *  \param ioStreamer XML streamer to write the individual into.
 *  \param inIndent Whether XML output should be indented.
 */
void Individual::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Individual", inIndent);
  ioStreamer.insertAttribute("size", uint2str(size()));
  if((mFitness==NULL) || (mFitness->isValid()==false)) {
    ioStreamer.openTag("Fitness", false);
    ioStreamer.insertAttribute("valid", "no");
    ioStreamer.closeTag();
  }
  else mFitness->write(ioStreamer, inIndent);
  for(size_type i=0; i<size(); ++i) {
    Beagle_NonNullPointerAssertM((*this)[i]);
    (*this)[i]->write(ioStreamer, false);
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Individual::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

