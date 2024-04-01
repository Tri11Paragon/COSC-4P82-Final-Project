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
 *  \file   beagle/LoggerXML.hpp
 *  \brief  Definition of the class LoggerXML.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.1 $
 *  $Date: 2007/05/09 01:51:15 $
 */
 
#ifndef Beagle_LoggerXML_hpp
#define Beagle_LoggerXML_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Logger.hpp"

namespace Beagle {

/*!
 *  \class LoggerXML beagle/LoggerXML.hpp "beagle/LoggerXML.hpp"
 *  \brief Beagle's XML logger class. Log to in a XML format to file or console.
 *  \ingroup ECF
 */
class LoggerXML : public Logger {

public:

  //! LoggerXML allocator type.
  typedef AllocatorT<LoggerXML,Logger::Alloc>
          Alloc;
  //! LoggerXML handle type.
  typedef PointerT<LoggerXML,Logger::Handle>
          Handle;
  //! LoggerXML bag type.
  typedef ContainerT<LoggerXML,Logger::Bag>
          Bag;

           LoggerXML();
  virtual ~LoggerXML();

  virtual void initialize(System& ioSystem);
  virtual void outputMessage(unsigned int inLevel,
                             string inType,
                             string inClass,
                             string inMessage);
  virtual void outputObject(unsigned int inLevel,
                            string inType,
                            string inClass,
                            const Object& inObject);
  virtual void postInit(System& ioSystem);
  virtual void terminate();


protected:

  UInt::Handle         mLogConsoleLevel;  //!< Console log level.
  UInt::Handle         mLogFileLevel;     //!< XML log level used.
  String::Handle       mLogFileName;      //!< File name to log output into.
  Bool::Handle         mShowLevel;        //!< Show log level in logs.
  Bool::Handle         mShowType;         //!< Show type of message in logs.
  Bool::Handle         mShowClass;        //!< Show class that output the message in logs.
  string          mActualFileName;   //!< Actual filename used to log message.
  PACC::XML::Streamer* mStreamerFile;     //!< Streamer used to output messages in file.
  std::ofstream*       mLogOutStream;     //!< Pointer to stream to log output.
  PACC::XML::Streamer* mStreamerConsole;  //!< Streamer used to output messages at standard output.
  bool                 mTerminated;       //!< Flag whether logger is terminated or not.

public:

  LoggerXML(const LoggerXML& inLoggerXML);              // Copy constructor prohibited!
  LoggerXML& operator=(const LoggerXML& inLoggerXML);   // operator= prohibited!

};

}

#endif // Beagle_LoggerXML_hpp

