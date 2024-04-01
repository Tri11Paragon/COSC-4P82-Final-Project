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
 *  \file   beagle/src/MilestoneReadOp.cpp
 *  \brief  Source code of class MilestoneReadOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.19.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <fstream>

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;


/*!
 *  \brief Construct a reprise checking operator.
 *  \param inName Name of the operator.
 */
MilestoneReadOp::MilestoneReadOp(Beagle::string inName) :
  Operator(inName),
  mMilestoneName(NULL)
{ }


/*!
 *  \brief Initialize the reprise checking operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void MilestoneReadOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ms.restart.file")) {
    mMilestoneName =
      castHandleT<String>(ioSystem.getRegister().getEntry("ms.restart.file"));
  } else {
    mMilestoneName = new String("");
    string lMessage = "Name of the milestone file from which the evolution should ";
    lMessage += "be restarted. An empty string means no restart.";
    Register::Description lDescription(
      "Milestone restart filename",
      "String",
      "\"\"",
      lMessage
    );
    ioSystem.getRegister().addEntry("ms.restart.file", mMilestoneName, lDescription);
  }
  Beagle_StackTraceEndM("void MilestoneReadOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the read milestone operation.
 *  \param ioDeme Actual deme of the evolution.
 *  \param ioContext Context of the evolution.
 */
void MilestoneReadOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(mMilestoneName->getWrappedValue().empty()) return;
  readMilestone(mMilestoneName->getWrappedValue(), ioContext);
  if(ioContext.getDemeIndex() == (ioContext.getVivarium().size()-1)) {
    ioContext.setGeneration(ioContext.getGeneration()+1);
    ioContext.setDemeIndex(0);
  }
  else ioContext.setDemeIndex(ioContext.getDemeIndex()+1);
  Beagle_StackTraceEndM("void MilestoneReadOp::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Read an evolution milestone.
 *  \param inFilename  Filename of the milestone.
 *  \param ioContext  Context of the evolution.
 *  \param inCommands Instructions for which sections to parse (see MilestoneReadOp::ParseCommands).
 *  \throw IOException If a read error occured.
 */
void MilestoneReadOp::readMilestone(Beagle::string inFilename, Context& ioContext, int inCommands)
{
  Beagle_StackTraceBeginM();
  Beagle_LogBasicM(
    ioContext.getSystem().getLogger(),
    "milestone", "Beagle::MilestoneReadOp",
    string("Reading milestone file \"")+
    inFilename+string("\"")
  );
  PACC::XML::Document lParser;
  try {
#ifdef BEAGLE_HAVE_LIBZ
    igzstream lIGZStream(inFilename.c_str());
    if(!lIGZStream) {
      throw Beagle_RunTimeExceptionM(string("Error!  In MilestoneReadOp::readMilestone(): Could not open milestone file '")+
        inFilename+string("' for reading. Check that the file exists."));
    }
    lParser.parse(lIGZStream, inFilename.c_str());
#else // BEAGLE_HAVE_LIBZ
    std::ifstream lIFStream(inFilename.c_str());
    if(!lIFStream) {
      throw Beagle_RunTimeExceptionM(string("Error!  In MilestoneReadOp::readMilestone(): Could not open milestone file '")+
        inFilename+string("' for reading. Check that the file exists."));
    }
    lParser.parse(lIFStream, inFilename.c_str());
#endif // BEAGLE_HAVE_LIBZ
  } catch(IOException& inExcept) {
    std::ostringstream lOSS;
    lOSS << "the evolution milestone file given is invalid: " << inExcept.what();
    throw Beagle_IOExceptionMessageM(lOSS.str().c_str());
  }
  for(PACC::XML::ConstIterator lNode=lParser.getFirstRoot(); lNode; ++lNode) {
    if((lNode->getType()==PACC::XML::eData) && (lNode->getValue()=="Beagle")) {
      string lGenerationStr = lNode->getAttribute("generation").c_str();
      if(lGenerationStr.empty()==false) ioContext.setGeneration(str2uint(lGenerationStr));
      string lDemeStr = lNode->getAttribute("deme").c_str();
      if(lDemeStr.empty()==false) ioContext.setDemeIndex(str2uint(lDemeStr));
      for(PACC::XML::ConstIterator lChild=lNode->getFirstChild(); lChild; ++lChild) {
        if(lChild->getType() == PACC::XML::eData) {
          // Read milestone part
          if(lChild->getValue() == "Milestone") {
            if(!(inCommands & eParseMilestone)) {
              Beagle_LogTraceM(
                ioContext.getSystem().getLogger(),
                "milestone", "Beagle::MilestoneReadOp",
                "Skipping milestone part of milestone as requested"
              );
            }
            else {
              for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
                if(lChild2->getType() == PACC::XML::eData) {
                  if(lChild2->getValue() == "Generation") {
                    if(lChild2->getFirstChild()->getType() != PACC::XML::eString) {
                      throw Beagle_IOExceptionNodeM(*lChild2, "generation value is not present!");
                    }
                    ioContext.setGeneration(str2uint(lChild2->getFirstChild()->getValue().c_str()));
                  }
                  else if(lChild2->getValue() == "Deme") {
                    if(lChild2->getFirstChild()->getType() != PACC::XML::eString) {
                      throw Beagle_IOExceptionNodeM(*lChild2, "deme index is not present!");
                    }
                    ioContext.setDemeIndex(str2uint(lChild2->getFirstChild()->getValue().c_str()));
                  }
                }
              }
            }
          }
          // Read system part
          else if(lChild->getValue() == "System") {
            if(!(inCommands & eParseSystem)) {
              Beagle_LogTraceM(
                ioContext.getSystem().getLogger(),
                "milestone", "Beagle::MilestoneReadOp",
                "Skipping system part of milestone as requested"
              );
            }
            else {
              Beagle_LogTraceM(
                ioContext.getSystem().getLogger(),
                "milestone", "Beagle::MilestoneReadOp",
                "Reading system part of milestone"
              );
              ioContext.getSystemHandle()->read(lChild);
            }
          }
          // Read evolver part
          else if(lChild->getValue() == "Evolver") {
            Evolver::Handle lEvolver = ioContext.getEvolverHandle();
            if(!(inCommands & eParseEvolver)) {
              Beagle_LogTraceM(
                ioContext.getSystem().getLogger(),
                "milestone", "Beagle::MilestoneReadOp",
                "Skipping evolver part of milestone as requested"
              );
            }
            else if(lEvolver == NULL) {
              Beagle_LogTraceM(
                ioContext.getSystem().getLogger(),
                "milestone", "Beagle::MilestoneReadOp",
                "Skipping the evolver part of the milestone because no Evolver was detected in the Context."
              );
            }
            else {
              Beagle_LogTraceM(
                ioContext.getSystem().getLogger(),
                "milestone", "Beagle::MilestoneReadOp",
                "Reading evolver part of milestone"
              );
              lEvolver->read(lChild);
            }
          }
          // Read register part
          else if(lChild->getValue() == "Register") {
            if(!(inCommands & eParseRegister)) {
              Beagle_LogTraceM(
                ioContext.getSystem().getLogger(),
                "milestone", "Beagle::MilestoneReadOp",
                "Skipping register part of milestone as requested"
              );
            }
            else {
              Beagle_LogTraceM(
                ioContext.getSystem().getLogger(),
                "milestone", "Beagle::MilestoneReadOp",
                "Reading register part of milestone"
              );
              ioContext.getSystem().getRegister().readWithSystem(lChild, ioContext.getSystem());
            }
          }
          // Read vivarium part
          else if(lChild->getValue() == "Vivarium") {
            if(!(inCommands & eParseVivarium)) {
              Beagle_LogTraceM(
                ioContext.getSystem().getLogger(),
                "milestone", "Beagle::MilestoneReadOp",
                "Skipping vivarium part of milestone as requested"
              );
            }
            else {
              Beagle_LogTraceM(
                ioContext.getSystem().getLogger(),
                "milestone", "Beagle::MilestoneReadOp",
                "Reading population part of milestone"
              );
              ioContext.getVivarium().readWithContext(lChild, ioContext);
            }
          }
        }
      }
    }
  }
  Beagle_StackTraceEndM("void MilestoneReadOp::readMilestone(string inFilename, Context& ioContext, int inCommands)");
}

