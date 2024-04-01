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
 *  \file   beagle/src/Evolver.cpp
 *  \brief  Source code of class Evolver.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.32.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

#include <set>

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;


/*!
 *  \brief Construct an evolver by adding common operators in it.
 */
Evolver::Evolver()
{
  Beagle_StackTraceBeginM();
  addBasicOperators();
  Beagle_StackTraceEndM("Evolver::Evolver()");
}


/*!
 *  \brief Add the basic operators into the operator map.
 */
void Evolver::addBasicOperators() 
{
  Beagle_StackTraceBeginM();
  addOperator(new IfThenElseOp);
  addOperator(new InvalidateFitnessOp);
  addOperator(new MigrationRandomRingOp);
  addOperator(new MilestoneReadOp);
  addOperator(new MilestoneWriteOp);
  addOperator(new RegisterReadOp);
  addOperator(new SelectRandomOp);
  addOperator(new SelectRouletteOp);
  addOperator(new SelectTournamentOp);
  addOperator(new SelectParsimonyTournOp);
  addOperator(new StatsCalcFitnessSimpleOp);
  addOperator(new StatsCalcFitnessSimpleOp("StatsCalcFitnessSimpleMinOp"));
  addOperator(new StatsCalcFitnessMultiObjOp);
  addOperator(new StatsCalcFitnessMultiObjOp("StatsCalcFitnessMultiObjMinOp"));
  addOperator(new TermMaxGenOp);
  addOperator(new TermMaxFitnessOp);
  addOperator(new TermMinFitnessOp);
  addOperator(new TermMaxEvalsOp);
  addOperator(new GenerationalOp);
  addOperator(new SteadyStateOp);
  addOperator(new MuCommaLambdaOp);
  addOperator(new MuCommaLambdaOp("ec.mulambda.ratio","MuCommaLambdaOp-2"));
  addOperator(new MuPlusLambdaOp);
  addOperator(new NSGA2Op);
  addOperator(new NPGA2Op);
  addOperator(new ParetoFrontCalculateOp);
  addOperator(new DecimateOp);
  addOperator(new OversizeOp);
  addOperator(new RandomShuffleDemeOp);
  addOperator(new HierarchicalFairCompetitionOp);
  addOperator(new IndividualSizeFrequencyStatsOp);
  Beagle_StackTraceEndM("void Evolver::addBasicOperators()");
}

/*!
 *  \brief Add an operator in the bootstrap operator set.
 *  \param inName Name of operator to add to bootstrap operator set.
 *  \throw RunTimeException If named operator is not in operator map.
 */
void Evolver::addBootStrapOp(Beagle::string inName)
{
  Beagle_StackTraceBeginM();
  Map::iterator lOpIter = mOperatorMap.find(inName);
  if(lOpIter == mOperatorMap.end()) {
    std::ostringstream lOSS;
    lOSS << "Operator named \"" << inName << "\" is not in operator map. Installed operators are:";
    for(Map::iterator lItr = mOperatorMap.begin();
        lItr != mOperatorMap.end(); ++lItr) {
      lOSS << "\n- " << lItr->second->getName();
    }
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  Operator::Handle lOper = castHandleT<Operator>(lOpIter->second);
  mBootStrapSet.push_back(lOper->giveReference());
  Beagle_StackTraceEndM("void Evolver::addBootStrapOp(string)");
}


/*!
 *  \brief Add an operator in the main-loop operator set.
 *  \param inName Name of operator to add to main-loop operator set.
 *  \throw RunTimeException If named operator is not in operator map.
 */
void Evolver::addMainLoopOp(Beagle::string inName)
{
  Beagle_StackTraceBeginM();
  Map::iterator lOpIter = mOperatorMap.find(inName);
  if(lOpIter == mOperatorMap.end()) {
    std::ostringstream lOSS;
    lOSS << "Operator named \"" << inName << "\" is not in operator map.  Installed operators are:";
    for(Map::iterator lItr = mOperatorMap.begin();
        lItr!=mOperatorMap.end(); ++lItr) {
      lOSS << "\n- " << lItr->second->getName();
    }
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  Operator::Handle lOper = castHandleT<Operator>(lOpIter->second);
  mMainLoopSet.push_back(lOper->giveReference());
  Beagle_StackTraceEndM("void Evolver::addMainLoopOp(string inName)");
}


/*!
 *  \brief Add a new operator in the evolver.
 *  \param inOperator Operator to add.
 */
void Evolver::addOperator(Operator::Handle inOperator)
{
  Beagle_StackTraceBeginM();
  Beagle_NonNullPointerAssertM(inOperator);
  mOperatorMap[inOperator->getName()] = inOperator;
  Beagle_StackTraceEndM("void Evolver::addOperator(Operator::Handle)");
}


/*!
 *  \brief Evolve a given vivarium.
 *  \param ioVivarium Handle to the vivarium to evolve.
 */
void Evolver::evolve(Vivarium::Handle ioVivarium)
{
  Beagle_StackTraceBeginM();
  logWelcomeMessages();

  // Initialize the evolution context.
  Context::Handle lEvolContext =
    castObjectT<Context*>(mSystemHandle->getContextAllocator().allocate());
  lEvolContext->setSystemHandle(mSystemHandle);
  lEvolContext->setEvolverHandle(this);
  lEvolContext->setVivariumHandle(ioVivarium);
  lEvolContext->setDemeIndex(0);
  lEvolContext->setGeneration(0);
  lEvolContext->setContinueFlag(true);

  Beagle_LogTraceM(
    mSystemHandle->getLogger(),
    "evolver", "Beagle::Evolver",
    string("Vivarium resized to ")+uint2str(mPopSize->size())+" demes"
  );
  ioVivarium->resize(mPopSize->size());

  while( lEvolContext->getContinueFlag() ) {
    unsigned int lGeneration = lEvolContext->getGeneration();

    Beagle_LogBasicM(
      mSystemHandle->getLogger(),
      "evolver", "Beagle::Evolver",
      string("Evolving generation ")+uint2str(lGeneration)
    );
    if(lGeneration == 0) {
      for(unsigned int i=lEvolContext->getDemeIndex(); i<ioVivarium->size(); i++) {
        lEvolContext->setDemeIndex(i);
        lEvolContext->setDemeHandle((*ioVivarium)[i]);

        Beagle_LogInfoM(
          mSystemHandle->getLogger(),
          "evolver", "Beagle::Evolver",
          string("Applying bootstrap operators to the ")+uint2ordinal(i+1)+
          string(" deme")
        );
        for(unsigned int j=0; j<mBootStrapSet.size(); j++) {
          Beagle_LogDetailedM(
            mSystemHandle->getLogger(),
            "evolver", "Beagle::Evolver",
            string("Applying \"")+mBootStrapSet[j]->getName()+string("\"")
          );
          mBootStrapSet[j]->operate(*(*ioVivarium)[i], *lEvolContext);
        }
        if(lEvolContext->getContinueFlag() == false) break;
        if(i != lEvolContext->getDemeIndex()) break;
        if(lGeneration != lEvolContext->getGeneration()) break;
        if(i == (ioVivarium->size()-1)) {
          lEvolContext->setGeneration(lGeneration+1);
          lEvolContext->setDemeIndex(0);
        }
      }
    }
    else {
      for(unsigned int i=lEvolContext->getDemeIndex(); i<ioVivarium->size(); i++) {
        lEvolContext->setDemeIndex(i);
        lEvolContext->setDemeHandle((*ioVivarium)[i]);
        Beagle_LogInfoM(
          mSystemHandle->getLogger(),
          "evolver", "Beagle::Evolver",
          string("Applying main-loop operators to the ")+uint2ordinal(i+1)+
          string(" deme")
        );
        for(unsigned int j=0; j<mMainLoopSet.size(); j++) {
          Beagle_LogDetailedM(
            mSystemHandle->getLogger(),
            "evolver", "Beagle::Evolver",
            string("Applying \"")+mMainLoopSet[j]->getName()+string("\"")
          );
          mMainLoopSet[j]->operate(*(*ioVivarium)[i], *lEvolContext);
        }
        if(lEvolContext->getContinueFlag() == false) break;
        if(i != lEvolContext->getDemeIndex()) break;
        if(lGeneration != lEvolContext->getGeneration()) break;
        if(i == (ioVivarium->size()-1)) {
          lEvolContext->setGeneration(lGeneration+1);
          lEvolContext->setDemeIndex(0);
        }
      }
    }
  }

  mSystemHandle->getLogger().logCurrentTime(Logger::eBasic);
  Beagle_LogBasicM(
    mSystemHandle->getLogger(),
    "evolver", "Beagle::Evolver",
    "End of evolution"
  );
  Beagle_StackTraceEndM("void Evolver::evolve(Vivarium::Handle)");
}


/*!
 *  \brief Return a constant handle to a named operator. Return a NULL handle if
 *    the named operator is not in the evolver.
 *  \param inName Name of the operator to get.
 *  \return Constant handle to the named operator.
 */
const Operator::Handle Evolver::getOperator(Beagle::string inName) const
{
  Beagle_StackTraceBeginM();
  Map::const_iterator lOpIter = mOperatorMap.find(inName);
  if(lOpIter == mOperatorMap.end()) return Operator::Handle(NULL);
  return castHandleT<Operator>(lOpIter->second);
  Beagle_StackTraceEndM("const Operator::Handle Evolver::getOperator(string)");
}


/*!
 *  \brief Return a handle to a named operator. Return a NULL handle if
 *    the named operator is not in the evolver.
 *  \param inName Name of the operator to get.
 *  \return Handle to the named operator.
 */
Operator::Handle Evolver::getOperator(Beagle::string inName)
{
  Beagle_StackTraceBeginM();
  Map::iterator lOpIter = mOperatorMap.find(inName);
  if(lOpIter == mOperatorMap.end()) return Operator::Handle(NULL);
  return castHandleT<Operator>(lOpIter->second);
  Beagle_StackTraceEndM("Operator::Handle Evolver::getOperator(string)");
}


/*!
 *  \brief Initialize the evolver, its operators and the system.
 *  \param ioSystem Handle to the system of the evolution.
 *  \param ioArgc Number of elements on the command-line.
 *  \param ioArgv Element on the command-line.
 */
void Evolver::initialize(System::Handle ioSystem, int& ioArgc, char** ioArgv)
{
  Beagle_StackTraceBeginM();
  // Get system handle.
  mSystemHandle = ioSystem;

  // Parse command-line.
  parseCommandLine(*ioSystem, ioArgc, ioArgv);

  // Logging message.
  Beagle_LogDetailedM(
    ioSystem->getLogger(),
    "evolver", "Beagle::Evolver",
    "Initializing evolver"
  );

  // Add configuration dumper parameter.
  if(ioSystem->getRegister().isRegistered("ec.conf.dump")) {
    mConfigDumper =
      castHandleT<ConfigurationDumper>(ioSystem->getRegister().getEntry("ec.conf.dump"));
  }
  else {
    mConfigDumper = new ConfigurationDumper(*ioSystem, *this, "");
    string lLongDescripDump = "Filename used to dump the configuration. ";
    lLongDescripDump += "A configuration dump means that a configuration file is ";
    lLongDescripDump += "written with the evolver (including the composing operators) ";
    lLongDescripDump += "and the register (including the registered parameters and their ";
    lLongDescripDump += "default values). No evolution is conducted on a configuration dump. ";
    lLongDescripDump += "An empty string means no dump.";
    Register::Description lFileDumperDescription(
      "Configuration dump filename",
      "String",
      "\"\"",
      lLongDescripDump
    );
    ioSystem->getRegister().addEntry("ec.conf.dump", mConfigDumper, lFileDumperDescription);
  }

  // Add configuration file name.
  if(ioSystem->getRegister().isRegistered("ec.conf.file")) {
    mFileName = castHandleT<String>(ioSystem->getRegister().getEntry("ec.conf.file"));
  }
  else {
    mFileName = new String("");
    string lLongDescripFN = "The name of a configuration file containing ";
    lLongDescripFN += "evolver and parameter values. A typical configuration file can ";
    lLongDescripFN += "be created with parameter \"ec.conf.dump\".";
    Register::Description lFileNameDescription(
      "Configuration filename",
      "String",
      "\"\"",
      lLongDescripFN
    );
    ioSystem->getRegister().addEntry("ec.conf.file", mFileName, lFileNameDescription);
  }

  // Add population size parameter
  if(ioSystem->getRegister().isRegistered("ec.pop.size")) {
    mPopSize = castHandleT<UIntArray>(ioSystem->getRegister().getEntry("ec.pop.size"));
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
    ioSystem->getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
  }

  // Initialize operators.
  initializeOperators(*ioSystem);

  // Initialize the system.
  ioSystem->initialize(ioArgc, ioArgv);

  // Calling system post-initialization hook.
  ioSystem->postInit();

  // Calling post-initialization hook of operators.
  postInitOperators(*ioSystem);

  Beagle_StackTraceEndM("void Evolver::initialize(System::Handle, int&, char**)");
}


/*!
 *  \brief Initialize the evolver, its operators and the system.
 *  \param ioSystem Handle to the system of the evolution.
 *  \param inConfigFilename Filename containing configuration values.
 */
void Evolver::initialize(System::Handle ioSystem, Beagle::string inConfigFilename)
{
  Beagle_StackTraceBeginM();
  // Get system handle.
  mSystemHandle = ioSystem;

  // Reading evolver configuration, if any.
  if(inConfigFilename.empty() == false) readEvolverFile(inConfigFilename);

  // Logging message.
  Beagle_LogDetailedM(
    ioSystem->getLogger(),
    "evolver", "Beagle::Evolver",
    "Initializing evolver"
  );

  // Add configuration dumper parameter.
  if(ioSystem->getRegister().isRegistered("ec.conf.dump")) {
    mConfigDumper =
      castHandleT<ConfigurationDumper>(ioSystem->getRegister().getEntry("ec.conf.dump"));
  }
  else {
    mConfigDumper = new ConfigurationDumper(*ioSystem, *this, "");
    string lLongDescripDump = "Filename used to dump the configuration. ";
    lLongDescripDump += "A configuration dump means that a configuration file is ";
    lLongDescripDump += "written with the evolver (including the composing operators) ";
    lLongDescripDump += "and the register (including the registered parameters and their ";
    lLongDescripDump += "default values). No evolution is conducted on a configuration dump. ";
    lLongDescripDump += "An empty string means no dump.";
    Register::Description lFileDumperDescription(
      "Configuration dump filename",
      "String",
      "\"\"",
      lLongDescripDump
    );
    ioSystem->getRegister().addEntry("ec.conf.dump", mConfigDumper, lFileDumperDescription);
  }

  // Add configuration file name.
  if(ioSystem->getRegister().isRegistered("ec.conf.file")) {
    mFileName = castHandleT<String>(ioSystem->getRegister().getEntry("ec.conf.file"));
  }
  else {
    mFileName = new String(inConfigFilename);
    string lDefaultFileName = string("\"") + inConfigFilename + string("\"");
    string lLongDescripFN = "The name of a configuration file containing ";
    lLongDescripFN += "evolver and parameter values. A typical configuration file can ";
    lLongDescripFN += "be created with parameter \"ec.conf.dump\".";
    Register::Description lFileNameDescription(
      "Configuration filename",
      "String",
      lDefaultFileName,
      lLongDescripFN
    );
    ioSystem->getRegister().addEntry("ec.conf.file", mFileName, lFileNameDescription);
  }

  // Add population size parameter
  if(ioSystem->getRegister().isRegistered("ec.pop.size")) {
    mPopSize = castHandleT<UIntArray>(ioSystem->getRegister().getEntry("ec.pop.size"));
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
    ioSystem->getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
  }

  // Initialize operators.
  initializeOperators(*ioSystem);

  // Initialize the system.
  ioSystem->initialize(inConfigFilename);

  // Calling system post-initialization hook.
  ioSystem->postInit();

  // Calling post-initialization hook of operators.
  postInitOperators(*ioSystem);

  Beagle_StackTraceEndM("void Evolver::initialize(System::Handle, string)");
}


/*!
 *  \brief Initialize operators of the evolver.
 *  \param ioSystem System to use for initialization.
 */
void Evolver::initializeOperators(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle_LogDetailedM(
    ioSystem.getLogger(),
    "evolver", "Beagle::Evolver",
    "Initializing operators"
  );
  for(unsigned int i=0; i<mBootStrapSet.size(); i++) {
    if(mBootStrapSet[i]->isInitialized() == false) {
      Beagle_LogTraceM(
        ioSystem.getLogger(),
        "evolver", "Beagle::Evolver",
        string("Initializing operator \"") + mBootStrapSet[i]->getName() + "\""
      );
      mBootStrapSet[i]->setInitialized();
      mBootStrapSet[i]->initialize(ioSystem);
    }
  }
  for(unsigned int j=0; j<mMainLoopSet.size(); j++) {
    if(mMainLoopSet[j]->isInitialized() == false) {
      Beagle_LogTraceM(
        ioSystem.getLogger(),
        "evolver", "Beagle::Evolver",
        string("Initializing operator \"") + mMainLoopSet[j]->getName() + "\""
      );
      mMainLoopSet[j]->setInitialized();
      mMainLoopSet[j]->initialize(ioSystem);
    }
  }
  Beagle_StackTraceEndM("void Evolver::initializeOperators(System&)");
}


/*!
 *  \brief Log welcome messages for the start of an evolutionary run.
 */
void Evolver::logWelcomeMessages() 
{
  Beagle_StackTraceBeginM();
  Beagle_LogBasicM(
    mSystemHandle->getLogger(),
    "evolver", "Beagle::Evolver",
    "Starting an evolution"
  );

  mSystemHandle->getLogger().logCurrentTime(Logger::eBasic);

  Beagle_LogObjectM(
    mSystemHandle->getLogger(),
    Logger::eDetailed,
    "evolver", "Beagle::Evolver",
    mSystemHandle->getRegister()
  );

  Beagle_LogObjectM(
    mSystemHandle->getLogger(),
    Logger::eDetailed,
    "evolver", "Beagle::Evolver",
    (*this)
  );
  Beagle_StackTraceEndM("void Evolver::logWelcomeMessages()");
}


/*!
 *  \brief Read the evoler parameters from the command-line.
 *  \param ioSystem System to use to log messages.
 *  \param ioArgc Number of arguments on the command-line.
 *  \param ioArgv Arguments of the command-line.
 *  \throw Exception If something bad occured.
 */
void Evolver::parseCommandLine(System& ioSystem, int& ioArgc, char** ioArgv)
{
  Beagle_StackTraceBeginM();
  string lMilestoneFilename;

  string lFileName = ioArgv[0];
  if(lFileName.size() > 4) {
    string lSuffix(lFileName.end()-4, lFileName.end());
    if(lSuffix == ".exe") lFileName.resize(lFileName.size()-4);
  }
  lFileName += ".conf";
  std::ifstream lDefaultIS(lFileName.c_str());
  bool lGoodFile = lDefaultIS.good();
  lDefaultIS.close();
  if(lGoodFile) readEvolverFile(lFileName);

  string::size_type lLTLibPos = lFileName.find(string(".libs/lt-"));
  if(lLTLibPos != string::npos) {
    string lFilenameNoLT(lFileName.begin(), lFileName.begin()+lLTLibPos);
    lFilenameNoLT.append(lFileName.begin()+lLTLibPos+9, lFileName.end());
    std::ifstream lDefaultISNoLT(lFilenameNoLT.c_str());
    bool lGoodFileNoLT = lDefaultISNoLT.good();
    lDefaultISNoLT.close();
    if(lGoodFileNoLT) readEvolverFile(lFilenameNoLT);
  }

  string::size_type lLTPos = lFileName.find(string("lt-"));
  if(lLTPos != string::npos) {
    string lFilenameNoLT(lFileName.begin(), lFileName.begin()+lLTPos);
    lFilenameNoLT.append(lFileName.begin()+lLTPos+3, lFileName.end());
    std::ifstream lDefaultISNoLT(lFilenameNoLT.c_str());
    bool lGoodFileNoLT = lDefaultISNoLT.good();
    lDefaultISNoLT.close();
    if(lGoodFileNoLT) readEvolverFile(lFilenameNoLT);
  }

  for(int i=1; i<ioArgc; i++) {
    char lPrefix[4] = "???";
    strncpy(lPrefix, ioArgv[i], 3);
    string lPrefixStr = lPrefix;
    if((lPrefixStr == "-OB") || (lPrefixStr == "-EC")) {
      string lArgument(ioArgv[i]+3);
      while(lArgument.empty() == false) {

        // Extract token from command-line. Tokens are separated by commas.
        string::size_type lComma = lArgument.find(',');
        string lToken;
        if(lComma != string::npos) {
          lToken.insert(lToken.begin(), lArgument.begin(), lArgument.begin()+lComma);
          string lTemp(lArgument.begin()+lComma+1, lArgument.end());
          lArgument = lTemp;
        }
        else {
          lToken = lArgument;
          lArgument.resize(0);
        }

        // Get tag value of expression.
        string::size_type lEqual = lToken.find('=');
        if(lEqual == string::npos) continue;
        string lTag(lToken.begin(), lToken.begin()+lEqual);

        // If parameter is a configuration file name, read evolver part of it.
        if(lTag == "ec.conf.file") {
          string lFilename(lToken.begin()+lEqual+1, lToken.end());
          readEvolverFile(lFilename);
        }
        else if(lTag == "ms.restart.file") {
          string lTmp(lToken.begin()+lEqual+1, lToken.end());
          lMilestoneFilename = lTmp;
        }
      }
    }
  }

  if(lMilestoneFilename.empty() == false) readEvolverFile(lMilestoneFilename);
  Beagle_StackTraceEndM("void Evolver::parseCommandLine(System&, int&, char**)");
}


/*!
 *  \brief Post initialize operators of the evolver.
 *  \param ioSystem System to use for post-initialization.
 */
void Evolver::postInitOperators(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle_LogDetailedM(
    ioSystem.getLogger(),
    "evolver", "Beagle::Evolver",
    "Calling operators post-initialization hook"
  );
  for(unsigned int i=0; i<mBootStrapSet.size(); i++) {
    if(mBootStrapSet[i]->isPostInitialized() == false) {
      Beagle_LogTraceM(
        ioSystem.getLogger(),
        "evolver", "Beagle::Evolver",
        string("Calling post-initialization hook of operator \"") +
        mBootStrapSet[i]->getName() + "\""
      );
      mBootStrapSet[i]->postInit(ioSystem);
      mBootStrapSet[i]->setPostInitialized();
    }
  }
  for(unsigned int j=0; j<mMainLoopSet.size(); j++) {
    if(mMainLoopSet[j]->isPostInitialized() == false) {
      Beagle_LogTraceM(
        ioSystem.getLogger(),
        "evolver", "Beagle::Evolver",
        string("Calling post-initialization hook of operator \"") +
        mMainLoopSet[j]->getName() + "\""
      );
      mMainLoopSet[j]->postInit(ioSystem);
      mMainLoopSet[j]->setPostInitialized();
    }
  }  
  Beagle_StackTraceEndM("void Evolver::postInitOperators(System&)");
}


/*!
 *  \brief Read an evolver from a XML subtree.
 *  \param inIter XML iterator from which the evolver is read.
 *  \throw IOException If a reading error occurs.
 */
void Evolver::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Evolver"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Evolver> expected!");

  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      if(lChild->getValue() == "BootStrapSet") {
        mBootStrapSet.clear();
        for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
          if(lChild2->getType() == PACC::XML::eData) {
            addBootStrapOp(lChild2->getValue().c_str());
            Operator::Handle lOp = castHandleT<Operator>(mBootStrapSet.back());
            lOp->readWithMap(lChild2, mOperatorMap);
          }
        }
      }
      else if(lChild->getValue() == "MainLoopSet") {
        mMainLoopSet.clear();
        for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
          if(lChild2->getType() == PACC::XML::eData) {
            addMainLoopOp(lChild2->getValue().c_str());
            Operator::Handle lOp = castHandleT<Operator>(mMainLoopSet.back());
            lOp->readWithMap(lChild2, mOperatorMap);
          }
        }
      }
    }
  }
  Beagle_StackTraceEndM("void Evolver::read(PACC::XML::ConstIterator)");
}


/*!
 *  \brief Read an evolver configuration from a file.
 *  \param inFilename Name of file from which the evolver is read.
 *  \throw IOException If a reading error occurs.
 */
void Evolver::readEvolverFile(Beagle::string inFilename)
{
  Beagle_StackTraceBeginM();
#ifdef BEAGLE_HAVE_LIBZ
  igzstream lIFStream(inFilename.c_str());
#else // BEAGLE_HAVE_LIBZ
  std::ifstream lIFStream(inFilename.c_str());
#endif // BEAGLE_HAVE_LIBZ
  if(lIFStream.good() == false) {
    string lMessage = "The filename \"";
    lMessage += inFilename;
    lMessage += "\" doesn't refer to a valid configuration file!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  Beagle_LogInfoM(
    mSystemHandle->getLogger(),
    "evolver", "Beagle::Evolver",
    string("Reading evolver from file named \"")+inFilename+"\""
  );
  PACC::XML::Document lParser;
  lParser.parse(lIFStream, inFilename.c_str());
  lIFStream.close();
  for(PACC::XML::ConstIterator lNode=lParser.getFirstRoot(); lNode; ++lNode) {
    if((lNode->getType()==PACC::XML::eData) && (lNode->getValue()=="Beagle")) {
      for(PACC::XML::ConstIterator lChild = lNode->getFirstChild(); lChild; ++lChild) {
        if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Evolver")) read(lChild);
      }
    }
  }
  Beagle_StackTraceEndM("void Evolver::readEvolverFile(string)");
}


/*!
 *  \brief Remove an operator from the evolver.
 *  \param inName Name of the operator to remove.
 *  \return Operator removed.
 *  \throw Beagle::ObjectException If operator doesn't exist.
 */
Operator::Handle Evolver::removeOperator(Beagle::string inName)
{
  Beagle_StackTraceBeginM();
  Map::iterator lOpIter = mOperatorMap.find(inName);
  if(lOpIter == mOperatorMap.end()) {
    std::ostringstream lOSS;
    lOSS << "Operator named \"" << inName;
    lOSS << "\" doesn't exist in the evolver!";
    throw Beagle_ObjectExceptionM(lOSS.str().c_str());
  }
  Operator::Handle lOper = castHandleT<Operator>(lOpIter->second);
  mOperatorMap.erase(lOpIter);
  return lOper;
  Beagle_StackTraceEndM("Operator::Handle Evolver::removeOperator(string)");
}


/*!
 *  \brief Write an evolver into a XML streamer.
 *  \param ioStreamer XML streamer in which the evolver is written.
 *  \param inIndent Whether XML output should be indented.
 */
void Evolver::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Evolver", inIndent);
  ioStreamer.openTag("BootStrapSet", inIndent);
  for(Operator::Bag::size_type i=0; i<mBootStrapSet.size(); i++) {
    mBootStrapSet[i]->write(ioStreamer, inIndent);
  }
  ioStreamer.closeTag();
  ioStreamer.openTag("MainLoopSet", inIndent);
  for(Operator::Bag::size_type j=0; j<mMainLoopSet.size(); j++) {
    mMainLoopSet[j]->write(ioStreamer, inIndent);
  }
  ioStreamer.closeTag();
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Evolver::write(PACC::XML::Streamer, bool inIndent) const");
}
