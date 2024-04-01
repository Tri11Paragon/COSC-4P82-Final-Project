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
 *  \file   beagle/src/MilestoneWriteOp.cpp
 *  \brief  Source code of class MilestoneWriteOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15.2.2 $
 *  $Date: 2007/05/11 21:49:00 $
 */

#include "beagle/Beagle.hpp"

#include <cstdio>
#include <fstream>

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;


/*!
 *  \brief Construct a milestone writer operator.
 */
MilestoneWriteOp::MilestoneWriteOp(Beagle::string inName) :
  Operator(inName),
  mMilestonePrefix(NULL),
  mWritingInterval(NULL),
  mPerDemeMilestone(NULL),
  mOverwriteMilestone(NULL),
  mPopSize(NULL)
{ }


/*!
 *  \brief Initialize the write milestone operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void MilestoneWriteOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ms.write.prefix")) {
    mMilestonePrefix =
      castHandleT<String>(ioSystem.getRegister().getEntry("ms.write.prefix"));
  } else {
    mMilestonePrefix = new String("beagle");
    string lLongDescrip = "Prefix used to name the evolution milestone files. ";
    lLongDescrip += "An empty string means no milestone.";
    Register::Description lPrefixDescription(
      "Milestone filename prefix",
      "String",
      "\"beagle\"",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ms.write.prefix", mMilestonePrefix, lPrefixDescription);
  }

  if(ioSystem.getRegister().isRegistered("ms.write.interval")) {
    mWritingInterval =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ms.write.interval"));
  } else {
    mWritingInterval = new UInt(0);
    string lLongDescrip = "Milestone saving interval (in number of generations). ";
    lLongDescrip += "When zero, only the last generation milestone is saved.";
    Register::Description lIntervalDescription(
      "Milestone saving interval",
      "UInt",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ms.write.interval", mWritingInterval, lIntervalDescription);
  }

  if(ioSystem.getRegister().isRegistered("ms.write.perdeme")) {
    mPerDemeMilestone =
      castHandleT<Bool>(ioSystem.getRegister().getEntry("ms.write.perdeme"));
  } else {
    mPerDemeMilestone = new Bool(false);
    string lLongDescrip = "If true, this flag indicates that separate milestones ";
    lLongDescrip += "should be written after each demes processing. Otherwise milestones are ";
    lLongDescrip += "written after the processing of a complete populations.";
    Register::Description lPerdemeDescription(
      "Milestone per deme saving flag",
      "Bool",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ms.write.perdeme", mPerDemeMilestone, lPerdemeDescription);
  }

  if(ioSystem.getRegister().isRegistered("ms.write.over")) {
    mOverwriteMilestone =
      castHandleT<Bool>(ioSystem.getRegister().getEntry("ms.write.over"));
  } else {
    mOverwriteMilestone = new Bool(true);
    string lLongDescrip = "If true, this flag indicates that old milestones should ";
    lLongDescrip += "be over-written. Otherwise, each milestone has a different suffix.";
    Register::Description lOverwriteDescription(
      "Milestone over-writing flag",
      "Bool",
      "1",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ms.write.over",
                                    mOverwriteMilestone, lOverwriteDescription);
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

#ifdef BEAGLE_HAVE_LIBZ
  if(ioSystem.getRegister().isRegistered("ms.write.compress")) {
    mCompressMilestone =
      castHandleT<Bool>(ioSystem.getRegister().getEntry("ms.write.compress"));
  } else {
    mCompressMilestone = new Bool(true);
    string lLongDescrip = "If true, this flag indicates that milestones will be ";
    lLongDescrip += "compressed with gzip. Otherwise, each milestone are kept plain text.";
    Register::Description lCompressDescription(
      "Milestone compression flag",
      "Bool",
      "1",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ms.write.compress",
                                    mCompressMilestone, lCompressDescription);
  }
#endif // BEAGLE_HAVE_LIBZ
  Beagle_StackTraceEndM("void MilestoneWriteOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the characteristic operation.
 *  \param ioDeme Deme to use to write the milestone.
 *  \param ioContext Context of the evolution.
 */
void MilestoneWriteOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(mMilestonePrefix->getWrappedValue().empty()) return;
  if(ioContext.getContinueFlag() == true) {
    if(mWritingInterval->getWrappedValue() == 0) return;
    if((ioContext.getGeneration() % mWritingInterval->getWrappedValue()) != 0) return;
    if((mPerDemeMilestone->getWrappedValue() == false) &&
       (ioContext.getDemeIndex() != (mPopSize->size()-1))) return;
  }
  string lFilename = mMilestonePrefix->getWrappedValue();
  if(mPerDemeMilestone->getWrappedValue()) {
    lFilename += "_d";
    lFilename += uint2str(ioContext.getDemeIndex()+1);
  }
  if(mOverwriteMilestone->getWrappedValue() == false) {
    lFilename += "_g";
    lFilename += uint2str(ioContext.getGeneration());
  }
  lFilename += ".obm";
#ifdef BEAGLE_HAVE_LIBZ
  if(mCompressMilestone->getWrappedValue()) lFilename += ".gz";
#endif // BEAGLE_HAVE_LIBZ
  Beagle_LogBasicM(
    ioContext.getSystem().getLogger(),
    "milestone", "Beagle::MilestoneWriteOp",
    string("Writing milestone file \"")+lFilename+"\""
  );
  writeMilestone(lFilename, ioContext);
  Beagle_StackTraceEndM("void MilestoneWriteOp::operate(Deme& ioDeme, Context& ioContext)");
}


/*!
 *  \brief Write an evolution milestone.
 *  \param inFilename Filename of the milestone.
 *  \param ioContext  Context of the evolution,
 */
void MilestoneWriteOp::writeMilestone(Beagle::string inFilename, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  std::string lFilenameBak = inFilename + "~";
  std::remove(lFilenameBak.c_str());
  std::rename(inFilename.c_str(), lFilenameBak.c_str());
#ifdef BEAGLE_HAVE_LIBZ
  std::ostream* lOS = NULL;
  if(mCompressMilestone->getWrappedValue()) lOS = new ogzstream(inFilename.c_str());
  else lOS = new std::ofstream(inFilename.c_str());
#else
  std::ostream* lOS = new std::ofstream(inFilename.c_str());
#endif
  try {
    PACC::XML::Streamer lStreamer(*lOS);
    lStreamer.insertHeader();
    lStreamer.openTag("Beagle");
    lStreamer.insertAttribute("version", BEAGLE_VERSION);
    lStreamer.insertAttribute("generation", uint2str(ioContext.getGeneration()));
    lStreamer.insertAttribute("deme", uint2str(ioContext.getDemeIndex()));
    lStreamer.insertComment("Created by MilestoneWriteOp operator");
    lStreamer.insertComment("System: configuration of the system");
    ioContext.getSystem().write(lStreamer);
    lStreamer.insertComment("Evolver: configuration of the algorithm");
    ioContext.getEvolver().write(lStreamer);
    lStreamer.insertComment("Vivarium: evolved population");
    ioContext.getVivarium().write(lStreamer);
    lStreamer.closeTag();
    *lOS << std::endl;
  }
  catch (...) {
    delete lOS;
    throw;
  }
  delete lOS;
  lOS = NULL;
  Beagle_StackTraceEndM("void MilestoneWriteOp::writeMilestone(string inFilename, Context& ioContext)");
}



