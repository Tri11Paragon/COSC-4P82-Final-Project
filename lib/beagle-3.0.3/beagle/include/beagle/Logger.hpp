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
 *  \file   beagle/Logger.hpp
 *  \brief  Definition of the class Logger.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:15 $
 */
 
#ifndef Beagle_Logger_hpp
#define Beagle_Logger_hpp

#include <ctime>
#include <fstream>
#include <string>
#include <list>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Component.hpp"
#include "beagle/UInt.hpp"
#include "beagle/String.hpp"
#include "beagle/Bool.hpp"


/*!
 *  \def Beagle_LogBasicM
 *  \brief Log given message if log level is equal to or more than "Basic (1)".
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogBasicM(ioLogger,inType,inClass,inMessage) \
if(ioLogger.isInitialized()==false) \
  ioLogger.addToBuffer((unsigned int)Beagle::Logger::eBasic,Beagle::string(inType), \
                       Beagle::string(inClass),Beagle::string(inMessage)); \
else ioLogger.outputMessage(Beagle::Logger::eBasic,Beagle::string(inType), \
                            Beagle::string(inClass),Beagle::string(inMessage))

/*!
 *  \def Beagle_LogStatsM
 *  \brief Log given message if log level is equal to or more than "Stats (2)".
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogStatsM(ioLogger,inType,inClass,inMessage) \
if(ioLogger.isInitialized()==false) \
  ioLogger.addToBuffer((unsigned int)Beagle::Logger::eStats,Beagle::string(inType), \
                       Beagle::string(inClass),Beagle::string(inMessage)); \
else ioLogger.outputMessage(Beagle::Logger::eStats,Beagle::string(inType), \
                            Beagle::string(inClass),Beagle::string(inMessage))

/*!
 *  \def Beagle_LogInfoM
 *  \brief Log given message if log level is equal to or more than "Info (3)".
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogInfoM(ioLogger,inType,inClass,inMessage) \
if(ioLogger.isInitialized()==false) \
  ioLogger.addToBuffer((unsigned int)Beagle::Logger::eInfo,Beagle::string(inType), \
                       Beagle::string(inClass),Beagle::string(inMessage)); \
else ioLogger.outputMessage(Beagle::Logger::eInfo,Beagle::string(inType), \
                            Beagle::string(inClass),Beagle::string(inMessage))

/*!
 *  \def Beagle_LogDetailedM
 *  \brief Log given message if log level is equal to or more than "Detailed (4)".
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogDetailedM(ioLogger,inType,inClass,inMessage) \
if(ioLogger.isInitialized()==false) \
  ioLogger.addToBuffer((unsigned int)Beagle::Logger::eDetailed,Beagle::string(inType),\
                       Beagle::string(inClass),Beagle::string(inMessage)); \
else ioLogger.outputMessage(Beagle::Logger::eDetailed,Beagle::string(inType), \
                            Beagle::string(inClass),Beagle::string(inMessage))

/*!
 *  \def Beagle_LogTraceM
 *  \brief Log given message if log level is equal to or more than "Trace (5)".
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#define Beagle_LogTraceM(ioLogger,inType,inClass,inMessage) \
if(ioLogger.isInitialized()==false) \
  ioLogger.addToBuffer((unsigned int)Beagle::Logger::eTrace,Beagle::string(inType), \
                       Beagle::string(inClass),Beagle::string(inMessage)); \
else ioLogger.outputMessage(Beagle::Logger::eTrace,Beagle::string(inType), \
                            Beagle::string(inClass),Beagle::string(inMessage))

/*!
 *  \def Beagle_LogVerboseM
 *  \brief Log given message if log level is equal to or more than "Verbose (6)".
 *    Executed only when the framework is compiled with optimisation mode off.
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_LogVerboseM(ioLogger,inType,inClass,inMessage) \
if(ioLogger.isInitialized()==false) \
  ioLogger.addToBuffer((unsigned int)Beagle::Logger::eVerbose,Beagle::string(inType), \
                       Beagle::string(inClass),Beagle::string(inMessage)); \
else ioLogger.outputMessage(Beagle::Logger::eVerbose,Beagle::string(inType), \
                            Beagle::string(inClass),Beagle::string(inMessage))
#else  // BEAGLE_NDEBUG
#define Beagle_LogVerboseM(ioLogger,inType,inClass,inMessage)
#endif // BEAGLE_NDEBUG
    
/*!
 *  \def Beagle_LogDebugM
 *  \brief Log given message if log level is equal to or more than "Debug (7)".
 *    Executed only when the framework is compiled with full debug mode activated,
 *    and optimisation mode off.
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inMessage Message to log.
 *  \ingroup Sys
 */
#if defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)
#define Beagle_LogDebugM(ioLogger,inType,inClass,inMessage) \
if(ioLogger.isInitialized()==false) \
  ioLogger.addToBuffer((unsigned int)Beagle::Logger::eDebug,Beagle::string(inType), \
                       Beagle::string(inClass),Beagle::string(inMessage)); \
else ioLogger.outputMessage((unsigned int)Beagle::Logger::eDebug,Beagle::string(inType), \
                            Beagle::string(inClass),Beagle::string(inMessage))
#else  // defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)
#define Beagle_LogDebugM(ioLogger,inType,inClass,inMessage)
#endif // defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)

/*!
 *  \def Beagle_LogObjectM
 *  \brief Log given Beagle object if log level is equal to or more than the given one.
 *  \param ioLogger Logger to log message into.
 *  \param inLogLevel Log level of the message
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inObject Beagle object to log.
 *  \ingroup Sys
 */
#define Beagle_LogObjectM(ioLogger,inLogLevel,inType,inClass,inObject) \
if(ioLogger.isInitialized()==false) \
  ioLogger.addToBuffer((unsigned int)inLogLevel,Beagle::string(inType), \
                       Beagle::string(inClass),(inObject).serialize()); \
else ioLogger.outputObject((unsigned int)inLogLevel,Beagle::string(inType), \
                           Beagle::string(inClass),inObject)

/*!
 *  \def Beagle_LogObjectDebugM
 *  \brief Log given Beagle object if log level is equal to or more than "Debug (7)".
 *    Executed only when the framework is compiled with full debug mode activated,
 *    and optimisation mode off.
 *  \param ioLogger Logger to log message into.
 *  \param inType Type of the message to log.
 *  \param inClass Class name associated to the message to log.
 *  \param inObject Beagle object to log.
 *  \ingroup Sys
 */
#if defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)
#define Beagle_LogObjectDebugM(ioLogger,inType,inClass,inObject) \
if(ioLogger.isInitialized()==false) \
  ioLogger.addToBuffer(Beagle::Logger::eDebug,Beagle::string(inType), \
                       Beagle::string(inClass),(inObject).serialize()); \
else ioLogger.outputObject(Beagle::Logger::eDebug,Beagle::string(inType), \
                           Beagle::string(inClass),inObject)
#else  // defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)
#define Beagle_LogObjectDebugM(ioLogger,inType,inClass,inObject)
#endif // defined (BEAGLE_FULL_DEBUG) && ! defined (BEAGLE_NDEBUG)


namespace Beagle {

// Forward declaration
class System;

/*!
 *  \class Logger beagle/Logger.hpp "beagle/Logger.hpp"
 *  \brief Beagle's logger class. Log to a file.
 *  \ingroup ECF
 *  \ingroup Sys
 */
class Logger : public Component {

public:

  /*!
   *  \brief Log levels available in the logger. A given level imply all the previous levels.
   */
  enum LogLevel {
    eNothing=0,         //!< Nothing.
    eBasic=1,           //!< Essential informations.
    eStats=2,           //!< eBasic   + evolution statistics.
    eInfo=3,            //!< eStats   + general informations.
    eDetailed=4,        //!< eCommon  + details on operations.
    eTrace=5,           //!< eDetails + trace of the algorithms.
    eVerbose=6,         //!< eTrace   + details on everything (disabled in optimization mode).
    eDebug=7            //!< Debug (enabled only in full debug mode).
  };

  //! Logger allocator type.
  typedef AllocatorT<Logger,Component::Alloc>
          Alloc;
  //! Logger handle type.
  typedef PointerT<Logger,Component::Handle>
          Handle;
  //! Logger bag type.
  typedef ContainerT<Logger,Component::Bag>
          Bag;

           Logger();
  virtual ~Logger() { }

  virtual void initialize(System& ioSystem);
  virtual void postInit(System& ioSystem) { }

  /*!
   *  \brief Write message to the output device.
   *  \param inLevel Log level of the message
   *  \param inType Type of the message to log.
   *  \param inClass Class associated to the message.
   *  \param inMessage Message to log in the output device.
   */
  virtual void outputMessage(unsigned int inLevel,
                             string inType,
                             string inClass,
                             string inMessage) =0;

  /*!
   *  \brief Write Beagle object to the output device.
   *  \param inLevel Log level of the message
   *  \param inType Type of the message to log.
   *  \param inClass Class associated to the message.
   *  \param inObject Beagle object to log in the output device.
   */
  virtual void outputObject(unsigned int inLevel,
                            string inType,
                            string inClass,
                            const Object& inObject) =0;

  /*!
   *  \brief Add message to buffer for output when logger will be initialized.
   *  \param inLevel Log level of the message.
   *  \param inType Type of the message to log.
   *  \param inClass Class name associated to the message.
   *  \param inMessage Message to log.
   */
  inline void addToBuffer(unsigned int inLevel,
                          string inType,
                          string inClass,
                          string inMessage)
  {
    mBuffer.push_back(Message(inLevel,inType,inClass,inMessage));
  }

  /*!
   *  \return True if log level is initialized, false if not.
   */
  inline bool isInitialized() const
  {
    return mInitialized;
  }

  /*!
   *  \brief Log given message if log level is greater or equal than the level associated.
   *  \param inLevel Log level of the message.
   *  \param inType Type of the message to log.
   *  \param inClass Class name associated to the message.
   *  \param inMessage Message to log.
   */
  inline void log(unsigned int inLevel,
                  string inType,
                  string inClass,
                  string inMessage)
  {
    Beagle_StackTraceBeginM();
    if(mInitialized==false) addToBuffer(inLevel,inType,inClass,inMessage);
    else outputMessage(inLevel,inType,inClass,inMessage);
    Beagle_StackTraceEndM("void log(unsigned int inLevel, string inType, string inClass, string inMessage)");
  }

  /*!
   *  \brief Log current time if log level is equal to than the level associated.
   *  \param inLevel Log level of the message.
   */
  inline void logCurrentTime(unsigned int inLevel)
  {
    Beagle_StackTraceBeginM();
    std::time_t lTime = std::time(NULL);
    std::tm* lTM = std::localtime(&lTime);
    char lFormatedTime[512];
    std::strftime(lFormatedTime, 512, "%X %d %b %Y", lTM);
    string lMessage = string("Current date and time: ")+string(lFormatedTime);
    if(mInitialized) outputMessage(inLevel, "logger", "Beagle::Logger", lMessage);
    else addToBuffer(inLevel, "logger", "Beagle::Logger", lMessage);
    Beagle_StackTraceEndM("void logCurrentTime(unsigned int inLevel)");
  }

  /*!
   *  \brief Log given Beagle object if log level is greater or equal than the level associated.
   *  \param inLevel Log level of the message.
   *  \param inType Type of the message to log.
   *  \param inClass Class name associated to the message.
   *  \param inObject Beagle object to log.
   */
  inline void logObject(unsigned int inLevel,
                        string inType,
                        string inClass,
                        const Object& inObject)
  {
    Beagle_StackTraceBeginM();
    if(mInitialized==false) addToBuffer(inLevel,inType,inClass,inObject.serialize());
    else outputObject(inLevel,inType,inClass,inObject);
    Beagle_StackTraceEndM("void logObject(unsigned int inLevel, string inType, string inClass, const Object& inObject)");
  }

  /*!
   *  \brief Terminate logger.
   */
  virtual void terminate() { }

protected:

  struct Message {
    unsigned int mLogLevel;
    string  mType;
    string  mClass;
    string  mMessage;

    Message(unsigned int inLogLevel=0,
            string  inType="unknown",
            string  inClass="unknown",
            string  inMessage="") :
      mLogLevel(inLogLevel),
      mType(inType),
      mClass(inClass),
      mMessage(inMessage)
    { }

    friend class std::list< Beagle::Logger::Message,BEAGLE_STLALLOCATOR<Beagle::Logger::Message> >;
  };

  std::list< Message,BEAGLE_STLALLOCATOR<Message> > mBuffer;  //!< Messages buffer.
  bool               mInitialized;    //!< Flag whether the logger is initialized or not.

};

}

#endif // Beagle_Logger_hpp
