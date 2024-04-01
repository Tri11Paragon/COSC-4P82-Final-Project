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
 *  \file   beagle/src/Logger.cpp
 *  \brief  Implementation of class Logger.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */
 
#include "beagle/Beagle.hpp"

#include <ctime>
#include <iomanip>
#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a logger.
 */
Logger::Logger() :
  Component("Logger"),
  mInitialized(false)
{ }


/*!
 *  \brief Initialize logger by registering its parameters.
 *  \param ioSystem Reference to the system to use for initialization.
 */
void Logger::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle_LogDetailedM(
    ioSystem.getLogger(),
    "logger", "Beagle::Logger",
    "Initializing logger"
  );
  Beagle_StackTraceEndM("void Logger::initialize(System& ioSystem)");
}








