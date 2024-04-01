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
 *  \file   beagle/src/Register.cpp
 *  \brief  Source code of class Register.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.23.2.1 $
 *  $Date: 2007/05/09 01:51:21 $
 */

#include "beagle/Beagle.hpp"

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>

#ifdef BEAGLE_HAVE_LIBZ
#include "gzstream.h"
#endif // BEAGLE_HAVE_LIBZ

using namespace Beagle;


/*!
 *  \brief Default constructor of Register::Description.
 */
Register::Description::Description() :
  mBrief("[No brief]"),
  mType(""),
  mDefaultValue(""),
  mDescription("[No description]")
{ }


/*!
 *  \brief Construct a Register::Description instance.
 *  \param inBrief Brief desccription of the parameter.
 *  \param inType Type of the parameter contained.
 *  \param inDefaultValue Default value of the parameter.
 *  \param inDescription Long description of the parameter.
 */
Register::Description::Description(Beagle::string inBrief,
                                   Beagle::string inType,
                                   Beagle::string inDefaultValue,
                                   Beagle::string inDescription) :
  mBrief(inBrief),
  mType(inType),
  mDefaultValue(inDefaultValue),
  mDescription(inDescription)
{ }


/*!
 *  \brief Construct register class instance.
 */
Register::Register() :
  Component("Register")
{ }


/*!
 *  \brief Add a new description in the register.
 *  \param inTag Tag of the entry.
 *  \param inDescription Description of the entry.
 *  \throw RunTimeException If the entry is already described.
 */
void Register::addDescription(Beagle::string inTag, const Description& inDescription)
{
  Beagle_StackTraceBeginM();
  DescriptionMap::const_iterator lIterDescrip = mDescriptions.find(inTag);
  if(lIterDescrip != mDescriptions.end()) {
    string lMessage = "The entry \"";
    lMessage += inTag;
    lMessage += "\" is already described!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  mDescriptions[inTag] = inDescription;
  Beagle_StackTraceEndM("void Register::addDescription(string inTag, const Description& inDescription)");
}


/*!
 *  \brief Add a new entry in the register.
 *  \param inTag Tag of the entry.
 *  \param inEntry Value of the entry.
 *  \param inDescription Description of the entry.
 *  \throw RunTimeException If the entry is already in the register.
 */
void Register::addEntry(Beagle::string inTag, Object::Handle inEntry,
                        const Register::Description& inDescription)
{
  Beagle_StackTraceBeginM();
  Map::const_iterator lIterObj = mParameters.find(inTag);
  if(lIterObj != mParameters.end()) {
    string lMessage = "The entry \"";
    lMessage += inTag;
    lMessage += "\" is already in the register!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  mParameters[inTag] = inEntry;
  addDescription(inTag,inDescription);
  Beagle_StackTraceEndM("void Register::addEntry(string inTag, Object::Handle inEntry, const Register::Description& inDescription)");
}


/*!
 *  \brief Delete an entry of the register.
 *  \param inTag Tag of the entry.
 *  \return Smart pointer to the deleted object.
 *  \throw RunTimeException If the entry is not in the register.
 */
Object::Handle Register::deleteEntry(Beagle::string inTag)
{
  Beagle_StackTraceBeginM();
  Map::iterator lIterObj = mParameters.find(inTag);
  if(lIterObj == mParameters.end()) {
    string lMessage = "The entry \"";
    lMessage += inTag;
    lMessage += "\" is not registered!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  Object::Handle lObjHandle = lIterObj->second;
  mParameters.erase(lIterObj);
  DescriptionMap::iterator lIterDescrip = mDescriptions.find(inTag);
  if(lIterDescrip == mDescriptions.end()) {
    string lMessage = "No description for entry \"";
    lMessage += inTag;
    lMessage += "\"!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  mDescriptions.erase(lIterDescrip);
  return lObjHandle;
  Beagle_StackTraceEndM("Object::Handle Register::deleteEntry(string inTag)");
}


/*!
 *  \brief Get the description of the given entry.
 *  \param inTag Tag name of the entry.
 *  \throw RunTimeException If the tag is not registred.
 */
const Register::Description& Register::getDescription(Beagle::string inTag) const
{
  Beagle_StackTraceBeginM();
  DescriptionMap::const_iterator lIterDescrip = mDescriptions.find(inTag);
  if(lIterDescrip == mDescriptions.end()) {
    string lMessage = "No description for entry \"";
    lMessage += inTag;
    lMessage += "\"!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  return lIterDescrip->second;
  Beagle_StackTraceEndM("const Register::Description& Register::getDescription(string inTag) const");
}


/*!
 *  \brief Erase an argument from the command-line.
 *  \param inN Index of the argument to remove.
 *  \param ioArgc Number of elements on the command-line.
 *  \param ioArgv Elements of the command-line.
 *  \return Removed element of the command-line.
 */
char* Register::eraseArg(int inN, int& ioArgc, char** ioArgv) const
{
  Beagle_StackTraceBeginM();
  Beagle_BoundCheckAssertM(inN, 1, (ioArgc-1));
  char *lTemp = ioArgv[inN];
  for(int i=inN; i<ioArgc-1; i++) ioArgv[i] = ioArgv[i+1];
  ioArgv[ioArgc-1] = NULL;
  ioArgc--;
  return lTemp;
  Beagle_StackTraceEndM("char* Register::eraseArg(int inN, int& ioArgc, char** ioArgv) const");
}


/*!
 *  \brief Initialize the register.
 *  \param ioSystem System used.
 */
void Register::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle_LogDetailedM(
    ioSystem.getLogger(),
    "register", "Beagle::Register",
    "Initializing register"
  );

  Description lUsageDescription(
    "Print program usage",
    "",
    "",
    "Shows the Open BEAGLE specific command-line usage."
  );
  addDescription("usage", lUsageDescription);

  Description lHelpDescription(
    "Print detailed help",
    "",
    "",
    "Shows the Open BEAGLE specific command-line usage and detailed parameter descriptions."
  );
  addDescription("help", lHelpDescription);

  if(isRegistered("ec.conf.file")) {
    mFileName = castHandleT<String>(getEntry("ec.conf.file"));
  }
  else {
    mFileName = new String("");
    string lLongDescripFN = "The name of a configuration file containing ";
    lLongDescripFN += "evolver and parameter values. A typical configuration file can ";
    lLongDescripFN += "be created with parameter \"ec.conf.dump\".";
    Description lFileNameDescription(
      "Configuration filename",
      "String",
      "",
      lLongDescripFN
    );
    addEntry("ec.conf.file", mFileName, lFileNameDescription);
  }
  Beagle_StackTraceEndM("void Register::initialize(System& ioSystem)");
}


/*!
 *  \brief Interpret the command-line to extract the appropriated arguments.
 *  \param ioSystem System to use to log messages.
 *  \param ioArgc Number of elements of the command-line.
 *  \param ioArgv Elements of the command-line.
 */
void Register::interpretArgs(System& ioSystem, int& ioArgc, char** ioArgv)
{
  Beagle_StackTraceBeginM();
  for(int i=1; i<ioArgc; i++) {
    char lPrefix[4] = "???";
    strncpy(lPrefix, ioArgv[i], 3);
    string lPrefixStr = lPrefix;
    if((lPrefixStr == "-OB") || (lPrefixStr == "-EC")) {
      if(lPrefixStr == "-EC") {
        Beagle_LogBasicM(
          ioSystem.getLogger(),
          "register", "Beagle::Register",
          string("The prefix -EC for Open BEAGLE command-line directives is DEPRECATED, ")+
          string("please use the prefix -OB instead.")
        );
      }
      char *lArg = eraseArg(i--,ioArgc,ioArgv)+3;
      while(true) {
        string lTag(lArg);

        char *lDelimiter = strchr(lArg, '=');
        if(lDelimiter == NULL) {
          if(lTag == "usage") {
            showUsage(ioArgv, std::cerr);
            exit(0);
          }
          else if(lTag == "help") {
            showHelp(ioArgv, std::cerr);
            exit(0);
          }
          else {
            std::cerr << "Bad command-line format!" << std::endl;
            showUsage(ioArgv, std::cerr);
            exit(1);
          }
        }

        *lDelimiter = '\0';
        lTag = lArg;

        Map::iterator lIterTag = mParameters.find(lTag);
        if(lIterTag == mParameters.end()) {
          std::cerr << "Bad command-line format!" << std::endl
                    << "The parameter \"" << lTag << "\" is not registred." << std::endl;
          showUsage(ioArgv, std::cerr);
          exit(1);
        }

        char *lNext = strchr(lDelimiter+1,',');
        if(lNext!=NULL) *lNext = '\0';
        string lStringValue(lDelimiter+1);
        Beagle_LogDetailedM(
          ioSystem.getLogger(),
          "register", "Beagle::Register",
          string("Parameter tag \"")+lTag+string("\" with associated value \"")+
          lStringValue+"\" read on the command-line"
        );

        try {
          std::istringstream lISStream(lStringValue.c_str());
          PACC::XML::Document lParser(lISStream);
          PACC::XML::ConstIterator lRootIter = lParser.getFirstRoot();
          lIterTag->second->read(lRootIter);
        } catch(Exception& inException) {
          std::ostringstream lOSS;
          lOSS << "An exception of the type \"";
          lOSS << inException.getExceptionName();
          lOSS << "\" had been thrown during the command-line parsing. ";
          lOSS << "The reason is: ";
          lOSS << inException.getMessage();
          throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
        } catch(std::runtime_error& inException) {
          std::ostringstream lOSS;
          lOSS << "A runtime exception had been thrown during the command-line parsing. ";
          lOSS << "The reason is: ";
          lOSS <<  inException.what();
          throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
        }

        // If it is a configuration file name.
        if((lTag == "ec.conf.file") &&
           (lStringValue.empty()==false) &&
           (lStringValue != "")) {
          readParametersFile(lStringValue, ioSystem);
        }

        if(lNext != NULL) lArg = lNext+1;
        else break;
      }
    }
  }
  Beagle_StackTraceEndM("void Register::interpretArgs(System& ioSystem, int& ioArgc, char** ioArgv)");
}


/*!
 *  \brief To check whether a given tag-entry is registered.
 *  \param inTag Tag of the entry.
 *  \return True if an entry with the given tag is registred, false if not.
 */
bool Register::isRegistered(Beagle::string inTag) const
{
  Beagle_StackTraceBeginM();
  Map::const_iterator lIterObj = mParameters.find(inTag);
  return (lIterObj != mParameters.end());
  Beagle_StackTraceEndM("bool Register::isRegistered(string inTag) const");
}


/*!
 *  \brief Modify the smart pointer of an entry in the register.
 *  \param inTag Tag of the entry.
 *  \param inNewEntry New entry that will replace to old one.
 *  \return Handle to the old entry.
 *  \throw RunTimeException If the entry is not found in the register.
 */
Object::Handle Register::modifyEntry(Beagle::string inTag, Object::Handle inNewEntry)
{
  Beagle_StackTraceBeginM();
  Map::iterator lIterObj = mParameters.find(inTag);
  if(lIterObj == mParameters.end()) {
    string lMessage = "The entry \"";
    lMessage += inTag;
    lMessage += "\" is not registered!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  Object::Handle lObjHandle = lIterObj->second;
  lIterObj->second = inNewEntry;
  return lObjHandle;
  Beagle_StackTraceEndM("Object::Handle Register::modifyEntry(string inTag, Object::Handle inNewEntry)");
}


/*!
 *  \brief Modify the description of an entry in the register.
 *  \param inTag Tag of the entry.
 *  \param inNewDescription New description that will replace to old one.
 *  \return Old description.
 *  \throw RunTimeException If the entry is not found in the register.
 */
Register::Description Register::modifyDescription(Beagle::string inTag,
                                                  const Register::Description& inNewDescription)
{
  Beagle_StackTraceBeginM();
  DescriptionMap::iterator lIterDescrip = mDescriptions.find(inTag);
  if(lIterDescrip == mDescriptions.end()) {
    string lMessage = "The entry \"";
    lMessage += inTag;
    lMessage += "\" is not registered!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  Description lDescription = lIterDescrip->second;
  lIterDescrip->second = inNewDescription;
  return lDescription;
  Beagle_StackTraceEndM("Register::Description Register::modifyDescription(string inTag, const Register::Description& inNewDescription)");
}


/*!
 *  \brief Read the parameters from the command-line and files.
 *  \param ioSystem System to use to log messages.
 *  \param ioArgc Number of arguments on the command-line.
 *  \param ioArgv Arguments of the command-line.
 *  \throw Exception If something bad occured ;).
 */
void Register::parseCommandLine(System& ioSystem, int& ioArgc, char** ioArgv)
{
  Beagle_StackTraceBeginM();
  string lCommandLine = ioArgv[0];
  for(int i=1; i<ioArgc; ++i) lCommandLine += string(" ") + ioArgv[i];
  Beagle_LogInfoM(
    ioSystem.getLogger(),
    "register", "Beagle::Register",
    string("Parsing command-line \"")+lCommandLine+string("\"")
  );
  string lFileName = ioArgv[0];
  if(lFileName.size() > 4) {
    string lSuffix(lFileName.end()-4, lFileName.end());
    if(lSuffix == ".exe") lFileName.resize(lFileName.size()-4);
  }
  lFileName += ".conf";
  std::ifstream lDefaultIS(lFileName.c_str());
  bool lGoodFile = lDefaultIS.good();
  lDefaultIS.close();
  if(lGoodFile) readParametersFile(lFileName, ioSystem);

  string::size_type lLTLibPos = lFileName.find(string(".libs/lt-"));
  if(lLTLibPos != string::npos) {
    string lFilenameNoLT(lFileName.begin(), lFileName.begin()+lLTLibPos);
    lFilenameNoLT.append(lFileName.begin()+lLTLibPos+9, lFileName.end());
    std::ifstream lDefaultISNoLT(lFilenameNoLT.c_str());
    bool lGoodFileNoLT = lDefaultISNoLT.good();
    lDefaultISNoLT.close();
    if(lGoodFileNoLT) readParametersFile(lFilenameNoLT, ioSystem);
  }

  string::size_type lLTPos = lFileName.find(string("lt-"));
  if(lLTPos != string::npos) {
    string lFilenameNoLT(lFileName.begin(), lFileName.begin()+lLTPos);
    lFilenameNoLT.append(lFileName.begin()+lLTPos+3, lFileName.end());
    std::ifstream lDefaultISNoLT(lFilenameNoLT.c_str());
    bool lGoodFileNoLT = lDefaultISNoLT.good();
    lDefaultISNoLT.close();
    if(lGoodFileNoLT) readParametersFile(lFilenameNoLT, ioSystem);
  }

  interpretArgs(ioSystem, ioArgc, ioArgv);
  Beagle_StackTraceEndM("void Register::parseCommandLine(System& ioSystem, int& ioArgc, char** ioArgv)");
}


/*!
 *  \brief Read a register from a XML subtree.
 *  \param inIter XML iter to read the register from.
 *  \throw Beagle::IOException If reading from the stream problems occured.
 */
void Register::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Register"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Register> expected!");
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Entry")) {
      string lEntryKey = lChild->getAttribute("key").c_str();
      if(lEntryKey.empty())
        throw Beagle_IOExceptionNodeM(*lChild, "no key given for actual entry!");
      if(mParameters.find(lEntryKey) == mParameters.end()) continue;
      PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
      mParameters[lEntryKey]->read(lChild2);
    }
  }
  Beagle_StackTraceEndM("void Register::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Read parameters from a file.
 *  \param inFileName File name of the parameters to read.
 *  \param ioSystem Evolutionary system.
 */
void Register::readParametersFile(Beagle::string inFileName, System& ioSystem)
{
  Beagle_StackTraceBeginM();
  mFileName->setWrappedValue(inFileName);
#ifdef BEAGLE_HAVE_LIBZ
  igzstream lIFStream(inFileName.c_str());
#else // BEAGLE_HAVE_LIBZ
  std::ifstream lIFStream(inFileName.c_str());
#endif // BEAGLE_HAVE_LIBZ
  if(lIFStream.good() == false) {
    string lMessage = "The filename \"";
    lMessage += inFileName;
    lMessage += "\" doesn't refer to a valid parameter file!";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
  Beagle_LogInfoM(
    ioSystem.getLogger(),
    "register", "Beagle::Register",
    string("Reading parameter file named \"")+inFileName+"\""
  );
  PACC::XML::Document lParser(lIFStream, inFileName.c_str());
  lIFStream.close();
  for(PACC::XML::ConstIterator lIter=lParser.getFirstRoot(); lIter; ++lIter) {
    if((lIter->getType()==PACC::XML::eData) && (lIter->getValue()=="Register")) {
      readWithSystem(lIter, ioSystem);
    }
    else if((lIter->getType()==PACC::XML::eData) && (lIter->getValue()=="Beagle")) {
      for(PACC::XML::ConstIterator lChild = lIter->getFirstChild(); lChild; ++lChild) {
        if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Register")) {
          readWithSystem(lChild, ioSystem);
        }
        if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="System")) {
          ioSystem.read(lChild);
        }
      }
    }
  }
  Beagle_StackTraceEndM("void Register::readParametersFile(string inFileName, System& ioSystem)");
}


/*!
 *  \brief Read a register from a Beagle input stream.
 *  \param inIter XML iterator to read the register from.
 *  \param ioSystem Reference to evolutionary system.
 *  \throw Beagle::IOException If reading from the stream problems occured.
 */
void Register::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Register"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Register> expected!");
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Entry")) {
      string lEntryKey = lChild->getAttribute("key").c_str();
      if(lEntryKey.empty())
        throw Beagle_IOExceptionNodeM(*lChild, "no key given for actual entry!");
      if(mParameters.find(lEntryKey) == mParameters.end()) {
        Beagle_LogBasicM(
          ioSystem.getLogger(),
          "register", "Beagle::Register",
          string("Warning: error while reading register, parameter named \"")+
          lEntryKey+"\" is not registered!"
        );
        continue;
      }
      PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
      mParameters[lEntryKey]->read(lChild2);
      Beagle_LogTraceM(
        ioSystem.getLogger(),
        "register", "Beagle::Register",
        string("Register entry \"")+lEntryKey+string("\" is now ")+
        mParameters[lEntryKey]->serialize()
      );
    }
  }
  Beagle_StackTraceEndM("void Register::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)");
}


/*!
 *  \brief Display the program usage in the output stream.
 *  \param ioArgv Elements of the command-line.
 *  \param ioOs C++ output stream to write the usage into.
 */
void Register::showHelp(char** ioArgv, std::ostream& ioOs) const
{
  Beagle_StackTraceBeginM();
  ioOs << "Open BEAGLE detailed help  (package: ";
  ioOs << BEAGLE_PACKAGE << ", version: " << BEAGLE_VERSION << ")" << std::endl << std::endl;
  ioOs << "usage: " << ioArgv[0] << " [-OBparameter=value] ..." << std::endl;
  ioOs << "       " << ioArgv[0] << " [-OBparam1=value1,param2=value2, ... ,paramN=valueN] ...";
  ioOs << std::endl << std::endl;

  ioOs << "Supported parameters:" << std::endl << std::endl;
  for(DescriptionMap::const_iterator lIter=mDescriptions.begin();
      lIter!=mDescriptions.end(); ++lIter) {
    ioOs << "  " << lIter->first;

    if(lIter->second.mType.empty() == false) {
      unsigned int lCountSize = lIter->first.size()+2;
      Beagle_AssertM(lCountSize <= 46);
      string lTmpStr1(46-lCountSize, ' ');
      ioOs << lTmpStr1;
      ioOs << "<" << lIter->second.mType << "> (def: ";
      ioOs << lIter->second.mDefaultValue << ")";
    }
    ioOs << std::endl;

    string lDescription = lIter->second.mDescription;
    wrapString(lDescription, 74);
    string lBlank("    ");
    lDescription = lBlank + lDescription;
    for(unsigned int i=0; i<lDescription.size(); i++) {
      if(lDescription[i] == '\n') lDescription.insert(i+1, "    ");
    }
    ioOs << lDescription << std::endl << std::endl;
  }
  ioOs << std::endl << std::flush;
  Beagle_StackTraceEndM("void Register::showHelp(char** ioArgv, std::ostream& ioOs) const");
}


/*!
 *  \brief Display the program help in the output stream.
 *  \param ioArgv Elements of the command-line.
 *  \param ioOs C++ output stream to write the usage into.
 */
void Register::showUsage(char** ioArgv, std::ostream& ioOs) const
{
  Beagle_StackTraceBeginM();
  ioOs << "usage: " << ioArgv[0] << " [-OBparameter=value] ..." << std::endl;
  ioOs << "       " << ioArgv[0] << " [-OBparam1=value1,param2=value2, ... ,paramN=valueN] ...";
  ioOs << std::endl << std::endl;

  for(DescriptionMap::const_iterator lIter=mDescriptions.begin();
      lIter!=mDescriptions.end(); ++lIter) {
    ioOs << " -OB" << lIter->first;
    unsigned int lCountSize = lIter->first.size()+4;
    if(lIter->second.mType.empty() == false) {
      ioOs << "=<" << lIter->second.mType << ">";
      lCountSize += lIter->second.mType.size()+3;
    }
    Beagle_AssertM(lCountSize <= 46);
    string lTmpStr3(46-lCountSize, ' ');
    ioOs << lTmpStr3;
    ioOs << lIter->second.mBrief << std::endl;
  }
  ioOs << std::endl << std::flush;
  Beagle_StackTraceEndM("void Register::showUsage(char** ioArgv, std::ostream& ioOs) const");
}


/*!
 *  \brief Write a register into a Beagle output stream.
 *  \param ioStreamer XML streamer to read the register to.
 *  \param inIndent Whether XML output should be indented.
 */
void Register::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Register", inIndent);
  for(Map::const_iterator lIter=mParameters.begin(); lIter != mParameters.end(); ++lIter) {
    DescriptionMap::const_iterator lDescription=mDescriptions.find(lIter->first);
    if(lDescription != mDescriptions.end()) {
      std::ostringstream lOSS;
      lOSS << lIter->first << " [";
      lOSS << lDescription->second.mType << "]: ";
      lOSS << lDescription->second.mDescription;
      ioStreamer.insertComment(lOSS.str());
    }
    ioStreamer.openTag("Entry", false);
    ioStreamer.insertAttribute("key", lIter->first);
    lIter->second->write(ioStreamer, false);
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Register::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

