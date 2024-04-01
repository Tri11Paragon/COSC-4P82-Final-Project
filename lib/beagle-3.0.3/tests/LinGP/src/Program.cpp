/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/src/Program.cpp
 *  \brief  Implementation of the type LinGP::Program.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */

#include <algorithm>

#include "beagle/GP.hpp"
#include "beagle/LinGP/Program.hpp"
#include "beagle/LinGP/Context.hpp"
#include "beagle/LinGP/System.hpp"
#include "beagle/LinGP/Instruction.hpp"
#include "beagle/IOException.hpp"
#include "beagle/ObjectException.hpp"
#include "beagle/AssertException.hpp"
#include "beagle/InternalException.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a linear GP program of the size given.
 *  \param inSize Size of the Program.
 */
LinGP::Program::Program(unsigned int inSize) :
  std::vector<Instruction::Handle>(inSize)
{ }


/*!
 *  \brief Interpret the linear GP program.
 *  \param ioContext GP evolutionary context.
 *  \throw Beagle::ObjectException When Program is empty or not in contextual individual.
 *  \throw Beagle::AssertException When the contextual individual is a NULL pointer.
 */
void LinGP::Program::interpret(LinGP::Context& ioContext)
{
  if(empty()) throw Beagle_ObjectExceptionM("Could not interpret, program is empty!");
  LinGP::Individual::Handle lIndiv = ioContext.getIndividualHandle();
  Beagle_NonNullPointerAssertM(lIndiv.getPointer());
  unsigned int lProgramIndex = 0;
  for(; lProgramIndex < lIndiv->size(); lProgramIndex++) {
    if(this == (*lIndiv)[lProgramIndex].getPointer()) break;
  }
  if(lProgramIndex == lIndiv->size())
    throw Beagle_ObjectExceptionM("Interpreted Program is not in the actual individual of the context!");

  Program::Handle lOldProgramHandle = ioContext.getGenotypeHandle();
  unsigned int lOldProgramIndex  = ioContext.getGenotypeIndex();
  ioContext.setGenotypeIndex(lProgramIndex);
  ioContext.setGenotypeHandle(Handle(this));
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "program", "Beagle::LinGP::Program",
    string("Interpreting the ")+uint2ordinal(lProgramIndex+1)+
    string(" linear program of the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
    string(" individual")
  );
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "program", "Beagle::LinGP::Program",
    string("The individual is: ")+ioContext.getIndividual().serialize()
  );
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "program", "Beagle::LinGP::Program",
    string("Executing the linear program first instruction \"")+
    (*this)[0]->getName()+"\""
  ); 
  (*this)[0]->execute(ioContext);
  ioContext.setGenotypeIndex(lOldProgramIndex);
  ioContext.setGenotypeHandle(lOldProgramHandle);
}


/*!
 *  \brief Compare the equality of two linear GP programs.
 *  \param inRightObj Right program to be compare to tha actual one.
 *  \return True if the programs are identical, false if not.
 */
bool LinGP::Program::isEqual(const Object& inRightObj) const
{
  const LinGP::Program& lRightProgram = castObjectT<const LinGP::Program&>(inRightObj);
  if(size() != lRightProgram.size()) return false;
  return std::equal(begin(), end(), lRightProgram.begin());
}


/*!
 *  \brief Read a linear GP program from an XML node.
 *  \param inIter XML node to use to read the program from.
 *  \param ioContext Context to use to read the program.
 */
void LinGP::Program::readWithContext(PACC::XML::ConstIterator inIter, Beagle::Context& ioContext)
{
  if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Genotype"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Genotype> expected!");

  string lType = inIter->getAttribute("type");
  if(lType.empty())
    throw Beagle_IOExceptionNodeM(*inIter, "type of the genotype is not present!");
  if(lType != "gplinear")
    throw Beagle_IOExceptionNodeM(*inIter, "type of genotype mismatch, expected \"gplinear\"!");

  LinGP::Context& lLinGPContext = castObjectT<LinGP::Context&>(ioContext);
  LinGP::InstructionSet::Handle lInstructionSet =
    lLinGPContext.getSystem().getInstructionSuperSet()[ioContext.getGenotypeIndex()];

  clear();
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() != PACC::XML::eData)
      throw Beagle_IOExceptionNodeM(*lChild, "tag expected!");

    LinGP::Instruction::Handle lInstruction =
      lInstructionSet->getInstructionByName(lChild->getValue());
    if(lInstruction == NULL) {
      std::ostringstream lOSS;
      lOSS << "no instruction named \"" <<  lChild->getValue();
      lOSS << "\" found in the instruction set";
      throw Beagle_IOExceptionNodeM(*lChild, lOSS.str());
    }
    push_back(lInstruction->giveReference(lLinGPContext));
    back()->read(lChild);
  }
}


/*!
 *  \brief Write a GP Program into a XML streamer.
 *  \param ioStreamer XML streamer to write the program into.
 *  \param inIndent Whether XML output should be indented.
 */
void LinGP::Program::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  ioStreamer.openTag("Genotype", inIndent);
  ioStreamer.insertAttribute("type", "gplinear");
  ioStreamer.insertAttribute("size", uint2str(size()));
  for(unsigned int i=0; i<size(); ++i) (*this)[i]->write(ioStreamer, inIndent);
  ioStreamer.closeTag();
}



