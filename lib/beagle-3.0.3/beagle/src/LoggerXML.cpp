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
 *  \file   beagle/src/LoggerXML.cpp
 *  \brief  Implementation of class LoggerXML.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.16.2.2 $
 *  $Date: 2007/09/10 18:24:12 $
 */

#include "beagle/Beagle.hpp"

#include <cstdio>
#include <iomanip>
#include <fstream>

using namespace Beagle;

/*!
 *  \brief Construct a new LoggerXML.
 */
LoggerXML::LoggerXML() :
  mActualFileName(""),
  mStreamerFile(NULL),
  mLogOutStream(NULL),
  mStreamerConsole(NULL),
  mTerminated(false)
{ }


/*!
 *  \brief Copy construct a LoggerXML is undefined.
 */
LoggerXML::LoggerXML(const LoggerXML& inLoggerXML) :
  Logger()
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("LoggerXML(const LoggerXML&)", "LoggerXML", getName());
  Beagle_StackTraceEndM("LoggerXML::LoggerXML(const LoggerXML& inLoggerXML)");
}


/*!
 *  \brief Destruct a LoggerXML.
 */
LoggerXML::~LoggerXML()
{
  Beagle_StackTraceBeginM();
  terminate();
  Beagle_StackTraceEndM("LoggerXML::~LoggerXML()");
}


/*!
 *  \brief Copy a LoggerXML is undefined.
 */
LoggerXML& LoggerXML::operator=(const LoggerXML& inLoggerXML)
{
  Beagle_StackTraceBeginM();
  throw Beagle_UndefinedMethodInternalExceptionM("operator=","LoggerXML",getName());
  Beagle_StackTraceEndM("LoggerXML& LoggerXML::operator=(const LoggerXML& inLoggerXML)");
}


/*!
 *  \brief Initialize logger by registering its parameters.
 *  \param ioSystem Reference to the system to use for initialization.
 */
void LoggerXML::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Logger::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("lg.console.level")) {
    mLogConsoleLevel = castHandleT<UInt>(ioSystem.getRegister().getEntry("lg.console.level"));
  } else {
    mLogConsoleLevel = new UInt(eStats);
    string lLongDescrip("Log level used for console output generation. ");
    lLongDescrip += "Log levels available are: (0) no log, (1) basic logs, (2) stats, ";
    lLongDescrip += "(3) general informations, (4) details on operations, ";
    lLongDescrip += "(5) trace of the algorithms, (6) verbose, ";
    lLongDescrip += "(7) debug (enabled only in full debug mode).";
    Register::Description lDescription(
      "Console log level",
      "UInt",
      "2",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("lg.console.level", mLogConsoleLevel, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("lg.file.level")) {
    mLogFileLevel = castHandleT<UInt>(ioSystem.getRegister().getEntry("lg.file.level"));
  } else {
    mLogFileLevel = new UInt(eInfo);
    string lLongDescrip("Log level used for file output generation. ");
    lLongDescrip += "Log levels available are: (0) no log, (1) basic logs, (2) stats, ";
    lLongDescrip += "(3) general informations, (4) details on operations, ";
    lLongDescrip += "(5) trace of the algorithms, (6) verbose, ";
    lLongDescrip += "(7) debug (enabled only in full debug mode).";
    Register::Description lDescription(
      "File log level",
      "UInt",
      "3",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("lg.file.level", mLogFileLevel, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("lg.file.name")) {
    mLogFileName = castHandleT<String>(ioSystem.getRegister().getEntry("lg.file.name"));
  } else {
    mLogFileName = new String("beagle.log");
    string lLongDescrip("Filename in which messages are outputed. ");
    lLongDescrip += "An empty string file name means no output is done to a file.";
    Register::Description lDescription(
      "Log filename",
      "String",
      "\"beagle.log\"",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("lg.file.name", mLogFileName, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("lg.show.level")) {
    mShowLevel = castHandleT<Bool>(ioSystem.getRegister().getEntry("lg.show.level"));
  } else {
    mShowLevel = new Bool(false);
    string lLongDescrip("Flag whether logging level in outputed in the logs.");
    Register::Description lDescription(
      "Show level in logs",
      "Bool",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("lg.show.level", mShowLevel, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("lg.show.type")) {
    mShowType = castHandleT<Bool>(ioSystem.getRegister().getEntry("lg.show.type"));
  } else {
    mShowType = new Bool(false);
    string lLongDescrip("Flag whether message type is outputed in the logs.");
    Register::Description lDescription(
      "Show message type in logs",
      "Bool",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("lg.show.type", mShowType, lDescription);
  }

  if(ioSystem.getRegister().isRegistered("lg.show.class")) {
    mShowClass = castHandleT<Bool>(ioSystem.getRegister().getEntry("lg.show.class"));
  } else {
    mShowClass = new Bool(false);
    string lLongDescrip("Flag whether class name is outputed in the logs.");
    Register::Description lDescription(
      "Show class name in logs",
      "Bool",
      "0",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("lg.show.class", mShowClass, lDescription);
  }
  Beagle_StackTraceEndM("void LoggerXML::initialize(System& ioSystem)");
}


/*!
 *  \brief Write message to the output device.
 *  \param inLevel Log level of the message
 *  \param inType Type of the message to log.
 *  \param inClass Class associated to the message.
 *  \param inMessage Message to log in the output device.
 */
void LoggerXML::outputMessage(unsigned int inLevel,
                              Beagle::string inType,
                              Beagle::string inClass,
                              Beagle::string inMessage)
{
  Beagle_StackTraceBeginM();
  if(mTerminated)
    throw Beagle_RunTimeExceptionM("Can't log in a terminated XML logger!");

  // Log file output
  if(mLogFileLevel->getWrappedValue()>=inLevel) {
    if(mActualFileName != mLogFileName->getWrappedValue()) {
      mActualFileName = mLogFileName->getWrappedValue();
      if(mStreamerFile != NULL) {
        mStreamerFile->closeTag();
        mStreamerFile->closeTag();
        *mLogOutStream << std::endl;
        delete mStreamerFile;
        mStreamerFile = NULL;
      }
      if(mLogOutStream) {
        mLogOutStream->close();
        delete mLogOutStream;
        mLogOutStream = NULL;
      }
      if(mLogFileName->getWrappedValue().empty() == false) {
        string lFilenameBak = mLogFileName->getWrappedValue() + "~";
        std::remove(lFilenameBak.c_str());
        std::rename(mLogFileName->getWrappedValue().c_str(), lFilenameBak.c_str());
        mLogOutStream = new std::ofstream(mLogFileName->getWrappedValue().c_str());
        mStreamerFile = new PACC::XML::Streamer(*mLogOutStream);
        mStreamerFile->insertHeader();
        mStreamerFile->openTag("Beagle");
        mStreamerFile->insertAttribute("version", BEAGLE_VERSION);
        mStreamerFile->openTag("Logger");
      }
    }
    if(mStreamerFile != NULL) {
      mStreamerFile->openTag("Log", false);
      if(mShowLevel->getWrappedValue()) mStreamerFile->insertAttribute("level", uint2str(inLevel));
      if(mShowType->getWrappedValue())  mStreamerFile->insertAttribute("type",  inType);
      if(mShowClass->getWrappedValue()) mStreamerFile->insertAttribute("class", inClass);
      mStreamerFile->insertStringContent(inMessage.c_str());
      mStreamerFile->closeTag();
    }
  }

  // Log console output  
  if(mLogConsoleLevel->getWrappedValue()>=inLevel) {
    if(mStreamerConsole == NULL) {
      mStreamerConsole = new PACC::XML::Streamer(std::cout);
      mStreamerConsole->insertHeader();
      mStreamerConsole->openTag("Beagle");
      mStreamerConsole->insertAttribute("version", BEAGLE_VERSION); 
      mStreamerConsole->openTag("Logger");
    }
    mStreamerConsole->openTag("Log", false);
    if(mShowLevel->getWrappedValue()) mStreamerConsole->insertAttribute("level", uint2str(inLevel));
    if(mShowType->getWrappedValue())  mStreamerConsole->insertAttribute("type",  inType);
    if(mShowClass->getWrappedValue()) mStreamerConsole->insertAttribute("class", inClass);
    mStreamerConsole->insertStringContent(inMessage.c_str());
    mStreamerConsole->closeTag();
  }
  Beagle_StackTraceEndM("void LoggerXML::outputMessage(unsigned int inLevel, string inType, string inClass, string inMessage)");
}


/*!
 *  \brief Write Beagle object to the output device.
 *  \param inLevel Log level of the message
 *  \param inType Type of the message to log.
 *  \param inClass Class associated to the message.
 *  \param inObject Beagle object to log in the output device.
 */
void LoggerXML::outputObject(unsigned int inLevel,
                             Beagle::string inType,
                             Beagle::string inClass,
                             const Object& inObject)
{
  Beagle_StackTraceBeginM();
  if(mTerminated)
    throw Beagle_RunTimeExceptionM("Can't log in a terminated XML logger!");

  // Log file output
  if(mLogFileLevel->getWrappedValue()>=inLevel) {    
    if(mActualFileName != mLogFileName->getWrappedValue()) {
      mActualFileName = mLogFileName->getWrappedValue();
      if(mStreamerFile != NULL) {
        mStreamerFile->closeTag();
        mStreamerFile->closeTag();
        *mLogOutStream << std::endl;
        delete mStreamerFile;
        mStreamerFile = NULL;
      }
      if(mLogOutStream) {
        mLogOutStream->close();
        delete mLogOutStream;
        mLogOutStream = NULL;
      }
      if(mLogFileName->getWrappedValue().empty() == false) {
        string lFilenameBak = mLogFileName->getWrappedValue() + "~";
        std::remove(lFilenameBak.c_str());
        std::rename(mLogFileName->getWrappedValue().c_str(), lFilenameBak.c_str());
        mLogOutStream = new std::ofstream(mLogFileName->getWrappedValue().c_str());
        mStreamerFile = new PACC::XML::Streamer(*mLogOutStream);
        mStreamerFile->insertHeader();
        mStreamerFile->openTag("Beagle");
        mStreamerFile->insertAttribute("version", BEAGLE_VERSION); 
        mStreamerFile->openTag("Logger");
      }
    }
    if(mStreamerFile != NULL) {
      mStreamerFile->openTag("Log");
      if(mShowLevel->getWrappedValue()) mStreamerFile->insertAttribute("level", uint2str(inLevel));
      if(mShowType->getWrappedValue())  mStreamerFile->insertAttribute("type",  inType);
      if(mShowClass->getWrappedValue()) mStreamerFile->insertAttribute("class", inClass);
      inObject.write(*mStreamerFile);
      mStreamerFile->closeTag();
    }
  }

  // Log console output
  if(mLogConsoleLevel->getWrappedValue()>=inLevel) {
    if(mStreamerConsole == NULL) {
      mStreamerConsole = new PACC::XML::Streamer(std::cout);
      mStreamerConsole->insertHeader();
      mStreamerConsole->openTag("Beagle");
      mStreamerConsole->insertAttribute("version", BEAGLE_VERSION);  
      mStreamerConsole->openTag("Logger");
    }
    mStreamerConsole->openTag("Log");
    if(mShowLevel->getWrappedValue()) mStreamerConsole->insertAttribute("level", uint2str(inLevel));
    if(mShowType->getWrappedValue())  mStreamerConsole->insertAttribute("type",  inType);
    if(mShowClass->getWrappedValue()) mStreamerConsole->insertAttribute("class", inClass);
    inObject.write(*mStreamerConsole);
    mStreamerConsole->closeTag();
  }
  Beagle_StackTraceEndM("void LoggerXML::outputObject(unsigned int inLevel, string inType, string inClass, const Object& inObject)");
}


/*!
 *  \brief Post-initialize logger.
 */
void LoggerXML::postInit(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Logger::postInit(ioSystem);

  mInitialized = true;

  Beagle_LogBasicM(
    (*this),
    "logger", "Beagle::LoggerXML",
    string("Open BEAGLE, version ")+BEAGLE_VERSION
  );

  switch(mLogConsoleLevel->getWrappedValue()) {
    case eNothing:
    {
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console logger initialized, using log level 0 (no message)"
      );
      break;
    }
    case eBasic:
    {
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console logger initialized, using log level 1 (essential informations)"
      );
      break;
    }
    case eStats:
    {
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console logger initialized, using log level 2 (evolution statistics)"
      );
      break;
    }
    case eInfo:
    {
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console logger initialized, using log level 3 (general informations)"
      );
      break;
    }
    case eDetailed:
    {
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console logger initialized, using log level 4 (details on operations)"
      );
      break;
    }
    case eTrace:
    {
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console logger initialized, using log level 5 (trace of the algorithms)"
      );
      break;
    }
    case eVerbose:
    {
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console logger initialized, using log level 6 (details on everything)"
      );
#ifdef BEAGLE_NDEBUG
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console log level 6 (verbose) is unused as optimization mode is enabled"
      );
#endif // BEAGLE_NDEBUG
      break;
    }
    case eDebug:
    {
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console logger initialized, using log level 7 (debug)"
      );
#ifdef BEAGLE_NDEBUG
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console log level 7 (debug) is unused as optimization mode is enabled"
      );
#endif // BEAGLE_NDEBUG
#ifndef BEAGLE_FULL_DEBUG
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        "Console log level 7 (debug) is unused as full debug mode is disabled"
      );
#endif // BEAGLE_FULL_DEBUG
      break;
    }
    default:
    {
      Beagle_LogBasicM(
        (*this),
        "logger", "Beagle::LoggerXML",
        string("Console logger initialized, using log level ")+
        uint2str(mLogConsoleLevel->getWrappedValue())+
        " (unused)"
      );
      break;
    }
  }

  if(mLogFileName->getWrappedValue().empty()) {
    Beagle_LogBasicM(
      (*this),
      "logger", "Beagle::LoggerXML",
      "File logging disabled"
    );
  }
  else {
    switch(mLogFileLevel->getWrappedValue()) {
      case eNothing:
      {
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File logger initialized, using log level 0 (no message)"
        );
        break;
      }
      case eBasic:
      {
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File logger initialized, using log level 1 (essential informations)"
        );
        break;
      }
      case eStats:
      {
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File logger initialized, using log level 2 (evolution statistics)"
        );
        break;
      }
      case eInfo:
      {
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File logger initialized, using log level 3 (general informations)"
        );
        break;
      }
      case eDetailed:
      {
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File logger initialized, using log level 4 (details on operations)"
        );
        break;
      }
      case eTrace:
      {
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File logger initialized, using log level 5 (trace of the algorithms)"
        );
        break;
      }
      case eVerbose:
      {
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File logger initialized, using log level 6 (details on everything)"
        );
  #ifdef BEAGLE_NDEBUG
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File log level 6 (verbose) is unused as optimization mode is enabled"
        );
  #endif // BEAGLE_NDEBUG
        break;
      }
      case eDebug:
      {
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File logger initialized, using log level 7 (debug)"
        );
  #ifdef BEAGLE_NDEBUG
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File log level 7 (debug) is unused as optimization mode is enabled"
        );
  #endif // BEAGLE_NDEBUG
  #ifndef BEAGLE_FULL_DEBUG
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          "File log level 7 (debug) is unused as full debug mode is disabled"
        );
  #endif // BEAGLE_FULL_DEBUG
        break;
      }
      default:
      {
        Beagle_LogBasicM(
          (*this),
          "logger", "Beagle::LoggerXML",
          string("File logger initialized, using log level ")+
          uint2str(mLogFileLevel->getWrappedValue())+
          " (unused)"
        );
        break;
      }
    }

    Beagle_LogBasicM(
      (*this),
      "logger", "Beagle::LoggerXML",
      string("Logging to file named \"")+mLogFileName->getWrappedValue()+string("\"")
    );
  }

  for(std::list< Message,BEAGLE_STLALLOCATOR<Message> >::const_iterator lIter = mBuffer.begin();
      lIter != mBuffer.end(); ++lIter) {
    outputMessage(lIter->mLogLevel, lIter->mType, lIter->mClass, lIter->mMessage);
  }
  mBuffer.clear();
  Beagle_StackTraceEndM("void LoggerXML::postInit(System& ioSystem)");
}



/*!
 *  \brief Terminate XML logger operations.
 */
void LoggerXML::terminate()
{
  Beagle_StackTraceBeginM();
  if(mTerminated == false) {
    mTerminated = true;
    if(mStreamerFile != NULL) {
      mStreamerFile->closeTag();
      mStreamerFile->closeTag();
      *mLogOutStream << std::endl;
      delete mStreamerFile;
      mStreamerFile = NULL;
    }
    if(mStreamerConsole != NULL) {
      mStreamerConsole->closeTag();
      mStreamerConsole->closeTag();
      std::cout << std::endl;
      delete mStreamerConsole;
      mStreamerConsole = NULL;
    }
    if(mLogOutStream) {
      mLogOutStream->close();
      delete mLogOutStream;
      mLogOutStream = NULL;
    }
  }
  Beagle_StackTraceEndM("void LoggerXML::terminate()");
}

